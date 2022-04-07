#include "parseConfig.hpp"

parseConfig::parseConfig(void) : _lsn(0), _idx(0), _rt(0), _hst(0), _srv(0),_cgx(0),_cgb(0), _err(0),_mtd(0), _cms(0), _ab(0), _abf(0), _rdr(0), _up(0)
{ return; }

parseConfig::~parseConfig(void) 
{ return; }

parseConfig::parseConfig(const parseConfig& cpy) : _lsn(cpy._lsn), _idx(cpy._idx),
_rt(cpy._rt), _hst(cpy._hst), _srv(cpy._srv),_cgx(cpy._cgx),_cgb(cpy._cgb), _err(cpy._err),_mtd(cpy._mtd), _cms(cpy._cms), _ab(cpy._ab),
_abf(cpy._abf), _rdr(cpy._rdr), _up(cpy._up)
{}

parseConfig &parseConfig::operator=(const parseConfig& other)
{	
	_lsn = other._lsn;
	_idx = other._idx;
	_rt  = other._rt;
	_hst = other._hst;
	_srv = other._srv;
	_cgx = other._cgx;
	_cgb = other._cgb;
	_err = other._err;
	_mtd = other._mtd;
	_cms = other._cms;
	_ab  = other._ab;
	_abf = other._abf;
	_rdr = other._rdr;
	_up = other._up;
	return *this; }

/******_____PARSEING COMMON FUNCTION______******/

void	parseConfig::commonParsingValues(std::string &value)
{
	if (value[value.size() -1] != ';')
		throw OurException("ERROR: Missing ';' at the end of the line");
	value = value.substr(0, value.size() -1);
//	std::cout << "VALUE IN COMMON FT IS [" << value << "]\n";
}

/******_____PARSE && SET && GET && VALUE FROM SERVER BLOCK______******/

void	parseConfig::parseAndSetUploadServer(std::string &value, serverBlock &server)
{
	int i = 0;

	commonParsingValues(value);
	while (value[i])
	{
		if (value[i] == ' ' && value[i + 1] && !isspace(value[i + 1]))
			throw OurException("ERROR: server block :upload server one path expected");
		i++;
	}
    std::string::iterator end_pos = std::remove(value.begin(), value.end(), ' ');
	value.erase(end_pos, value.end());
	server.setUpload_s(value);
	std::cout << "UPLOAD SERVER = [" << server.getUpload_s() << "]\n";

}

void	parseConfig::parseAndSetPort(std::string &value, serverBlock &server)
{
	int val;
	commonParsingValues(value);
	for (int i = 0; value[i]; i++)
		if (!isdigit(value[i]))
			throw OurException("ERROR: PORT: only digit value expected");

	server.setPortStr(value);
	val = atoi(value.c_str());
	if (val < 0 || val > 65535)
		throw OurException("ERROR: PORT: range [0 - 65535] expected");
	server.setPort(val);
	server.setAI_s(0);
	//std::cout << "PORT STR value in serverblock is [" << server.getPortStr() << "]\n"; 
	//std::cout << "PORT value in serverblock is [" << server.getAI_s() << "]\n"; 
	//std::cout << "PORT value in serverblock is [" << server.getPort() << "]\n"; 
}

void	parseConfig::parseAndSetIndex(std::string &value, serverBlock &server)
{
	std::vector <std::string> 	values;
	std::string					valueToadd;
	std::string					newVal;
	int i = 0;
	commonParsingValues(value);
	
	for (int i = 0; value[i]; i++)
		if (value[i] == ',' && value[i] && value[i + 1]  != ' ')
			throw OurException("ERROR: INDEX: invalid char after ','"); //maybe not necessary
	while(value[i])
	{
		if (value[i] != ',' && value[i + 1] == ' ')
			throw OurException("ERROR: INDEX: invalid char after ','");
		while (value[i] == ' ')
			i++;
		newVal += value[i];
		i++;
	}
	i = 0;
	while (newVal[i])
	{

		if (newVal[i] == ',')
		{
			if (!isalpha(newVal[i + 1]))
				throw OurException("ERROR: INDEX: invalid char after ','");
			values.push_back(valueToadd);
			valueToadd.clear();
			i++;
		}
		valueToadd += newVal[i];
		i++;
	}
	values.push_back(valueToadd);
	server.setIndex(values);
	//tools::printVector(server.getIndex());
}

