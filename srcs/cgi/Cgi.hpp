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
	Cgi(Request R, serverLocation loc, serverBlock serv, std::string query, std::string simple, std::string rp);
	~Cgi();
	std::string							getMimeType(std::string fileName);
	void								init(void);
	std::string							getBody() const;
	std::map<std::string, std::string>	getHeaders() const;
	int									getStatus() const;
	void								setIsIt(int n);
	int									getIsIt() const;
	void								getEnv(int i);
	void								setReal(std::string str);
	void								setQuery(std::string str);
	void								setSimple(std::string str);
	std::string							getQuery(std::string str);
	std::string							getR() const;
	std::string							getQ() const;
	std::string							getS() const;
	std::string							getPathWithoutPathInfo(std::string str, std::string ext);
	std::string							getPathWithoutQuery(std::string str, std::string ext);
	std::string							getPathInfo(std::string str, std::string ext);
	void								cgiRun();
	std::string							getHex(std::string path);
	void								setResponse(std::string str, int r);

	private:
	std::map<std::string, std::string>	_headers;
	std::string							_body;
	std::string							_query;
	std::string							_simple;
	std::string							_realPath;
	int									_status;
	int									_isItCgi;
	Request								_request;
	serverLocation						_loc;
	serverBlock							_serv;
	char								**_env;
};

#endif
