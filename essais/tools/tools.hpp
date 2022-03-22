#ifndef _TOOLS_HPP_
#define _TOOLS_HPP_

#include <iostream>
#include <string>
#include <exception>
#include <vector>
#include <sstream>
#include <limits>



namespace tools
{
	 //just for help
	void            printVector(std::vector<std::string> content);
	bool            isSpaces(std::string str);
	std::string     removeDuplicateSpaces(std::string const &str);
	unsigned int    strToIp(std::string strIp);
	bool 			isValidDirectiveLocation(std::string const &dir);
	bool 			isValidDirective(std::string const &dir);
}

struct OurExcetpion : public std::exception
{
	std::string _str;

	OurExcetpion(std::string str): _str(str){}
	~OurExcetpion() {}
	const char * what() const throw() { return _str.c_str(); }
};

#endif