void	parseConfig::parseAndSetServerName(std::string &value, serverBlock &server)
{
	commonParsingValues(value);
	for (int i = 0; value[i]; i++)
		if (isspace(value[i]))
			throw OurException("ERROR: SERVER NAME: [string] expected");
	server.setName(value);
	//std::cout << "NAME value in serverblock is [" << server.getName() << "]\n"; 
}

//MUST BE nb.nb.nb.nb 
//with nb >= 0  && nb <= 255
void	parseConfig::parseAndSetHost(std::string &value, serverBlock &server)
{
	std::vector<std::string> nb;
	std::string value_toadd;
	int i = 0;
	
	commonParsingValues(value);
	if (value == "localhost")
	{
		server.setHostStr(value);
		unsigned int val = 	tools::strToIp(value);
		server.setHost(val);
		return ;
	}
	for (int i = 0; value[i]; i++)
	{
		if (isspace(value[i]))
			throw OurException("ERROR: HOST: [0.0.0.0] format expected");
	}
	while (value[i])
	{
		if (value[i] == '.' && value[i + 1])
		{
			nb.push_back(value_toadd);
			value_toadd.clear();
			i++;
		}
		value_toadd += value[i];
		i++;
	}
	nb.push_back(value_toadd);
	
	int count = 0;
	for (IT it = nb.begin(); it != nb.end(); it++, count++)
	{
		if ((*it).size() > 3)
			throw OurException("ERROR: HOST:[0.0.0.0] format expected | range [0-255]");
		if (atoi((*it).c_str()) < 0 || atoi((*it).c_str()) > 255)
			throw OurException("ERROR: HOST:[0.0.0.0] format expected | range [0-255]");
		for (int i = 0; (*it)[i]; i++)
		{
			if (!isdigit((*it)[i]))
			throw OurException("ERROR: HOST:[0.0.0.0] format expected | range [0-255]");
		}
	}
	if (count != 4)
			throw OurException("ERROR: HOST:[0.0.0.0] format expected");
	server.setHostStr(value);
	//std::cout << "HOST STR value in serverblock is [" << server.getHostStr() << "]\n"; 

	//CCLAUDE FT to convert str in IP 
	unsigned int val = 	tools::strToIp(value);
	server.setHost(val);
	//std::cout << "HOST value in serverblock is [" << server.getHost() << "]\n"; 
}

void	parseConfig::parseAndServerRoot(std::string &value, serverBlock &server)
{
	int i = 0;

	commonParsingValues(value);
	while (value[i])
	{
		if (value[i] == ' ' && value[i + 1] && !isspace(value[i + 1]))
			throw OurException("ERROR: server block :root server one path expected");
		i++;
	}
    std::string::iterator end_pos = std::remove(value.begin(), value.end(), ' ');
	value.erase(end_pos, value.end());
	server.setRootServer(value);
	//std::cout << "ROOT SERVER = [" << server.getRootServer() << "]\n";
}

void	parseConfig::parseAndSetCgiExt(std::string &value, serverBlock &server)
{
	commonParsingValues(value);
	size_t i = 0;
    std::string extension;
	while (value[i])
	{
		if (value[i] == ' ' && value[i + 1] && !isspace(value[i + 1]))
			throw OurException("ERROR: server block : cgi_extension, one argument expected");
		i++;
	}
	i = 0;
    for (; i < value.size() && !isspace(value[i]); i++)
	{
        extension += value[i];
	}
	server.setCgiExt(extension);
	//std::cout << "SERVER CGI_EXT = [" << server.getCgiExt() << "]\n";
}

void	parseConfig::parseAndSetCgiBin(std::string &value, serverBlock &server)
{
	int i = 0;

	commonParsingValues(value);
	while (value[i])
	{
		if (value[i] == ' ' && value[i + 1] && !isspace(value[i + 1]))
			throw OurException("ERROR: server block : cgi_bin one path expected");
		i++;
	}
    std::string::iterator end_pos = std::remove(value.begin(), value.end(), ' ');
	value.erase(end_pos, value.end());
	server.setCgiBin(value);
	//std::cout << "CGI_BIN SERVER = [" << server.getCgiBin() << "]\n";(value);
}

void	parseConfig::parseAndSetError(std::string &value, serverBlock &server)
{
	int i = 0;

	commonParsingValues(value);
	while (value[i])
	{
		if (value[i] == ' ' && value[i + 1] && !isspace(value[i + 1]))
			throw OurException("ERROR: server block : error one path expected");
		i++;
	}
    std::string::iterator end_pos = std::remove(value.begin(), value.end(), ' ');
	value.erase(end_pos, value.end());
	server.setError(value);
	//std::cout << "ERROR = [" << server.getError() << "]\n";
}

