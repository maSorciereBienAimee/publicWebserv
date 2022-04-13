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
	int					processContent(int fd, bool *max_size);
	int					contentLength(int fd, std::string buf);
	bool						check_size_body(std::string request);
	std::string							processContentLength(std::string);
	int							chunkDecoder(int fd);
	bool errorChecker(std::string request);
	void						nonblock(int sockfd);
	std::vector<std::string>	_server;
	void						launchResponse(std::string str, bool max_size_check);
	int							getListen() const;
	int							readData(int fd);
	std::string					getCutReply();
	std::string					getHex(int n);
	std::string					getRep() const;
	int							getOk() const;
	int							getIsChunked() const;

	private:
	std::string			request;
	serverBlock			infoConfig;
	int					listenfd;
	struct epoll_event	*events;
	int					ok;
	int					isChunked;
	std::string			reply;
	std::string			header;
	std::string			body;
	std::map<int, std::string>	req;
};

#endif
