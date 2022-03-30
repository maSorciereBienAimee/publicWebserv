#ifndef CGI_HPP
#define CGI_HPP
#include <map>
#include <iostream>
#include <string>
#include <unistd.h>
#include "../tools/tools.hpp"
#include "../request/Request.hpp"
#include "../parseConfig/serverBlock.hpp"

class Cgi
{
	public:
	Cgi(Request R, serverLocation loc, serverBlock serv, std::string query, std::string simple);
	~Cgi();
	std::string	getMimeType(std::string fileName);
	void init(void);
	std::string getBody();
	int getStatus();
	void setIsIt(int n);
	int getIsIt() const;
	void getEnv();
	std::string getQuery(std::string str);
	std::string getPathWithoutPathInfo(std::string str, std::string ext);
	std::string getPathWithoutQuery(std::string str, std::string ext);
	std::string getPathInfo(std::string str, std::string ext);
	void cgiRun();
	std::string getHex(std::string path);

	private:
	std::string		_body;
	std::string		_query;
	std::string		_simple;
	int				_status;
	int				_isItCgi;
	Request			_request;
	serverLocation	_loc;
	serverBlock		_serv;
	char **_env;
};

#endif
