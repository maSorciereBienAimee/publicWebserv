#ifndef RESPONSE_HPP
#define RESPONSE_HPP
# include "Request.hpp"

class Response
{
	public:
	Response();
	~Response();
	void launch();
	std::string getHeaders();
	int getBody();
	std::string responseGet();
	std::string responsePost();
	std::string responseDelete();
	void initErrors();
	
	private :
	std::string header; 
	std::string body;
	int status;
	std::string type;
	std::string errorPath;
	std::map<int, std::string> errors;
	Request request;
	int fd;
};

#endif
