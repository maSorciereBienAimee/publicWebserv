#include "Server.hpp"
//modification en cours
int main()
{
	Server myServ;
	myServ.connect();
	myServ.init_epoll();
	myServ.loop();
	return (0);
	return (1);
}
