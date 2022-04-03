#include "Response.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include "../manageServer/Server.hpp"
#include <sys/stat.h>
#include <vector>
#include <ctime>
#include <cstring>
#include <sstream>

//Response::Response(void)
//{
//	return;
//}

//(peut etre on va avoir besoin plus tard):,config(C), fd(F)


Response::Response(Request R, int F, Cgi myCgi, serverLocation loc, serverBlock server) : request(R), _cgi(myCgi), _loc(loc), _server(server)
{
	if (F == 400)
		this->body_message = "Bad request";
	if (_loc.getRedir() == "")
	{
		redir = 0;
		_path = R.getPath();
		_old_path = "";
		_redir_path = "";

	}
	else
	{
		redir = 1;
		_redir_path = _loc.getRedir();

	}
	index_vec = _loc.getIndex();
	// std::cout << "ROOT LOCATION IS ["<< _loc.getRootLoc() << "\n";
	// std::cout << "LOCATION PATH IS [" << _loc.getLocationPath() << "\n";

	_autoindex = _loc.getAI();
	status = F;
	initErrors();
	launch();
	return;
}

Response::~Response()
{
	return;
}

void Response::launch()
{
	std::vector<std::string> methods = _loc.getMethods();
	std::string reponse;
	if (redir == 1)
	{
		std::pair<std::string, std::string> red("Location", _redir_path);
		extra_headers.insert(red);
		this->status = 307;
		this->body_message = "REDIRECTION";
	}
	else if (status == 200)
	{
		if (this->request.getMethod() == "GET" && find(methods.begin(), methods.end(), "GET") != methods.end())
			_get(request);
		else if (this->request.getMethod() == "DELETE" && find(methods.begin(), methods.end(), "DELETE") != methods.end())
			_delete(request.getPath());
		else if (this->request.getMethod() == "POST" && find(methods.begin(), methods.end(), "POST") != methods.end())
			_post(request);
		else if ((this->request.getMethod() == "GET"
			|| this->request.getMethod() == "POST"
			|| this->request.getMethod() == "DELETE")
			&& find(methods.begin(), methods.end(), this->request.getMethod()) == methods.end())
				_notAllowed();
		else
			_other(request);
	}
	//200 has index page and others have no body
	if (status != 200 && status != 204 && status != 304 && !(status > 99 && status < 200)) //peut etre a changer si on utilise d'autres code 2xx
		setBody();
	setHeaders();
}

std::string Response::getReply()
{
	std::string reply = _header;
	//THESE CODES SHOULD NOT INCLUDE BODY... SHOULD THEY INCLUDE THE /r/n/r/n though?
	if (status != 204 && status != 304 && !(status > 99 && status < 200))
		reply += "\r\n\r\n" + this->body;
	std::cout << "REPLY IS:\n" << reply << std::endl;
	return (reply);
}

void    Response::_delete(std::string path)
{
    struct stat check;
//	path.erase(path.begin(), path.begin() + 1);
	status = 200;
	if (stat(path.c_str(), &check) == 0) //could change this to c++ method with fopen, but this is faster?
    {
        if (remove(path.c_str()) == 0)
		{
		   std::pair<std::string, std::string> p1("optiona_header_check", "WORKING");
		   this->extra_headers.insert(p1);
           status = 200;
		   readIn("deleted_200.html");
		   return;
		}
    }
	status = 403;
	this->body_message = "file_not_found.html";
	return;
}

void	Response::_other(Request req)
{
	status = 501;
	this->body_message = "Server does not accept this method";
	return;
}

void Response::_notAllowed()
{
	std::string str = "";
	std::vector<std::string> m = _loc.getMethods();
	for (std::vector<std::string>::iterator it = m.begin(); it != m.end(); it++)
	{
		str += *it;
		if (it + 1 != m.end())
			str += ", ";
	}
	status = 405;
	this->body_message = "Ressource location does not accept this method";
	this->extra_headers.insert(std::make_pair("Allow", str));
	return;
}

