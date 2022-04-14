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

	private: 

		// directives server's block
		int _lsn;
		int _idx;
		int _rt;
		int _hst;
		int _srv;
		int _cgx;
		int _cgb;
		int _err;
		int _mtd;
		int _cms;
		int _ab;
		int _abf;
		int _rdr;
		int _up;

		//directives location's block
		int _lsnl;
		int _idxl;
		int _rtl;
		int _ail;
		int _cgxl;
		int _cgbl;
		int _mtdl;
		int _cmsl;
		int _abl;
		int _abfl;
		int _rdrl;
		int _upl;
	
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
	void 						getValueServerBlock(std::string const& attribut, std::string& value, serverBlock &server);
	bool						isServerBlock(std::string line);
	void						countEachAtt(std::string const& attribut);
	
	/******_____LOCATION BLOCK PARSING______******/
	bool 						isLocationBlock(std::string const &line);
	std::string					parseLocationPath(std::string const& path);
	void						setLocationBlock(IT &start, IT &end, serverBlock &server, std::string path);
	void						setLocationConfig(std::string &line, serverLocation &location);
	int							getAttsLocation(std::string const &line, std::string &attribut, std::string& value, serverLocation &location);
	void 						getValuesLocationBlock(std::string const& attribut, std::string& value, serverLocation &location);
	void						countEachAttLoc(std::string const& attribut);

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
	void						parseAndSetUploadServer(std::string &value, serverBlock &server);


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
	void						parseAndSetUploadLoc(std::string &value, serverLocation &location);
};


#endif
