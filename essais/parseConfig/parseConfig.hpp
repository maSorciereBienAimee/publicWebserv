#ifndef _PARSECONFIG_HPP_
#define _PARSECONFIG_HPP_

#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include "../tools/tools.hpp"
#include "serverBlock.hpp"

#define IT std::vector<std::string>::iterator

class parseConfig  {

	public:

	parseConfig(void);
	~parseConfig(void);
	parseConfig(const parseConfig& cpy);
	parseConfig &operator=(const parseConfig& other);
	
	void                        parsing(std::string path, std::vector<serverBlock> &servers);
	std::vector<std::string>    parseLine(std::string line, std::vector<std::string> content);
	void						setServers(std::vector<std::string> &content, std::vector<serverBlock> &servers);
	bool						isServerBlock(std::string line);
	void 						setOneServer(IT &start, IT &end, std::vector<serverBlock> &servers);

	
	protected:
   

};


#endif