void	parseConfig::parseAndSetRedirServer(std::string &value, serverBlock &server)
{
	int i = 0;

	commonParsingValues(value);
	while (value[i])
	{
		if (value[i] == ' ' && value[i + 1] && !isspace(value[i + 1]))
			throw OurException("ERROR: location block : redirection one path expected");
		i++;
	}
    std::string::iterator end_pos = std::remove(value.begin(), value.end(), ' ');
	value.erase(end_pos, value.end());
	server.setRedir_s(value);
	//std::cout << "SERVER REDIRECTION = [" << server.getRedir_s() << "]\n";
}

void	parseConfig::parseAndSetAuthUsrServer(std::string &value, serverBlock &server)
{
	int i = 0;

	commonParsingValues(value);
	while (value[i])
	{
		if (value[i] == ' ' && value[i + 1] && !isspace(value[i + 1]))
			throw OurException("ERROR: location block : Auth User File one path expected");
		i++;
	}
    std::string::iterator end_pos = std::remove(value.begin(), value.end(), ' ');
	value.erase(end_pos, value.end());
	server.setAuthUsrFile_s(value);
	//std::cout << "SERVER AUTH_USER_FILE = [" << server.getAuthUsrFile_s() << "]\n";
}

void	parseConfig::parseAndSetAuthServer(std::string &value, serverBlock &server)
{
	commonParsingValues(value);
	if (value == "on")
		server.setAuthBasic_s(true);
	else if (value == "off")
		server.setAuthBasic_s(false);
	else
		throw OurException("ERROR : location block : Auth Basic  'on' or 'off' expected");
	//std::cout << "AUTH BASIC value in Server block is [" << server.getAuthBasic_s() << "]\n";
}

void	parseConfig::parseAndSetBodyServer(std::string &value, serverBlock &server)
{
	int i = 0;

	server.setBodySet_s(true);
	commonParsingValues(value);
	while (value[i])
	{
		if (value[i] == ' ' && value[i + 1] && !isspace(value[i + 1]))
			throw OurException("ERROR: location block: body size, only one argument expected");
		if (value[i] != ' ' && !isdigit(value[i]))
			throw OurException("ERROR: location block: body size, only digits expected");
		
		i++;
	}
	int val = ::atoi(value.c_str());
	server.setBody_s(val);
	//std::cout << "BODY SIZE SERVER = [" << server.getBody_s() << "]\n"; 
}

void	parseConfig::parseAndSetMethodsServer(std::string &value, serverBlock &server)
{
	int									i = 0;
	std::vector<std::string> 			methods;
	std::string							valueToAdd;
	std::string							newVal;
	std::vector<std::string>::iterator	it;
	
	commonParsingValues(value);
	//remove spaces
	while (value[i])
	{
		if (value[i] == ' ')
			i++;
		newVal += value[i];
		i++;
	}
	// get the values and push it in vector methods
	i = 0;
	while (newVal[i])
	{
		if (newVal[i] == ',')
		{
			methods.push_back(valueToAdd);
			valueToAdd.clear();
			i++;
		}
		valueToAdd += newVal[i];
		i++;
	}
	methods.push_back(valueToAdd);

	//check the values and if there is no double
	int p = 0, g = 0, d = 0;
	for (it = methods.begin(); it != methods.end(); it++)
	{
		if ((*it) == "POST")
			p++;
		else if ((*it) == "GET")
			g++;
		else if ((*it) == "DELETE")
			d++;
		else
			throw OurException("ERROR: location block: methods GET POST or DELETE expected");
	}
	if (p > 1 || g > 1 || d > 1)
		throw OurException("ERROR : location block: method too many arguments");
	server.setMethods_s(methods);
	//tools::printVector(server.getMethods_s());
}

std::string	parseConfig::parseLocationPath(std::string const& line)
{
	if (line == "location")
		throw OurException("ERROR: LOCATION PATH:argument for path expected");
	std::string path = line.substr(9, line.length() - 9);
	for (size_t i = 0; i < path.length(); i++)
		if (path[i] == ' ' && path[i + 1] && !isspace(path[i + 1]))
			throw OurException("ERROR: LOCATION PATH:only one path expected");
	//std::cout << "path is : " << path << "\n";
	return (path);
}

/******_____ IN SERVER BLOCK______*******/

