#ifndef CGI_HPP
#define CGI_HPP
#include <map>
#include <iostream>
#include <string>
#include "../tools/tools.hpp"
#include "../request/Request.hpp"
#include "../parseConfig/serverBlock.hpp"

class Cgi
{
	public:
	Cgi();
	~Cgi();
	std::string	getMimeType(std::string fileName);
	void init(void);
	void run(Request R);
	std::string getBody();
	int getStatus();
	void setIsIt(int n);
	int getIsIt() const;
	private:
	std::string	_body;
	int			_status;
	int			_isItCgi;
};

#endif
