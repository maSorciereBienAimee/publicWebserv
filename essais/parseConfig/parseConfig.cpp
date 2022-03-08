#include "parseConfig.hpp"


parseConfig::parseConfig(){}

parseConfig::~parseConfig(void){}

parseConfig::parseConfig(const parseConfig& cpy)
{
    *this = cpy;
}

parseConfig &parseConfig::operator=(const parseConfig& other){ return *this; }

std::vector<std::string> parseConfig::parseLine(std::string line, std::vector<std::string> content)
{
    return (content);
}

void parseConfig::parsing(std::string path)
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
            throw   OurExcetpion("ERROR: empty file");
    }

    //delete all empty lines
    //setup the servers


    std::cout << "ICI" << std::endl;
}