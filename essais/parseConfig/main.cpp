#include "parseConfig.cpp"
#include "parseConfig.hpp"

int main(int argc, char **argv)
{
	parseConfig confFile;
	if (argc != 2)
	{
		std::cout << "Error: argument" << std::endl;
		return (1);
	}

	else
	{
		try
		{
			confFile.parsing(argv[1]);
		}
		catch(const OurExcetpion& e)
		{
			std::cerr << e.what() << '\n';
		}
		
	}
	return (0);
}
