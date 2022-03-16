#ifndef ALLSERVERS_HPP
#define ALLSERVERS_HPP
#include <map>
#include <vector>
#include "Server.hpp"

class AllServers
{
	public :
	AllServers();
	AllServers(std::string path);
	~AllServers();
	void init();
	void loop();
	void clear_fd();
	void addFd(int fd);
	std::string processContentLength(std::string r);
	std::string processContent(std::string r, int i);
	std::string chunkDecoder(std::string str);
	void readData(int i);
	void nonblock(int sockfd);
	void pseudoReponse(std::string str, int i);
	int is_it_equal(int fd);
	private:
	std::vector<serverBlock> serverBlocks;
	std::vector<Server> servers;
	int epfd;
	int listenfd;
	std::vector<int> allListen;
	std::map<int, int> allFd;
	struct epoll_event *events;

};

#endif
