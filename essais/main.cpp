#include "Server.hpp"

int main()
{
	Server myServ;

	myServ.connect();
	myServ.init_epoll();
	myServ.loop();
}
