#include "Cgi.hpp"

Cgi::Cgi(void)
{
}

Cgi::~Cgi(void){}

std::string Cgi::getMimeType(std::string fileName)
{
	std::string res = tools::getMimeType(fileName);
	return (res);
}