bool	parseConfig::isServerBlock(std::string line)
{
	size_t i = 0;

	size_t pos = line.find("server");
	size_t posWrong = line.find("server_");
	size_t posWrong2 = line.find("server.");
	if (posWrong != std::string::npos || posWrong2 != std::string::npos)
		return (false);
	if (pos != std::string::npos)
	{	
		while (i < pos)
		{
			if (!isspace(line[i]))
				throw OurException("ERROR : ServerBlock : server line invalid");
			i++;
		}
		i += 6;
		while(i < line.length())
		{
			if (!isspace(line[i]))
				throw OurException("ERROR : ServerBlock : server line invalid");
			i++;
		}
	}
	if (i == line.length() && pos != std::string::npos)
		return (true);
	else
		return (false);
}

void 	parseConfig::getValueServerBlock(std::string const& attribut, std::string& value, serverBlock &server)
{
	if (attribut.compare("listen ") == 0)
		parseAndSetPort(value, server);
	else if (attribut.compare("index ") == 0)
		parseAndSetIndex(value, server);
	else if (attribut.compare("host ") == 0)
		parseAndSetHost(value, server);
	else if (attribut.compare("server_name ") == 0)
		parseAndSetServerName(value, server);
	else if (attribut.compare("cgi_extension ") == 0)
		parseAndSetCgiExt(value, server);
	else if (attribut.compare("cgi_bin ") == 0)
		parseAndSetCgiBin(value, server);
	else if (attribut.compare("error ") == 0)
		parseAndSetError(value, server);
	else if (attribut.compare("root ") == 0)
		parseAndServerRoot(value, server);
	else if (attribut.compare("methods ") == 0)
		parseAndSetMethodsServer(value, server);
	else if (attribut.compare("methods ") == 0)
		parseAndSetMethodsServer(value, server);
	else if (attribut.compare("client_max_body_size ") == 0)
	 	parseAndSetBodyServer(value, server);
	else if (attribut.compare("auth_basic ") == 0)
	 	parseAndSetAuthServer(value, server);
	else if (attribut.compare("auth_basic_user_file ") == 0)
		parseAndSetAuthUsrServer(value, server);
	else if (attribut.compare("redirection ") == 0)
		parseAndSetRedirServer(value, server);
	else if (attribut.compare("upload ") == 0)
		parseAndSetUploadServer(value, server);
}

void		parseConfig::countEachAtt(std::string const& attribut)
{
	if (attribut.compare("listen ") == 0)
		_lsn++;
	else if (attribut.compare("index ") == 0)
		_idx++;
	else if (attribut.compare("host ") == 0)
		_hst++;
	else if (attribut.compare("server_name ") == 0)
		_srv++;
	else if (attribut.compare("cgi_extension ") == 0)
		_cgx++;
	else if (attribut.compare("cgi_bin ") == 0)
		_cgb++;
	else if (attribut.compare("error ") == 0)
		_err++;
	else if (attribut.compare("root ") == 0)
		_rt++;
	else if (attribut.compare("methods ") == 0)
		_mtd++;
	else if (attribut.compare("client_max_body_size ") == 0)
	 	_cms++;
	else if (attribut.compare("auth_basic ") == 0)
	 	_ab++;
	else if (attribut.compare("auth_basic_user_file ") == 0)
		_abf++;
	else if (attribut.compare("redirection ") == 0)
		_rdr++;
	else if (attribut.compare("upload ") == 0)
		_up++;
}

int		parseConfig::getAttributName(std::string const &line, std::string &attribut, std::string& value, serverBlock &server)
{
	int 						j = 0;
	size_t 						pos;
	IT 							it;
	std::string					dir;
	std::vector<std::string> 	atts;
	atts.push_back("listen ");
	atts.push_back("index ");
	atts.push_back("root ");
	atts.push_back("host ");
	atts.push_back("server_name ");
	atts.push_back("cgi_extension ");
	atts.push_back("cgi_bin ");
	atts.push_back("error ");
	atts.push_back("methods ");
	atts.push_back("client_max_body_size ");
	atts.push_back("auth_basic ");
	atts.push_back("auth_basic_user_file ");
	atts.push_back("redirection ");
	atts.push_back("upload ");

	while(line[j] && line[j] != ' ')
	{
		dir+= line[j];
		j++;
	}
	if (!line.empty() && !tools::isValidDirective(dir))
		throw OurException("ERROR: server block, invalid directive");
	
	for (it = atts.begin(); it != atts.end(); it++)
	{
		pos = line.find((*it));
		if (pos != std::string::npos)
		{
			attribut = (*it).substr(0, (*it).length());
			countEachAtt(attribut);
			if (_lsn > 1 || _idx > 1 || _rt > 1 || _hst > 1 || _srv > 1 || _cgx > 1 || _cgb > 1 
			|| _err > 1 || _mtd > 1 || _cms > 1 || _ab > 1 || _abf > 1 || _rdr > 1 || _up > 1)
					throw OurException("Directive in server block not allowed here");
			value = (line).substr(attribut.length(), line.length() - attribut.length());
			getValueServerBlock(attribut, value, server);
			return ((*it).length());
		}	
	}
	return (0);
}

