#include "parseConfig.hpp"

parseConfig::parseConfig(void){ return; }

parseConfig::~parseConfig(void){ return; }

parseConfig::parseConfig(const parseConfig& cpy)
{}

parseConfig &parseConfig::operator=(const parseConfig& other){ return *this; }


/******_____PARSE && SET && GET && VALUE FROM SERVER BLOCK______******
/ * 			DON'T FORGET TO PUT VALUE IN CONST						*/

void	parseConfig::commonParsingValues(std::string &value)
{
	if (value[value.size() -1] != ';')
		throw OurExcetpion("ERROR: Missing ';' at the end of the line");
	value = value.substr(0, value.size() -1);
	//std::cout << "VALUE IN COMMON FT IS [" << value << "]\n";
}

void	parseConfig::parseAndSetPort(std::string &value, serverBlock &server)
{
	int val;
	commonParsingValues(value);
	for (int i = 0; value[i]; i++)
		if (!isdigit(value[i]))
			throw OurExcetpion("ERROR: PORT: only digit value expected");
	val = atoi(value.c_str());
	if (val < 0 || val > 65535)
		throw OurExcetpion("ERROR: PORT: range [0 - 65535] expected");
	server.setPort(val);
	//std::cout << "PORT value in serverblock is [" << server.getPort() << "]\n"; 
}

void	parseConfig::parseAndSetIndex(std::string &value, serverBlock &server)
{
	std::vector <std::string> 	values;
	size_t 						sep = 0;
	size_t						start = 0;
	std::string					valueToadd;
	std::string					newVal;
	int i = 0;
	commonParsingValues(value);
	
	for (int i = 0; value[i]; i++)
		if (value[i] == ',' && value[i] && value[i + 1]  != ' ')
			throw OurExcetpion("ERROR: INDEX: invalid char after ','"); //maybe not necessary
	while(value[i])
	{
		if (value[i] != ',' && value[i + 1] == ' ')
			throw OurExcetpion("ERROR: INDEX: invalid char after ','");
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
				throw OurExcetpion("ERROR: INDEX: invalid char after ','");
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
			throw OurExcetpion("ERROR: NAME: [string] expected");
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
		unsigned int val = 	tools::strToIp(value);
		server.setHost(val);
		return ;
	}
	for (int i = 0; value[i]; i++)
	{
		if (isspace(value[i]))
			throw OurExcetpion("ERROR: HOST: [0.0.0.0] format expected");
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
			throw OurExcetpion("ERROR: HOST:[0.0.0.0] format expected | range [0-255]");
		if (atoi((*it).c_str()) < 0 || atoi((*it).c_str()) > 255)
			throw OurExcetpion("ERROR: HOST:[0.0.0.0] format expected | range [0-255]");
		for (int i = 0; (*it)[i]; i++)
		{
			if (!isdigit((*it)[i]))
			throw OurExcetpion("ERROR: HOST:[0.0.0.0] format expected | range [0-255]");
		}
	}
	if (count != 4)
			throw OurExcetpion("ERROR: HOST:[0.0.0.0] format expected");
	//CCLAUDE FT to convert str in IP 
	unsigned int val = 	tools::strToIp(value);
	server.setHost(val);
	//std::cout << "HOST value in serverblock is [" << server.getHost() << "]\n"; 
}

void	parseConfig::parseAndSetCgiExt(std::string &value, serverBlock &server)
{
	commonParsingValues(value);
	
}

void	parseConfig::parseAndSetCgiBin(std::string &value, serverBlock &server)
{
	commonParsingValues(value);

}

void	parseConfig::parseAndSetError(std::string &value, serverBlock &server)
{
	commonParsingValues(value);
	
}

/******_____ IN SERVER BLOCK______*******/

/* check if server line is valid */
bool	parseConfig::isServerBlock(std::string line)
{
	int i = 0;

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
				throw OurExcetpion("ERROR : ServerBlock : server line invalid");
			i++;
		}
		i += 6;
		while(i < line.length())
		{
			if (!isspace(line[i]))
				throw OurExcetpion("ERROR : ServerBlock : server line invalid");
			i++;
		}
	}
	if (i == line.length() && pos != std::string::npos)
		return (true);
	else
		return (false);
}

void 	parseConfig::getValueServerBlock(int pos, std::string const& attribut, std::string& value, serverBlock &server)
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
}

int		parseConfig::getAttributName(std::string const &line, std::string &attribut, std::string& value, serverBlock &server)
{
	int 	i = 0;
	size_t 	pos;
	IT 		it;

 	std::vector<std::string> atts = {"listen ", "index ", "root ", "host ", "server_name ", "cgi_extension ", "cgi_bin ", "error "};
	for (it = atts.begin(); it != atts.end(); it++)
	{
		pos = line.find((*it));
		if (pos != std::string::npos)
		{
			attribut = (*it).substr(0, (*it).length());
			//std::cout << " Attribut = " << "'"<<  attribut << "'" << "\n";
			value = (line).substr(attribut.length(), line.length() - attribut.length());
		//	std::cout << " Value = " << "'"<<  value << "'" << "\n";
			getValueServerBlock(((*it).length() + 1), attribut, value, server);
			return ((*it).length());
		}
	}
	// std::cout << "LINE = " << line << "\n";
	// if (it == atts.end())
	// 	throw OurExcetpion("Directive in server block not allowed here");
	return (0);
}

void	parseConfig::setServerConfig(std::string const &line, serverBlock &server)
{
	int posEnd;
	std::string attributName;
	std::string value;
	posEnd = getAttributName(line, attributName, value, server);
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
		throw OurExcetpion("ERROR: LOCATION BLOCK: space between lacation and location path expected");
	return (true);
}	

/******_________________________________________******/


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

	 
	for(int bracket = 0; start != end;)
	{
		if (isServerBlock(*start))
			throw OurExcetpion("Server block not allowed here");
		if (isLocationBlock(*start))
		{
			std::cout << "YES IT IS :) \n";
		}
		//check location block

		else
		{
			setServerConfig((*start), server);
			
		}
		// std::cout << "LINE in SET ON : " << (*start) << "\n";
		start++;
	}
	// once we get all the config from one server we push back the server 
	// with updated info in the vector<serverBLock> servers
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
				throw OurExcetpion("ERROR: server block: no opening bracket");
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
				// std::cout << "IT -- = " << (*it) << "\n";
			}
			else
				throw OurExcetpion("ERROR: server block : missing a bracket");
		}
		else
			throw OurExcetpion("ERROR: server block" + *it + ": unexpected line");
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
		throw   OurExcetpion(oss.str().c_str());
	}
	else
	{
		while (std::getline(fd, line))
			content = parseLine(line, content);
		if (content.empty())
			throw  OurExcetpion("ERROR: empty file");
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
		tools::printVector(content);
	/*
		setup the servers by finding each blocks which is a server block
		then pushback each in std::vector<serverBlock>
	*/
	setServers(content, servers);
}