void Response::setHeaders()
{
	std::map<std::string, std::string>::iterator it = this->extra_headers.begin();
	this->_header = "HTTP/1.1 ";
	_header += (errors[this->status]) + "\n";
	_header += "Server: " + _server.getName() + "/1.0\n";
	_header += "Date: " + makeDate();
	for (int i = 0; i < this->extra_headers.size(); i++)
	{
		_header += (it->first + ": " + it->second + "\n");
		it++;
	}
	if (status != 404 && status != 304 && !(status > 99 && status < 200))
		_header += "Content-Length: " + body_len;
}

std::string Response::makeDate(void)
{
	std::stringstream conv;
	time_t now = time(0);
	time_t current;
    char rfc_2822[40];

    time(&current);
    strftime(rfc_2822, sizeof(rfc_2822), "%a, %d %b %Y %T GMT\n", localtime(&current));
	std::cout << rfc_2822 << std::endl;
	int i = 0;
	std::string ret;
	ret = rfc_2822;
	return ret;
}

void Response::setBody()
{
	std::string code;
	std::stringstream conv;
	std::cout << "SET BODYYY\n";
	conv << status;
	code = conv.str();
	
	this->body = "<!DOCTYPE html>\n<html><head><link rel='icon' type='image/png' href='data:image/jpeg;base64,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'/></head><style>\nhtml {\ndisplay: table;\nmargin: auto;\n}\n</style>\n<body>\n<h1>";
	this->body += (code + " " + body_message);
	if (status != 307)
		this->body += "</h1>\n<img src='https://ih1.redbubble.net/image.3330216512.5449/st,small,507x507-pad,600x600,f8f8f8.jpg'></body>\n</html>";
	else
		this->body += "</h1>\n</body>\n</html>";
	std::stringstream conv2;
	conv2 << body.length();
	this->body_len = conv2.str();
	
}

void Response::readIn(std::string file)
{
	//segfault avec DIRECTORY
	//checker si le path est un dossier si oui checker si il y a un index dans le dossier
	// si oui lire l;index
	// si non retourner une erreur
	std::cout << "_READIN FILE IS: " << file << std::endl;
	std::ifstream is (file, std::ifstream::binary);
	if (!is)
	{
		std::cout << "_READIN FILE DOESN'T exist: " << file << std::endl;
		this->status = 400;
		this->body = "";
		setBody();
		return;
	}
	int len;
	std::stringstream ss;
	_cgi.setIsIt(tools::isItCgi(file, _loc));
	if (_cgi.getIsIt() == 1)
	{
		_cgi.setReal(file);
		std::cout << _cgi.getR() << std::endl;
		_cgi.cgiRun();
		this->status = _cgi.getStatus();
		this->body = _cgi.getBody();
		this->extra_headers = _cgi.getHeaders();
		len = this->body.size();
		ss << len;
		ss >> this->body_len;
		return ;
	}
	is.seekg (0, is.end);
	int length = is.tellg();
	is.seekg (0, is.beg);
	char * b = (char *)malloc(sizeof(char) * (length + 1));
	b[length] = '\0';
	ss << length;
	this->body_len = ss.str();
	is.read (b,length);
	std::string bufStr(b);
	this->body = bufStr;
	std::string fav = "";
	std::string path;
	path = request.getPath();
	if (path.size() >= 12)
		fav = path.substr(path.size() - 12, 12);
	if (fav != "favicon.html")
		std::cout << "_READIN BODY IS: " << this->body << std::endl;


}