void	parseConfig::setServerConfig(std::string const &line, serverBlock &server)
{
//	int posEnd;
	std::string attributName;
	std::string value;
//	posEnd =
	getAttributName(line, attributName, value, server);
//	std::cout << "in setServerConfig " << attributName << "\n";
}

/******_____ IN LOCATION BLOCK______*******/

bool	parseConfig::isLocationBlock(std::string const &line)
{
	int i = 0;
	if (!line.compare(i, 8, "location"))
		i += 8;
	else
		return (false);
	if (line.size() > 8 && !isspace(line[8]))
		throw OurException("ERROR: LOCATION BLOCK: space between lacation and location path expected");
	return (true);
}	

/******_ GET && SET ATTRIBUTES LOCATION BLOCK___******/

void	parseConfig::parseAndSetAILoc(std::string &value, serverLocation &location)
{
	commonParsingValues(value);
	if (value == "on")
		location.setAI(true);
	else if (value == "off")
		location.setAI(false);
	else
		throw OurException("ERROR : location block : autoindex  'on' or 'off' expected");
	//std::cout << "AUTOINDEX value in locationblock is [" << location.getAI() << "]\n"; 
}

void	parseConfig::parseAndSetMethodsLoc(std::string &value, serverLocation &location)
{
	//methods available : GET POST DELETE
	// NEED the 3 methods to work ??
	int									i = 0;
	std::vector<std::string> 			methods;
	std::string							valueToAdd;
	std::string							newVal;
	std::vector<std::string>::iterator	it;
	
	commonParsingValues(value);
	//remove spaces
	while (value[i])
	{
		if (value[i] == ' ')
			i++;
		newVal += value[i];
		i++;
	}
	// get the values and push it in vector methods
	i = 0;
	while (newVal[i])
	{
		if (newVal[i] == ',')
		{
			methods.push_back(valueToAdd);
			valueToAdd.clear();
			i++;
		}
		valueToAdd += newVal[i];
		i++;
	}
	methods.push_back(valueToAdd);

	//check the values and if there is no double
	int p = 0, g = 0, d = 0;
	for (it = methods.begin(); it != methods.end(); it++)
	{
		if ((*it) == "POST")
			p++;
		else if ((*it) == "GET")
			g++;
		else if ((*it) == "DELETE")
			d++;
		else
			throw OurException("ERROR: location block: methods GET POST or DELETE expected");
	}
	if (p > 1 || g > 1 || d > 1)
		throw OurException("ERROR : location block: method too many arguments");
	location.setMethods(methods);
	//tools::printVector(location.getMethods());
}

void	parseConfig::parseAndSetRootLoc(std::string &value, serverLocation &location)
{
	int i = 0;

	commonParsingValues(value);
	while (value[i])
	{
		if (value[i] == ' ' && value[i + 1] && !isspace(value[i + 1]))
			throw OurException("ERROR: location block : root one path expected");
		i++;
	}
    std::string::iterator end_pos = std::remove(value.begin(), value.end(), ' ');
	value.erase(end_pos, value.end());
	location.setRootLoc(value);
	//std::cout << "LOCATION PATH = [" << location.getRootLoc() << "]\n";
}

void	parseConfig::parseAndSetCgiExtLoc(std::string &value, serverLocation &location)
{
	commonParsingValues(value);
	size_t i = 0;
    std::string extension;
	while (value[i])
	{
		if (value[i] == ' ' && value[i + 1] && !isspace(value[i + 1]))
			throw OurException("ERROR: location block : cgi_extension, one argument expected");
		i++;
	}
	i = 0;
    for (; i < value.size() && !isspace(value[i]); i++)
	{
		extension += value[i];
	}
	location.setCgiExt(extension);
	//std::cout << "LOCATION CGI_EXT = [" << location.getCgiExt() << "]\n";
}

