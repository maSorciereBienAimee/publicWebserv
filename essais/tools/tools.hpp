#ifndef _TOOLS_HPP_
#define _TOOLS_HPP_

#include <iostream>
#include <string>
#include <exception>
#include <vector>
#include <sstream>
#include <limits>
#include "../parseConfig/parseConfig.hpp"
#include "../parseConfig/serverLocation.hpp"
#include "../parseConfig/serverBlock.hpp"




namespace tools
{
	 //just for help
	void            printVector(std::vector<std::string> content);
	void    		printLocationBlock(std::vector<serverLocation> content);
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