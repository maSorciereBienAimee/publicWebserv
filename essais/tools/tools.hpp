#ifndef _TOOLS_HPP_
#define _TOOLS_HPP_

#include <iostream>
#include <string>
#include <exception>
#include <vector>



namespace tools
{
     //just for help
    void    printContent(std::vector<std::string> content);
    bool    isSpaces(std::string str);
    
}

struct OurExcetpion : public std::exception
{
    std::string _str;

    OurExcetpion(std::string str): _str(str){}
    ~OurExcetpion() {}
    const char * what() const throw() { return _str.c_str(); }
};

#endif