#ifndef SERVER_HPP
# define SERVER_HPP

#include <sys/epoll.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <fstream>
#include <sstream>


#define PORT 8080
#define MAX_CLIENT 5000

class Server
{
	public :
	Server();
	~Server();
	void clear_fd();
	void connect();
	void init_epoll();
	void loop();
	void nonblock(int sockfd); // !je crois qu'on a pas le droit a cela

	private:
	int listenfd;
	struct epoll_event *events;
	int epfd;
	void addFd();
	void readData(int i);
};

#endif
