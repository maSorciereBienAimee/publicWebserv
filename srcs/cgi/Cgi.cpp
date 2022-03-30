#include "Cgi.hpp"
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>

Cgi::Cgi(Request R, serverLocation loc, serverBlock serv, std::string Q, std::string S) : _request(R), _loc(loc), _serv(serv), _query(Q), _simple(S)
{
	_env = NULL;
}

Cgi::~Cgi(void)
{
}

std::string Cgi::getMimeType(std::string fileName)
{
	std::string res = tools::getMimeType(fileName);
	return (res);
}

void Cgi::setIsIt(int N)
{
	this->_isItCgi = N;
}

int Cgi::getIsIt() const
{
	return (this->_isItCgi);
}

std::string Cgi::getPathInfo(std::string str, std::string ext)
{
	std::string temp = "";
	std::string::iterator it = str.begin();
	std::string::iterator it2;
	int i = 0;
	int j = 0;
	for (; it != str.end(); it++)
	{
		temp = str.substr(i, ext.size());
		i++;
		if (temp == ext)
		{
			j = 0;
			while (j < ext.size() - 1)
			{
				it++;
				i++;
				j++;
			}
			break;
		}
	}
	it2 = it;
	j = 0;
	for (; it2 != str.end(); it2++)
	{
		if (*it2 == '?')
		{
			j--;
			break;
		}
		j++;
	}
	temp = str.substr(i, j);
	if (temp == "/")
			temp = "";
	return (temp);
}

std::string Cgi::getPathWithoutQuery(std::string str, std::string ext)
{
	std::string temp = "";
	std::string::iterator it = str.begin();
	std::string::iterator it2;
	int i = 0;
	int j = 0;
	for (; it != str.end(); it++)
	{
		temp = str.substr(i, ext.size());
		i++;
		if (temp == ext)
		{
			j = 0;
			while (j < ext.size() - 1)
			{
				it++;
				i++;
				j++;
			}
			break;
		}
	}
	it2 = it;
	j = i;
	for (; it2 != str.end(); it2++)
	{
		if (*it2 == '?')
		{
			j--;
			break;
		}
		j++;
	}
	temp = str.substr(0, j);
	if (temp == "/")
			temp = "";
	return (temp);
}

std::string Cgi::getPathWithoutPathInfo(std::string str, std::string ext)
{
	std::string temp = "";
	std::string::iterator it = str.begin();
	int i = 0;
	int j = 0;
	for (; it != str.end(); it++)
	{
		temp = str.substr(i, ext.size());
		i++;
		if (temp == ext)
		{
			j = 0;
			while (j < ext.size() - 1)
			{
				it++;
				i++;
				j++;
			}
			break;
		}
	}
	temp = str.substr(0, i);
	if (temp == "/")
			temp = "";
	return (temp);
}

std::string Cgi::getQuery(std::string str)
{
	int res;
	std::string result = "";
	res = str.find('?');
	if (res != std::string::npos)
	{
		result = str.substr(res + 1, str.size() - res);
	}
	return (result);
}

std::string Cgi::getHex(std::string path)
{
	std::string ret;
	char c;
	int i;
	int x;
	for (i=0; i < path.length(); i++)
	{
		if (path[i] == '%')
		{
			sscanf(path.substr(i + 1,2).c_str(), "%x", &x);
			c = static_cast<char>(x);
			ret += c;
			if (i + 2 <= path.length())
				i = i + 2;
			else
				i++;
		}
		else
			ret += path[i];
	}
	return (ret);
}

