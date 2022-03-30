#ifndef _TOOLS_HPP_
#define _TOOLS_HPP_

#include <iostream>
#include <string>
#include <exception>
#include <vector>
#include <map>
#include <sstream>
#include <limits>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include "../parseConfig/parseConfig.hpp"
#include "../parseConfig/serverLocation.hpp"
#include "../parseConfig/serverBlock.hpp"

class serverBlock;
class serverLocation;


namespace tools
{
	 //just for help
	void    							printServerBlock(serverBlock content);
	void           						 printVector(std::vector<std::string> content);
	void    							printLocationBlock(std::vector<serverLocation> content);
	bool           						 isSpaces(std::string str);
	std::string    						 removeDuplicateSpaces(std::string const &str);
	unsigned int   						 strToIp(std::string strIp);
	bool 								isValidDirectiveLocation(std::string const &dir);
	bool 								isValidDirective(std::string const &dir);
	std::map<std::string, std::string>	mime();
	std::string							getMimeType(std::string fileName);
	std::string							getExtension(std::string filename);
	int									isItCgi(std::string path, serverLocation info);
	serverLocation						whichLocation(std::string req, serverBlock block);
	serverLocation						searchLocation(std::string path, serverBlock block);
	int									searchInConfig(std::string str, std::vector<serverLocation> location, serverLocation *loc);
	std::vector<std::string>			getDirAI(std::string const& path);
	std::string							genreateAI(std::string const& host, std::string const& port, std::string const& path);
	std::string							getSimplePath(std::string r, std::string *p, serverBlock B);
	std::string							searchCorrectPath(std::string q, serverBlock B);

}


struct OurException : public std::exception
{
	std::string _str;

	OurException(std::string str): _str(str){}
	~OurException() {}
	const char * what() const throw() { return _str.c_str(); }
};

#endif