void	parseConfig::parseAndSetCgiBinLoc(std::string &value, serverLocation &location)
{
	int i = 0;

	commonParsingValues(value);
	while (value[i])
	{
		if (value[i] == ' ' && value[i + 1] && !isspace(value[i + 1]))
			throw OurException("ERROR: location block : CGI_BIN one path expected");
		i++;
	}
    std::string::iterator end_pos = std::remove(value.begin(), value.end(), ' ');
	value.erase(end_pos, value.end());
	location.setCgiBin(value);
//	std::cout << "LOCATION CGI_BIN = [" << location.getCgiBin() << "]\n";
}

void	parseConfig::parseAndSetUploadLoc(std::string &value, serverLocation &location)
{
	int i = 0;

	commonParsingValues(value);
	while (value[i])
	{
		if (value[i] == ' ' && value[i + 1] && !isspace(value[i + 1]))
			throw OurException("ERROR: location block : redirection one path expected");
		i++;
	}
    std::string::iterator end_pos = std::remove(value.begin(), value.end(), ' ');
	value.erase(end_pos, value.end());
	location.setUploadLoc(value);
	//std::cout << "LOCATION UPLOAD = [" << location.getUploadLoc() << "]\n";
}

void	parseConfig::parseAndSetRedirLoc(std::string &value, serverLocation &location)
{
	int i = 0;

	commonParsingValues(value);
	while (value[i])
	{
		if (value[i] == ' ' && value[i + 1] && !isspace(value[i + 1]))
			throw OurException("ERROR: location block : redirection one path expected");
		i++;
	}
    std::string::iterator end_pos = std::remove(value.begin(), value.end(), ' ');
	value.erase(end_pos, value.end());
	location.setRedir(value);
	//std::cout << "LOCATION REDIRECTION = [" << location.getRedir() << "]\n";
}

void	parseConfig::parseAndSetIndexLoc(std::string &value, serverLocation &location)
{
	std::vector <std::string> 	values;
	std::string					valueToadd;
	int 						i = 0;

	commonParsingValues(value);
	while (value[i])
	{
		if (value[i] == ' ')
		{
			if (!isalpha(value[i + 1]))
				throw OurException("ERROR: INDEX: invalid char after ','");
			values.push_back(valueToadd);
			valueToadd.clear();
			i++;
		}
		valueToadd += value[i];
		i++;
	}
	values.push_back(valueToadd);
	location.setIndex(values);
	//tools::printVector(location.getIndex());
}

void	parseConfig::parseAndSetAuthLoc(std::string &value, serverLocation &location)
{
	commonParsingValues(value);
	if (value == "on")
		location.setAuthBasic(true);
	else if (value == "off")
		location.setAuthBasic(false);
	else
		throw OurException("ERROR : location block : Auth Basic  'on' or 'off' expected");
//	std::cout << "AUTH BASIC value in locationblock is [" << location.getAuthBasic() << "]\n";
}

void	parseConfig::parseAndSetAuthUsrLoc(std::string &value, serverLocation &location)
{
	int i = 0;

	commonParsingValues(value);
	while (value[i])
	{
		if (value[i] == ' ' && value[i + 1] && !isspace(value[i + 1]))
			throw OurException("ERROR: location block : Auth User File one path expected");
		i++;
	}
    std::string::iterator end_pos = std::remove(value.begin(), value.end(), ' ');
	value.erase(end_pos, value.end());
	location.setAuthUsrFile(value);
	//std::cout << "LOCATION AUTH_USER_FILE = [" << location.getAuthUsrFile() << "]\n";
}

void	parseConfig::parseAndSetBodyLoc(std::string &value, serverLocation &location)
{
	int i = 0;

	location.setBodySetLoc(true);
	commonParsingValues(value);
	while (value[i])
	{
		if (value[i] == ' ' && value[i + 1] && !isspace(value[i + 1]))
			throw OurException("ERROR: location block: body size, only one argument expected");
		if (value[i] != ' ' && !isdigit(value[i]))
			throw OurException("ERROR: location block: body size, only digits expected");
		
		i++;
	}
	int val = ::atoi(value.c_str());
	location.setBody(val);
	//std::cout << "BODY SIZE = [" << location.getBody() << "]\n"; 
}

