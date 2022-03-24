#include "parseConfig.cpp"
#include "parseConfig.hpp"
#include "serverBlock.cpp"
#include "serverBlock.hpp"
#include "serverLocation.hpp"
#include "serverLocation.cpp"

int main(int argc, char **argv)
{
	parseConfig confFile;
	std::vector<serverBlock> servers;
	//std::vector<serverLocation> locations;


	if (argc != 2)
	{
		std::cout << "Error: argument" << std::endl;
		return (1);
	}
	else
	{
		try
		{
			confFile.parsing(argv[1], servers);
		}
		catch(const OurExcetpion& e)
		{
			std::cerr << e.what() << '\n';
		}	
	}
	return (0);
}
