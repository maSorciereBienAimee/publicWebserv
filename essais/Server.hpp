#ifndef SERVER_HPP
# define SERVER_HPP

#include <sys/epoll.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <fstream>
#include <sstream>
#include <vector>


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
	std::string processNewLine(std::string, int i);
	std::string processContentLength(std::string);
	std::string chunkDecoder(std::string str);
	void nonblock(int sockfd); // !je crois qu'on a pas le droit a cela
	std::vector<std::string> _server;
	void pseudoReponse(std::string str, int i); // a supprimer quand response sera faite

	private:
	std::string request;
	int listenfd;
	struct epoll_event *events;
	int epfd;
	void addFd();
	void readData(int i);
};

#endif
