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
#include <algorithm>
#include "../parseConfig/parseConfig.hpp"
#include "../parseConfig/serverBlock.hpp"
#include "../request/Request.hpp"
#include "../tools/tools.hpp"
#include "../cgi/Cgi.hpp"


#define MAX_CLIENT 5000
#define MAX_SIZE 65536

class Server
{
	public :
	Server();
	Server(serverBlock block);
	~Server();
	void						clear_fd();
	void						connect();
	std::string					processContent(int fd, int epfd, bool *max_size);
	bool						check_size_body(std::string request);
	std::string					processContentLength(std::string);
	std::string					chunkDecoder(std::string str);
	void						nonblock(int sockfd);
	std::vector<std::string>	_server;
	void						launchResponse(std::string str, bool max_size_check);
	int							getListen() const;
	void						readData(int fd, int epfd);
	std::string					getCutReply();
	std::string					getHex(int n);
	int							getOk() const;

	private:
	std::string			request;
	serverBlock			infoConfig;
	int					listenfd;
	struct epoll_event	*events;
	int					ok;
	std::string			reply;
	std::string			header;
	std::string			body;
};

#endif
