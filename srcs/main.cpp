#include "manageServer/Server.hpp"
#include "manageServer/AllServers.hpp"
#include "parseConfig/parseConfig.hpp"
#include "parseConfig/serverBlock.hpp"
#include "request/Request.hpp"
#include "tools/tools.hpp"

int main(int argc, char **argv)
{
	if (argc > 2)
	{
		std::cout << "Error: too much arguments" << std::endl;
		return (1);
	}
	std::string path;
	if (argc == 2)
		path = argv[1];
	else
		path = "conf/default.conf";
	AllServers all(path);
	all.init();
	all.loop();
}
