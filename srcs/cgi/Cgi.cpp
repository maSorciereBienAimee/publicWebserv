#include "Cgi.hpp"
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <istream>

Cgi::Cgi(Request R, serverLocation loc, serverBlock serv, std::string Q, std::string S, std::string rp) :  _query(Q), _simple(S), _realPath(rp), _request(R), _loc(loc), _serv(serv)
{
	_env = NULL;
	_headers.clear();
}

Cgi::~Cgi(void) { }

std::string Cgi::getMimeType(std::string fileName)
{
	std::string res = tools::getMimeType(fileName);
	return (res);
}

void Cgi::setSimple(std::string newSimple)
{
	this->_simple = newSimple;
}

void Cgi::setQuery(std::string newQuery)
{
	this->_query = newQuery;
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
	size_t i = 0;
	size_t j = 0;
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
	unsigned long i = 0;
	unsigned long j = 0;
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
	unsigned long i = 0;
	unsigned long j = 0;
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
	size_t res;
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
	size_t i;
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

std::string Cgi::getR() const
{
	return (this->_realPath);
}

void Cgi::setReal(std::string newPath)
{
	this->_realPath = newPath;
}

void Cgi::getEnv(int len)
{
	std::string pathFile = _realPath;
	std::map<std::string, std::string>	requestHeader = _request.getHeaders();
	std::map<std::string, std::string>	mapEnv;
	std::string pathWQ = getPathWithoutQuery(_request.getPath(), _loc.getCgiExt());
	std::string pathWPI = getPathWithoutPathInfo(pathWQ, _loc.getCgiExt());
	std::string body = "";
	std::fstream file;
	mapEnv["REDIRECT_STATUS"] = "200";
	mapEnv["SERVER_SOFTWARE"] = _serv.getName()+ "/1.0";
	mapEnv["SERVER_NAME"] = _serv.getHostStr();
	mapEnv["GATEWAY_INTERFACE"] = "CGI/1.1";
	mapEnv["SERVER_PROTOCOL"] = "HTTP/1.1";
	mapEnv["SERVER_PORT"] = _serv.getPortStr();
	mapEnv["REQUEST_METHOD"] = _request.getMethod();
	mapEnv["PATH_INFO"] = pathFile + getQuery(_query);
	if (mapEnv["PATH_INFO"] == "")
		mapEnv["PATH_TRANSLATED"] = "";
	else
		mapEnv["PATH_TRANSLATED"] = getHex(mapEnv["PATH_INFO"]);
	if (_simple != "")
		mapEnv["SCRIPT_FILENAME"] = pathFile; 
	mapEnv["SCRIPT_NAME"] = _loc.getCgiBin();
	mapEnv["QUERY_STRING"] = getQuery(_query);
	mapEnv["REMOTE_HOST"] = _serv.getHostStr();
	mapEnv["REMOTE_ADDR"] = _serv.getHostStr();
	if (requestHeader.find("Authorization") != requestHeader.end()
			&& requestHeader["Authorisation"] != "")
		mapEnv["AUTH_TYPE"] = requestHeader["Authorization"];
	mapEnv["REQUEST_URI"] = pathFile + getQuery(_query);
	mapEnv["REMOTE_USER"] = requestHeader["Authorization"];
	mapEnv["REMOTE_IDENT"] = requestHeader["Authorization"];
	if (requestHeader.find("Content-Type") != requestHeader.end()
			&& requestHeader["Content-Type"] != "")
		mapEnv["CONTENT_TYPE"] = requestHeader["Content-Type"];
	else
		mapEnv["CONTENT_TYPE"] = tools::getMimeType(pathFile);
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
	int status;
	char buf[1024];
	std::string str = "";
	std::string temp;
	char **arg = NULL;
	std::FILE *tmp = std::tmpfile();
	int fdTmp = fileno(tmp);

	std::string body = "";
	if (_request.getMethod() == "POST")
		body = _request.getBody();
	else if (_request.getMethod() == "GET")
	{
		std::fstream file;
		char c;
		file.open(_realPath.c_str(), std::ios::in);
		while (1)
		{
			file >> std::noskipws >> c;
			if (file.eof())
				break;
			body.push_back(c);
		}
		file.close();
	}

	getEnv(body.size());
	pipe(fd);	
	pid = fork();
	if (pid < 0)
	{
		std::cerr << RED << "error fork()" << RESET << std::endl;
		_headers.clear();
		_body = "";
		_status = 500;
		return;
	}
	else if (pid == 0)
	{
		if (fd[1] > 0)
			close (fd[1]);
		if (dup2(fd[0], 0) < 0)
		{
			std::cerr << RED << "Error dup2 in cgiRun() child" << RESET << std::endl;
			_headers.clear();
			_body = "";
			_status = 500;
			return;
		}
		close(fd[0]);
		dup2(fdTmp, 1);
		execve(_loc.getCgiBin().c_str(), arg, _env);
		std::cerr << RED << "error execve()" << RESET << std::endl;
		exit (1);
	}
	else
	{
		if (fd[0])
				close(fd[0]);
		if (dup2(fd[1], 1) < 0)
		{
			std::cerr << RED << "Error dup2 in cgiRun() parent" << RESET << std::endl;
			_headers.clear();
			_body = "";
			_status = 500;
			return;
		}
		std::cout << body << std::endl;
		dup2(stockOut, 1);
		close(stockOut);
		close(fd[1]);
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
		_env = NULL;
		rewind(tmp);
		while(!feof(tmp))
		{
			if (fgets(buf, 1024, tmp) == NULL)
				break;
			temp = buf;
			str += temp;
		}
		close(fdTmp);
		std::fclose(tmp);
		setResponse(str, WEXITSTATUS(status));		
	}
}

void	Cgi::setResponse(std::string str, int retStat)
{
	size_t	sep;
	int i = 0;
	size_t len;
	std::string head;
	std::string headS;
	std::string key;
	std::string val;
	std::string temp;

	if (retStat == 1)
	{
		_headers.clear();
		_body = "<html></html>";
		_status = 200;
		return ;
	}
	else
	{
		_status = 200;
		_body = str;
		_headers.clear();
		sep = str.find("\r\n\r\n", 0);
		if (sep != std::string::npos)
		{
			if (sep < str.size())
				sep += 4;
			_body = str.substr(sep, str.size() - sep);
			head = str.substr(0, sep - 4);
			headS = head;
			for (std::string::iterator it = head.begin(); it != head.end(); it++)
			{
				if (*it == ':')
				{
					key = headS.substr(0, i);
					len = headS.size() - (i + 1);
					temp = headS.substr(i + 1, len);
					headS = temp;
					i = 0;
				}
				else if (*it == '\n' || *it == '\r')
				{
					val = headS.substr(0, i);
					_headers.insert(std::make_pair(key, val));
					len = headS.size() - (i + 1);
					temp = headS.substr(i + 1, len);
					headS = temp;
					it++;
					i = 0;
				}
			}
			val = headS.substr(0, i);
			_headers.insert(std::make_pair(key, val));
		}
	}
	for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); it++)
	{
		if (it->first == "Status")
			_headers.erase(it);
	}
}

int Cgi::getStatus() const
{
	return (this->_status);
}

std::string Cgi::getBody() const
{
	return (this->_body);
}
std::string Cgi::getQ() const
{
	return (this->_query);		
}

std::string Cgi::getS() const
{
	return (this->_simple);
}

std::map<std::string, std::string> Cgi::getHeaders() const
{
	return (this->_headers);
}