void Cgi::getEnv()
{
	std::map<std::string, std::string>	requestHeader = _request.getHeaders();
	std::map<std::string, std::string>	mapEnv;
	std::string pathWQ = getPathWithoutQuery(_request.getPath(), _loc.getCgiExt());
	std::string pathWPI = getPathWithoutPathInfo(pathWQ, _loc.getCgiExt());
	mapEnv["REDIRECT_STATUS"] = "200";
	mapEnv["SERVER_SOFTWARE"] = _serv.getHostStr() + ":" + _serv.getPortStr()+ "/1.1"; //???
	mapEnv["SERVER_NAME"] = _serv.getHostStr();
	mapEnv["GATEWAY_INTERFACE"] = "CGI/1.1";
	mapEnv["SERVER_PROTOCOL"] = "HTTP/1.1";
	mapEnv["SERVER_PORT"] = _serv.getPortStr();
	mapEnv["REQUEST_METHOD"] = _request.getMethod();
	mapEnv["PATH_INFO"] = getPathInfo(_request.getPath(), _loc.getCgiExt());
	if (mapEnv["PATH_INFO"] == "")
		mapEnv["PATH_TRANSLATED"] = "";
	else
		mapEnv["PATH_TRANSLATED"] = pathWPI + getHex(mapEnv["PATH_INFO"]);
	mapEnv["SCRIPT_FILENAME"] = "./website/index.php";//_loc.getCgiBin(); //plutot nom du fichier envoye ? 
	mapEnv["SCRIPT_NAME"] = _loc.getCgiBin();
	mapEnv["QUERY_STRING"] = getQuery(_request.getPath());
	mapEnv["REMOTE_HOST"] = _serv.getHostStr();
	mapEnv["REMOTE_ADDR"] = _serv.getHostStr();
	if (requestHeader.find("Authorization") != requestHeader.end()
			&& requestHeader["Authorisation"] != "")
		mapEnv["AUTH_TYPE"] = requestHeader["Authorization"];
	mapEnv["REMOTE_USER"] = requestHeader["Authorization"];
	mapEnv["REMOTE_IDENT"] = requestHeader["Authorization"];
	mapEnv["CONTENT_TYPE"] = requestHeader["Content-Type"];
	std::string body = "";
	std::fstream file;
	char c;
	file.open("./website/index.php", std::ios::in);
	while (1)
	{
		file >> std::noskipws >> c;
		if (file.eof())
			break;
	body.push_back(c);
	}
	file.close();
	int len = body.size();
	std::string length;
	std::stringstream ss;
	ss << len;
	ss >> length;
	if (requestHeader.find("Content-Length") != requestHeader.end()
			&& requestHeader["Content-Length"] != "")
		mapEnv["CONTENT_LENGTH"] = requestHeader["Content-Length"];
	else
		mapEnv["CONTENT_LENGTH"] = length;
	if (requestHeader.find("Accept") != requestHeader.end()
			&& requestHeader["Accept"] != "")
		mapEnv["HTTP_ACCEPT"] = requestHeader["Accept-Language"];
	if (requestHeader.find("Accept-Language") != requestHeader.end()
			&& requestHeader["Accept"] != "")
	mapEnv["HTTP_ACCEPT_LANGUAGE"] = requestHeader["Accept-Language"];
	if (requestHeader.find("User-Agent") != requestHeader.end()
			&& requestHeader["User-Agent"] != "")
	mapEnv["HTTP_USER_AGENT"] = requestHeader["User-Agent"];

	_env = new char *[mapEnv.size() + 1];
	_env[mapEnv.size()] = 0;
	int i = 0;
	int j;
	for (std::map<std::string, std::string>::iterator it = mapEnv.begin(); it != mapEnv.end(); it++)
	{
		std::string keyValue = it->first + "=" + it->second;
		_env[i] = new char[it->first.size() + it->second.size() + 2];
		j = 0;
		for (std::string::iterator its = keyValue.begin(); its != keyValue.end(); its++)
		{
			_env[i][j] = *its;
			j++;
		}
		_env[i][j] = '\0';
		i++;
	}
}

void	Cgi::cgiRun()
{
	pid_t pid;
	int fd[2];
	int i = 0;
	int stockOut = dup(1);
	int stockIn = dup(0);
	int status;
	char buf[1024];
	int res; 
	std::string str = "";
	std::string temp;
	char **arg = NULL;
	std::string body = "";
	std::fstream file;
	char c;
	
//	file.open("./website/index.php", std::ios::in);
//	while (1)
//	{
//		file >> std::noskipws >> c;
//		if (file.eof())
//			break;
//		body.push_back(c);
//	}
//	file.close();

	
	
	std::FILE *tmp = std::tmpfile();
	getEnv();
	int fdTmp = fileno(tmp);
	pipe(fd);	
	pid = fork();
	if (pid < 0)
	{
		std::cerr << "Can't fork(), error" << std::endl;
		return;
	}
	else if (pid == 0)
	{
		if (fd[1] > 0)
			close (fd[1]);
		if (dup2(fd[0], 0) < 0)
		{
			std::cerr << "Error dup2 in cgiRun()" << std::endl;
			return;
		}
		dup2(1, fdTmp);
		execve(_loc.getCgiBin().c_str(), arg, _env);
		std::cout << "execve est retourne" << std::endl;
	}
	else
	{
		if (fd[0])
				close(fd[0]);
		if (dup2(fd[1], 1) < 0)
		{
			std::cerr << "Error dup2 in cgiRun()" << std::endl;
			return;
		}
		write(1, body.c_str(), body.size());
	close(1);
	close(fd[1]);
//	close(0);
	waitpid(pid, &status, 0);	
	dup2(stockOut, 1);
	i = 0;
	if (_env != NULL)
	{
		while (_env[i])
		{
			delete [] _env[i];
			i++;
		}
		delete [] _env;
	}
	//	res = 1;
	//	while (res > 0)
	//	{
	//		res = read(fd2[0], &buf, 1023);
	//		temp = buf;
	//		str += temp;
	//	}
//		std::cout << "YO THIS ID THE RETURN OF CGIRUN :  " << str  << "|" << std::endl;
	close(fdTmp);
	}
}