void 	parseConfig::getValuesLocationBlock(std::string const& attribut, std::string& value, serverLocation &location)
{
	if (attribut.compare("autoindex ") == 0)
		parseAndSetAILoc(value, location);
	else if (attribut.compare("methods ") == 0)
		parseAndSetMethodsLoc(value, location);
	else if (attribut.compare("root ") == 0)
		parseAndSetRootLoc(value, location);
	else if (attribut.compare("cgi_extension ") == 0)
		parseAndSetCgiExtLoc(value, location);
	else if (attribut.compare("cgi_bin ") == 0)
		parseAndSetCgiBinLoc(value, location);
	else if (attribut.compare("redirection ") == 0)
		parseAndSetRedirLoc(value, location);
	else if (attribut.compare("index ") == 0)
		parseAndSetIndexLoc(value, location);
	else if (attribut.compare("auth_basic ") == 0)
		parseAndSetAuthLoc(value, location);
	else if (attribut.compare("auth_basic_user_file ") == 0)
		parseAndSetAuthUsrLoc(value, location);
	else if (attribut.compare("client_max_body_size ") == 0)
		parseAndSetBodyLoc(value, location);
	else if (attribut.compare("upload ") == 0)
		parseAndSetUploadLoc(value, location);
}

/******_________________________________________******/
/******_________ PASRING LOCATION BLOCK_________******/

int		parseConfig:: getAttsLocation(std::string const &line, std::string &attribut, std::string& value, serverLocation &location)
{
	int 						j = 0;
	size_t 						pos;
	IT 							it;
	std::string 				dir;
	std::vector<std::string> 	atts;
	atts.push_back("autoindex ");
	atts.push_back("methods ");
	atts.push_back("root ");
	atts.push_back("cgi_extension ");
	atts.push_back("cgi_bin ");
	atts.push_back("index ");
	atts.push_back("client_max_body_size ");
	atts.push_back("auth_basic ");
	atts.push_back("auth_basic_user_file ");
	atts.push_back("redirection ");
	atts.push_back("upload ");
	
	while(line[j] && line[j] != ' ')
	{
		dir+= line[j];
		j++;
	}
	if (!line.empty() && !tools::isValidDirectiveLocation(dir))
			throw OurException("ERROR: location block, invalid directive");
 
	for (it = atts.begin(); it != atts.end(); it++)
	{
		pos = line.find((*it));
		if (pos != std::string::npos)
		{	
			attribut = (*it).substr(0, (*it).length());
			value = (line).substr(attribut.length(), line.length() - attribut.length());
			// std::cout << " Attribut = " << "'"<<  attribut << "'" << "\n";
			// std::cout << " Value = " << "'"<<  value << "'" << "\n";
			getValuesLocationBlock(attribut, value, location);
			return ((*it).length());
		}
	}
	// std::cout << "LINE = " << line << "\n";
	// if (it == atts.end())
	// 	throw OurException("Directive in server block not allowed here");
	return (0);
}

void	parseConfig::setLocationConfig(std::string &line, serverLocation &location)
{
//int posEnd;
	std::string attributName;
	std::string value;
//	posEnd =
			getAttsLocation(line, attributName, value, location);
}

void	parseConfig::setLocationBlock(IT &start, IT &end, serverBlock &server, std::string path)
{
	serverLocation location;

	//std::string locPath = parseLocationPath(path);
	for (; start != end;)
	{
		if (isServerBlock(*start) || isLocationBlock(*start))
			throw OurException("ERROR: LOCATION BLOCK: no location block or server block allowed");
		setLocationConfig((*start), location);
		if (start != end)
            start++;
	}
	
    for (std::vector<serverLocation>::const_iterator it = server._locations.begin(); it != server._locations.end(); it++)
        if (it->getLocationPath() == path)
            throw OurException("Directive: 'location' : duplicate symbols");

        
    location.setLocationPath(path);

    server._locations.push_back(location);
	//tools::printLocationBlock(server._locations);
}

/*  Push the  file line by line in content
	Splitting the file per ';' '{' '}'
*/
std::vector<std::string> parseConfig::parseLine(std::string line, std::vector<std::string> content)
{
	int i;
	std::string tmp;

	for (i = 0; isspace(line[i]); i++)
		{ }
	for (; line[i]; i++)
	{
		if (line[i] == '#')
			return (content);
		if (line[i] == '{')
		{
		   // std::cout << "line in if " << line << std::endl;
			tmp = line.substr(0, i);
			//checker if need to check spaces and empty()
			//if (!tools::isSpaces(tmp) || tmp.empty())
				content.push_back(tmp);
			content.push_back("{");
			if (!line[i + 1])
				return (content);
			line = &line[i + 1];
			i = -1;
		}
		else if (line[i] == '}')
		{
			tmp = line.substr(0, i);
		//	if (!tools::isSpaces(tmp) || tmp.empty())
				content.push_back(tmp);
			content.push_back("}");
			if (!line[i + 1])
				return (content);
			line = &line[i + 1];
			i = -1;
		}
		else if (line[i] == ';')
		{
			tmp = line.substr(0, i + 1);
			content.push_back(tmp);
			if (!line[i + 1])
				return (content);
			line = &line[i + 1];
			i = -1;
		}
		else if (!line[i + 1] && (!tools::isSpaces(line)) && line[0] != '#')
		{
			content.push_back(line);
			return (content);
		}
	}
   
	return (content);
}

