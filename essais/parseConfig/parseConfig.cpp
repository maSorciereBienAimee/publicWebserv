#include "parseConfig.hpp"

parseConfig::parseConfig(void){ return; }

parseConfig::~parseConfig(void){ return; }

parseConfig::parseConfig(const parseConfig& cpy)
{}

parseConfig &parseConfig::operator=(const parseConfig& other){ return *this; }


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
		if (line[0] == '#')
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
		if (!line[i + 1] && tools::isSpaces(line))
		{
			content.push_back(line);
			return (content);
		}
	}
   
	return (content);
}


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


void parseConfig::setOneServer(IT &start, IT &end, std::vector<serverBlock> &servers)
{
	std::cout << "ici\n";
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
				setOneServer(start, --it, servers);
			else
				throw OurExcetpion("ERROR: server block : missing a bracket");
		}
		else
			throw OurExcetpion("ERROR: server block" + *it + ": unexpected line");
		// if (it != content.end())
		// {
		// 	std::cout << "is it nessecary ??\n";
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

   tools::printContent(content);
	//delete all empty lines
	/*
		setup the servers by finding each blocks which is a server block
		then pushback each in std::vector<serverBlock>
	*/
	setServers(content, servers);
	

}