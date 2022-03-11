#include "tools.hpp"

namespace tools
{

	void    printVector(std::vector<std::string> content)
	{
		std::vector<std::string>::iterator it;
		std::cout << "_____CONTENT IS______\n";
		for (it = content.begin(); it != content.end(); it++)
			std::cout << (*it) << "\n";
		std::cout << "___________\n";
		
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

	std::string removeDuplicateSpaces(std::string const &str) 
	{
		std::string s;
		std::string word;
		std::istringstream ss(str);
	
		while (ss >> word) {
			if (!s.empty()) {
				s += ' ';
			}
			s += word;
		}
		return s;
	}

	unsigned int    strToIp(std::string strIp)
	{
		size_t  sep = 0;
		unsigned int   n;
		unsigned char  m[4];
		size_t  start = 0;
		if (strIp == "localhost")
			strIp = "127.0.0.1";
		for (unsigned int i = 3 ; i != std::numeric_limits<uint32_t>::max(); i--) {
			sep = strIp.find_first_of('.', sep);
			std::string str = strIp.substr(start, sep);
			n = atoi(str.c_str());
			m[i] = static_cast<unsigned char>(n);
			sep++;
			start = sep;
		}
		unsigned final = *(reinterpret_cast<unsigned int *>(m));
		return final;
		}
}
