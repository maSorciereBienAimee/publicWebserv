#ifndef _PARSECONFIG_HPP_
#define _PARSECONFIG_HPP_

#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "../tools/tools.hpp"
#include "serverBlock.hpp"
#include "serverLocation.hpp"

#define IT std::vector<std::string>::iterator

class serverBlock;
class serverLocation;

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
	std::string	 				parsingPath(std::string str);

	
	/******_____SERVER BLOCK PARSING______******/
	void						setServerConfig(std::string const &line, serverBlock &server);
	int							getAttributName(std::string const &line, std::string  &attribut, std::string& value, serverBlock &server);
	void 						getValueServerBlock(int pos, std::string const& attribut, std::string& value, serverBlock &server);
	bool						isServerBlock(std::string line);
	
	/******_____LOCATION BLOCK PARSING______******/
	bool 						isLocationBlock(std::string const &line);
	std::string					parseLocationPath(std::string const& path);
	void						setLocationBlock(IT &start, IT &end, serverBlock &server, std::string path);
	void						setLocationConfig(std::string &line, serverLocation &location, serverBlock &server);
	int							getAttsLocation(std::string const &line, std::string &attribut, std::string& value, serverBlock &server, serverLocation &location);
	void 						getValuesLocationBlock(int pos, std::string const& attribut, std::string& value, serverBlock &server, serverLocation &location);

	private:

	/******_____ FUNCTION PARSE && SET && GET && VALUE FROM SERVER BLOCK______******/
	void						commonParsingValues(std::string &value);
	void						parseAndSetPort(std::string& value, serverBlock &server);
	void						parseAndSetIndex(std::string &value, serverBlock &server);
	void						parseAndSetServerName(std::string &value, serverBlock &server);
	void						parseAndSetHost(std::string &value, serverBlock &server);
	void						parseAndSetCgiExt(std::string &value, serverBlock &server);
	void						parseAndSetCgiBin(std::string &value, serverBlock &server);
	void						parseAndSetError(std::string &value, serverBlock &server);
	void						parseAndServerRoot(std::string &value, serverBlock &server);
	void						parseAndSetMethodsServer(std::string &value, serverBlock &server);
	//void						parseAndSetAIServer(std::string &value, serverBlock &server);
	void						parseAndSetBodyServer(std::string &value, serverBlock &server);
	void						parseAndSetAuthServer(std::string &value, serverBlock &server);
	void						parseAndSetAuthUsrServer(std::string &value, serverBlock &server);
	void						parseAndSetRedirServer(std::string &value, serverBlock &server);


	/******_____ FUNCTION PARSE && SET && GET && VALUE FROM LOCATION BLOCK______******/
   
	void						parseAndSetAILoc(std::string &value, serverLocation &location);
	void						parseAndSetMethodsLoc(std::string &value, serverLocation &location);
	void						parseAndSetRootLoc(std::string &value, serverLocation &location);
	void						parseAndSetCgiExtLoc(std::string &value, serverLocation &location);
	void						parseAndSetCgiBinLoc(std::string &value, serverLocation &location);
	void						parseAndSetRedirLoc(std::string &value, serverLocation &location);
	void						parseAndSetIndexLoc(std::string &value, serverLocation &location);
	void						parseAndSetAuthLoc(std::string &value, serverLocation &location);
	void						parseAndSetAuthUsrLoc(std::string &value, serverLocation &location);
	void						parseAndSetBodyLoc(std::string &value, serverLocation &location);
};


#endif
