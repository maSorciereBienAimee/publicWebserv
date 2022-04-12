#ifndef ALLSERVERS_HPP
#define ALLSERVERS_HPP
#include <map>
#include <vector>
#include <csignal>
#include "Server.hpp"
# define BLACK "\33[30;30m"
# define RED "\33[30;31m"
# define GREEN "\33[30;32m"
# define YELLOW "\33[30;33m"
# define BLUE "\33[30;34m"
# define PURPLE "\33[30;35m"
# define CYAN "\33[30;36m"
# define WHITE "\33[30;37m"
# define RESET "\x1b[0m"

void handler_sigquit(int sig);
extern int stop;

class AllServers
{
	public :
	AllServers();
	AllServers(std::string path);
	~AllServers();
	void	init();
	void	loop();
	void	clear_fd();
	void	addFd(int fd);
	void	nonblock(int sockfd);
	int		is_it_equal(int fd);

	private:
	std::vector<serverBlock>	serverBlocks;
	std::vector<Server>			servers;
	std::map<int, Server *>			toSend;
	std::map<int, std::string>			reading;
	int							epfd;
	std::vector<int>			allListen;
	std::map<int, int>			allFd;
	struct epoll_event			*events;

};

#endif
