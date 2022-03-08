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

class parseConfig  {

    public:

    parseConfig();
    ~parseConfig(void);
    parseConfig(const parseConfig& cpy);
    parseConfig &operator=(const parseConfig& other);
    
    void    parsing(std::string path);
    std::vector<std::string> parseLine(std::string line, std::vector<std::string> content);

    
    // protected:

};


#endif