void Response::_get(Request R)
{
	status = 200;
//	int len;
	std::string length;
//	std::stringstream ss;
	
//	if (_cgi.getIsIt() == 1)
//	{
//		_cgi.cgiRun();
//		this->status = _cgi.getStatus();
//		this->body = _cgi.getBody();
//		this->extra_headers = _cgi.getHeaders();
//		len = this->body.size();
//		ss << len;
//		ss >> this->body_len;
//		return ;
//	}
//	if (R.getPath() != (R.getRoot() + '/') )
	if (R.getPath() != _loc.getRootLoc() + '/' )
	{
		struct stat check;
		std::string path = R.getPath();
		std::cout << _autoindex << std::endl;
		std::cout << "location path = " << _loc.getLocationPath() << std::endl;
//		path.erase(path.begin(), path.begin() + 1);
		if (stat(path.c_str(), &check) == 0) //could change this to c++ method with fopen, but this is faster?
    	{
			if (S_ISREG(check.st_mode))
			{
				std::string root = _loc.getRootLoc();
//				root.erase(root.begin(), root.begin() + 1);
				if (path == root + "/favicon.ico")
					path = root + "/favicon.html";
				readIn(path);
				this->status = 200;
			}
			else if (_autoindex == 0 && S_ISDIR(check.st_mode))
			{
				DIR *dir;
				struct dirent *de;
				std::vector<std::string> findIndex;
				if ((dir = opendir(path.c_str())) != NULL)
				{
					while ((de = readdir(dir)) != NULL)
						findIndex.push_back(de->d_name);
					std::string path = tools::getIndex(findIndex);	
					if (path.empty())
					{
						this->status = 404;
						this->body_message = "File not found";
					}
					else 
					{
						std::string root = R.getRoot();
						root.erase(root.begin(), root.begin() + 1);
						std::string newPath = R.getPath() + "/" + path;
						std::cout << "NEWW PATTTTHH " << newPath << std::endl;
				//		_cgi.setIsIt(tools::isItCgi(newPath, _loc));
				//		if (_cgi.getIsIt() == 1)
				//		{
				//			_cgi.setReal(newPath);
				//			std::cout << _cgi.getR() << std::endl;
				//			_cgi.cgiRun();
				//			this->status = _cgi.getStatus();
				//			this->body = _cgi.getBody();
				//			this->extra_headers = _cgi.getHeaders();
				//			len = this->body.size();
				//			ss << len;
				///			ss >> this->body_len;
				//			return ;
				//		}
						readIn(newPath);
						this->status = 200;
					}
				}
			}
			else if (_autoindex == 1 && S_ISDIR(check.st_mode))
			{
				std::string path = R.getPath();
				this->body = tools::genreateAI(_loc.getLocationPath() , _server.getHostStr(), _server.getPortStr(), path);
				this->status = 200;
				return ;
			}
		}
		else 
		{
			this->status = 404;
			this->body_message = "File not found";
		}
	}
	else //no file specified so home page??
		_homepage(R);
}

void Response::_homepage(Request R)
{
	int size = index_vec.size();
	int i;
	this->status = 200;

	std::cout << "IN _HOMEPAGE\n";

	for (i = 0; i < size; i++)
	{
		std::string str = R.getPath() + index_vec[i];
	//	str.erase(str.begin(), str.begin() + 1);
		std::cout << str << std::endl;
		struct stat check;
		std::stringstream ss;
		int len;
		if (stat(str.c_str(), &check) == 0) //could change this to c++ method with fopen, but this is faster?
		{
//				std::cout << "rentre" << std::endl;
//			_cgi.setIsIt(tools::isItCgi(str, _loc));
//			if (_cgi.getIsIt() == 1)
//			{
//				_cgi.setReal(str);
//				std::cout << _cgi.getR() << std::endl;
//				_cgi.cgiRun();
//				std::cout << "rentre" << std::endl;
//				this->status = _cgi.getStatus();
//				this->body = _cgi.getBody();
//				this->extra_headers = _cgi.getHeaders();
//				len = this->body.size();
//				ss << len;
//				ss >> this->body_len;
//				return ;
//			}
//		std::cout << str << std::endl;

			readIn(str);
			this->status = 200;
			return;
		}
	}
	// if (_autoindex == 1)
	// {
	// 	//std::string code;
	// 		std::stringstream conv;

	// 		std::string path = R.getPath();
	// 		// std::cout << "ROOT IS _______" << R.getRoot() << "\n";
	// 		// std::cout << "PATH IS _____________ " << path << "\n";
	// 		// std::vector<std::string> dataAI = tools::getDirAI();  // change to do a function generate autoindex(host, port, path,)
	// 		// if (dataAI.empty())
	// 		// 	throw OurException("Could not open directory");
	// 		conv << status;
	// 		//code = conv.str();
	// 		//std::cout << "CODE IS " << code << "\n";
	// 		//this->body = "<!DOCTYPE html>\n<html>\n<body>\n<h1>\nAUTOINDEX</h1>\n<style>html { color-scheme: light dark; }\nbody { width: 35em; margin: left auto;\nfont-family: Tahoma, Verdana, Arial, sans-serif;\n}\n</style>\n";
	// 		// for (std::vector<std::string>::iterator it = dataAI.begin(); it != dataAI.end(); it++)
	// 		// {
	// 		// 	this->body += "\t\t<p><a href=\"http://" +  _server.getHostStr() + ":" + _server.getPortStr() + "/" + (*it) + "\">" + (*it) + "</a></p>\n";
	// 		// }
	// 	//	this->body += "</body>\n</html>\n";
	// 		this->body = tools::genreateAI(_server.getHostStr(), _server.getPortStr(), path);
	// 		std::stringstream conv2;
	// 		conv2 << body.length();
	// 		this->body_len = conv2.str();
	
	// }
	// else
	// {
		this->status = 404;
		this->body_message = "File not found";
	//}
}