void parseConfig::setOneServer(IT &start, IT &end, std::vector<serverBlock> &servers)
{
	serverBlock server;
	_lsn = 0, _idx = 0, _rt = 0, _hst = 0, _srv = 0, _cgx = 0, _cgb = 0 , _err  = 0;
	_mtd = 0, _cms = 0, _ab = 0, _abf = 0, _rdr = 0, _up = 0;
	 
	for(int bracket = 0; start != end;)
	{
		if (isServerBlock(*start))
			throw OurException("Server block not allowed here");
		if (isLocationBlock(*start))
		{
			std::string pathLocation = parseLocationPath(*start);
			start++; //should be bracket
			if ((*start).find('{') != std::string::npos)
				start++, bracket++;
			else
				throw OurException("ERROR: LOCATION BLOCK: openning bracket expected");
			IT beginBlock = start;
			while (start != end && bracket != 0)
			{
				if ((*start).find('}') != std::string::npos)
					bracket--;
				start++;
			}
			if (bracket == 0)
				setLocationBlock(beginBlock, --start, server, pathLocation);
			else
				throw OurException("ERROR: LOCATION BLOCK: closing bracket expected");
		}
		else
		{
			setServerConfig((*start), server);
			
		}
		// std::cout << "LINE in SET ON : " << (*start) << "\n";
		start++;
	}
	// once we get all the config from one server we push back the server 
	// with updated info in the vector<serverBLock> servers
	if (_lsn == 0 || _hst == 0)
		throw OurException("Directive in server block missing: port and host");
	servers.push_back(server);
}

/*
	Find each server block and check if it's valid
	If yes we set it
	If no throw exception
*/
void	parseConfig::setServers(std::vector<std::string> &content, std::vector<serverBlock> &servers)
{
	int bracket = 0;
	IT it;
	for (it = content.begin(); it != content.end(); it++)
	{
		if (isServerBlock(*it))
		{	
			it++;
			if ((*it).find('{') != std::string::npos)
				bracket++, it++;
			else
				throw OurException("ERROR: server block: no opening bracket");
			IT start = it;
			while (it != content.end() && bracket != 0)
			{
				if ((*it).find('{') != std::string::npos)
					bracket++;
				if ((*it).find('}') != std::string::npos)
					bracket--;
				it++;
			}
			/* 
				Each time we found a server block we set up it
				&& get back the infos in the block
				--it: to be at the end of the server block w/ the last '}'
			*/
			if (bracket == 0)
			{
				setOneServer(start, --it, servers);
			
			}
			else
				throw OurException("ERROR: server block : missing a bracket");
		}
		//std::cout << "IT -- = " << (*it) << "\n";

		else
			throw OurException("ERROR: server block [" + *it + "] : unexpected line");
		// if (it != content.end())
		// {
		// 	//std::cout << "is it nessecary ??\n";
        //     it++;
		// }
	}
}

void parseConfig::parsing(std::string path, std::vector<serverBlock> &servers) 
{
	std::string line;
	std::vector<std::string> content;
	std::ifstream fd(path.c_str());

	if (!fd.is_open() || !fd.good())
	{
		std::ostringstream oss;
		oss << "ERROR: can't open file" << path;
		throw   OurException(oss.str().c_str());
	}
	else
	{
		while (std::getline(fd, line))
			content = parseLine(line, content);
		if (content.empty())
			throw  OurException("ERROR: empty file");
	}

	//delete all empty lines
	for (IT it = content.begin(); it != content.end(); it++)
	{
		if ((*it).empty())
 			content.erase(it);
	}
	//remove duplicate spaces
	for (IT it = content.begin(); it != content.end(); it++)
	{
		(*it) =  tools::removeDuplicateSpaces(*it);
	}
		//tools::printVector(content);
	/*
		setup the servers by finding each blocks which is a server block
		then pushback each in std::vector<serverBlock>
	*/
	setServers(content, servers);
}
