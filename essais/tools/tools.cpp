#include "tools.hpp"


namespace tools
{

    void    printContent(std::vector<std::string> content)
    {
        std::vector<std::string>::iterator it;
        for (it = content.begin(); it != content.end(); it++)
            std::cout << (*it) << "\n";
    }

    bool    isSpaces(std::string str)
    {
    
        for (int i = 0; str[i]; i++)
        {
            if (!isspace(str[i]))
                return (false);
        }
        return (true);
    }
}
