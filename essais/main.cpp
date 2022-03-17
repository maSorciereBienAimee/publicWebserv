#include "manageServer/Server.hpp"
#include "manageServer/AllServers.hpp"
#include "parseConfig/parseConfig.hpp"
#include "parseConfig/serverBlock.hpp"
#include "request/Request.hpp"
#include "tools/tools.hpp"

void    printServerBlock(std::vector<serverBlock> content)
	{
		std::vector<serverBlock>::iterator it;
		std::cout << "_____ServerBlock IS______\n";
		for (it = content.begin(); it != content.end(); it++)
		{
			std::cout<< "HOST : " << (*it).getHost() << "\n";
			std::cout<< "PORT : " << (*it).getPort() << "\n";
			std::cout<< "NAME : " << (*it).getName() << "\n";
			tools::printVector((*it).getIndex());
		}
		std::cout << "___________\n";		
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Error: argument" << std::endl;
		return (1);
	}
	AllServers all(argv[1]);
	all.init();
	all.loop();
}
