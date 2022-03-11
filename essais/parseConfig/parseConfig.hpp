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
	
	/******_____GLOBAL PARSING______******/
	void                        parsing(std::string path, std::vector<serverBlock> &servers);
	std::vector<std::string>    parseLine(std::string line, std::vector<std::string> content);
	void						setServers(std::vector<std::string> &content, std::vector<serverBlock> &servers);
	void 						setOneServer(IT &start, IT &end, std::vector<serverBlock> &servers);
	
	
	/******_____SERVER BLOCK PARSING______******/
	void						setServerConfig(std::string const &line, serverBlock &server);
	int							getAttributName(std::string const &line, std::string  &attribut, std::string& value, serverBlock &server);
	void 						getValueServerBlock(int pos, std::string const& attribut, std::string& value, serverBlock &server);
	bool						isServerBlock(std::string line);
	
	/******_____LOCATION BLOCK PARSING______******/
	bool 						isLocationBlock(std::string const &line);

	protected:
	
	/******_____ FUNCTION PARSE && SET && GET && VALUE FROM SERVER BLOCK______******/
	void						commonParsingValues(std::string &value);
	void						parseAndSetPort(std::string& value, serverBlock &server);
	void						parseAndSetIndex(std::string &value, serverBlock &server);
	void						parseAndSetServerName(std::string &value, serverBlock &server);
	void						parseAndSetHost(std::string &value, serverBlock &server);
	void						parseAndSetCgiExt(std::string &value, serverBlock &server);
	void						parseAndSetCgiBin(std::string &value, serverBlock &server);
	void						parseAndSetError(std::string &value, serverBlock &server);
   

};


#endif