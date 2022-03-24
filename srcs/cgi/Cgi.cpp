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

void Cgi::setIsIt(int N)
{
	this->_isItCgi = N;
}

int Cgi::getIsIt() const
{
	return (this->_isItCgi);
}
