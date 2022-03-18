#include "Response.hpp"
#include <iostream>
#include <string>
#include "../manageServer/Server.hpp"
#include <sys/stat.h>

Response::Response(void) 
{
	return;
}

//(peut etre on va avoir besoin plus tard):,config(C), fd(F)
Response::Response(Request R, int F) : request(R)
{
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
	std::string reponse;

	if (this->request.getMethod() == "GET")
		_get(request);
	else if (this->request.getMethod() == "DELETE")
		_delete(request.getPath());
	else if (this->request.getMethod() == "POST")
		_post(request);
	setHeaders();
}

std::string Response::getReply()
{
	std::string reply = _header + "\r\n\r\n" + this->body;
	std::cout << "REPLY IS:\n" << reply << std::endl;
	return (reply);
}

void    Response::_delete(std::string path)
{
    struct stat check;

	path.erase(path.begin(), path.begin() + 1);
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
	readIn("file_not_found.html");
	return;
}

void Response::setHeaders()
{
	std::map<std::string, std::string>::iterator it = this->extra_headers.begin();
	this->_header = "HTTP/1.1 ";
	_header += (errors[this->status]) + "\n";
	_header += "server: DreamTeamServer/1.0\n";
	for (int i = 0; i < this->extra_headers.size(); i++)
	{
		_header += (it->first + ": " + it->second + "\n");
		it++;
	}
	_header += "Content-Length: " + body_len;
	
}

/*int Response::getBody()
{
	std::ifstream is (this->request.getPath(), std::ifstream::binary);
	if (!is)
	{
		this->status = 400; // ou selon l'erreur
		this->body = "";
		return (-1);
	}
	is.seekg (0, is.end);
	int length = is.tellg();
	is.seekg (0, is.beg);
	char * b = (char *)malloc(sizeof(char) * (length + 1));
	b[length] = '\0';
	is.read (b,length);
	std::string bufStr(b);
	this->body = bufStr;
	return (length)	
}*/

void Response::readIn(std::string file)
{
	std::ifstream is (file, std::ifstream::binary);
	if (!is)
	{
		this->status = 400;
		this->body = "";
		return;
	}
	is.seekg (0, is.end);
	int length = is.tellg();
	is.seekg (0, is.beg);
	char * b = (char *)malloc(sizeof(char) * (length + 1));
	b[length] = '\0';
	std::stringstream ss;
	ss << length;
	this->body_len = ss.str();
	is.read (b,length);
	std::string bufStr(b);
	this->body = bufStr;
	
}

void Response::_get(Request R)
{
	(void)R;
	this->status = 200;
	readIn("index.html");
}

void Response::_post(Request R)
{
	(void)R;
	status = 200;
	readIn("index.html");
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


