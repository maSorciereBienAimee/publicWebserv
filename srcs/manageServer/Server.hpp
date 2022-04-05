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
#include "../parseConfig/parseConfig.hpp"
#include "../parseConfig/serverBlock.hpp"
#include "../request/Request.hpp"
#include "../tools/tools.hpp"
#include "../cgi/Cgi.hpp"


#define MAX_CLIENT 5000

class Server
{
	public :
	Server();
	Server(serverBlock block);
	~Server();

	void clear_fd();
	void connect();
	std::string processContent(std::string, int fd, int epfd, bool *max_size);
	bool	check_size_body(std::string request);
	std::string processContentLength(std::string);
	std::string chunkDecoder(std::string str);
	void nonblock(int sockfd); // !je crois qu'on a pas le droit a cela
	std::vector<std::string> _server;
	void pseudoReponse(std::string str, int fd, bool max_size_check); // a supprimer quand response sera faite
	int getListen() const;
	void readData(int fd, int epfd);
	private:
	std::string request;
	serverBlock infoConfig;
	int listenfd;
	struct epoll_event *events;
};

#endif