void Response::_post(Request R)
{
	(void)R;
	status = 200;
	int len;
	std::stringstream ss;
	if (_cgi.getIsIt() == 1)
	{
		_cgi.cgiRun();
		this->status = _cgi.getStatus();
		this->body = _cgi.getBody();
		this->extra_headers = _cgi.getHeaders();
		len = this->body.size();
		ss << len;
		ss >> this->body_len;
		return ;
	}
	else
	{
		this->status = 204;
		this->body = "";
	}
}




void Response::initErrors()
{
	errors[100] = "100 Continue";
	errors[101] = "101 Switching Protocols";
	errors[103] = "103 Early Hints";
	errors[200] = "200 OK";
	errors[201] = "201 Created";
	errors[202] = "202 Accepted";
	errors[203] = "203 Non-Authoritative Information";
	errors[204] = "204 No Content";
	errors[205] = "205 Reset Content";
	errors[206] = "206 Partial Content";
	errors[300] = "300 Multiple Choices";
	errors[301] = "301 Moved Permanently";
	errors[302] = "302 Found";
	errors[303] = "303 See Other";
	errors[304] = "304 Not Modified";
	errors[307] = "307 Temporary Redirect";
	errors[308] = "308 Permanent Redirect";
	errors[400] = "400 Bad Request";
	errors[401] = "401 Unauthorized";
	errors[402] = "402 Payment Required";
	errors[403] = "403 Forbidden";
	errors[404] = "404 Not Found";
	errors[405] = "405 Method Not Allowed";
	errors[406] = "406 Not Acceptable";
	errors[407] = "407 Proxy Authentication Required";
	errors[408] = "408 Request Timeout";
	errors[409] = "409 Conflict";
	errors[410] = "410 Gone";
	errors[411] = "411 Length Required";
	errors[412] = "412 Precondition Failed";
	errors[413] = "413 Payload Too Large";
	errors[414] = "414 URI Too Long";
	errors[415] = "415 Unsupported Media Type";
	errors[416] = "416 Range Not Satisfiable";
	errors[417] = "417 Expectation Failed";
	errors[418] = "418 I'm a teapot";
	errors[422] = "422 Unprocessable Entity";
	errors[425] = "425 Too Early";
	errors[426] = "426 Upgrade Required";
	errors[428] = "428 Precondition Required";
	errors[429] = "429 Too Many Requests";
	errors[431] = "431 Request Header Fields Too Large";
	errors[451] = "451 Unavailable For Legal Reasons";
	errors[500] = "500 Internal Server Error";
	errors[501] = "501 Not Implemented";
	errors[502] = "502 Bad Gateway";
	errors[503] = "503 Service Unavailable";
	errors[504] = "504 Gateway Timeout";
	errors[505] = "505 HTTP Version Not Supported";
	errors[506] = "506 Variant Also Negotiates";
	errors[507] = "507 Insufficient Storage";
	errors[508] = "508 Loop Detected";
	errors[510] = "510 Not Extended";
	errors[511] = "511 Network Authentication Required";
}


