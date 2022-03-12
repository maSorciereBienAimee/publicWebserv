#include "Server.hpp"
#include "parseConfig/parseConfig.hpp"
#include "parseConfig/serverBlock.hpp"
#include "Request.hpp"
#include "tools/tools.hpp"


int main(int argc, char **argv)
{
	

	if (argc != 2)
	{
		std::cout << "Error: argument" << std::endl;
		return (1);
	}

	Server myServ(argv[1]);

	myServ.connect();
	myServ.init_epoll();
	myServ.loop();
}
