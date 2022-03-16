#include "AllServers.hpp"

AllServers::AllServers(void)
{
	this->listenfd = -1;
	this->epfd = -1;
	this->events = (struct epoll_event *)calloc(MAX_CLIENT, sizeof(struct epoll_event));
}

AllServers::~AllServers(void)
{	
	clear_fd();
	free(events);
}

AllServers::AllServers(std::string path)
{
	parseConfig config;
	int reuse_addr = 1;
	
	this->events = (struct epoll_event *)calloc(MAX_CLIENT, sizeof(struct epoll_event));
	this->listenfd = -1;
	this->epfd = -1;
        try
        {
                config.parsing(path, this->serverBlocks);
        }
        catch(const OurExcetpion& e)
        {
                std::cerr << e.what() << '\n';
        }
	for (std::vector<serverBlock>::iterator it = serverBlocks.begin(); it != serverBlocks.end(); it++)
        	this->servers.push_back(Server(*it));
	for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
	{
                it->connect();
		allListen.push_back(it->getListen());
	}
}

void AllServers::init()
{
	struct epoll_event ev;
	
	this->epfd = epoll_create(MAX_CLIENT);
	if (!this->epfd)
	{
		std::cout << "epoll_create failed" << std::endl;
		close(this->listenfd);
		exit(1);
	}
	for (std::vector<int>::iterator it = allListen.begin(); it != allListen.end(); it++)
	{
		ev.events = EPOLLIN | EPOLLERR | EPOLLHUP;
		ev.data.fd = *it;
		if (epoll_ctl(this->epfd, EPOLL_CTL_ADD, *it, &ev) < 0)
		{
			std::cout << "epoll_ctl failed" << std::endl;
			clear_fd();
			exit(1);
		}
	}
}
void AllServers::clear_fd()
{
	if (this->listenfd != -1)
			close(this->listenfd);
	if (this->epfd != -1)
			close(this->epfd);
}

int AllServers::is_it_equal(int fd)
{
	for (std::vector<int>::iterator it = allListen.begin(); it != allListen.end(); it++)
	{
		if (fd == *it)
			return (fd);
	}
	return (-1);
}

void AllServers::loop()
{
	int res;
	int ret;
	while (1)
	{
		usleep(8000);
		res = epoll_wait(this->epfd, this->events, MAX_CLIENT, -1);
		for (int i = 0; i < res; i++)
		{
			if ((ret = is_it_equal(this->events[i].data.fd)) != -1)
				addFd(ret);
			else
			{
				for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
				{

					if (it->getListen() == allFd[this->events[i].data.fd])
					{
						it->readData(this->events[i].data.fd, epfd);
						break;	
					}
				}
			}
		}
	}
}

void AllServers::addFd(int fd_origin)
{
	int clifd;
	struct epoll_event ev;

	clifd = accept(fd_origin, NULL, NULL);
	if (clifd > 0)
	{
		nonblock(clifd);
		ev.events = EPOLLIN | EPOLLET;
		ev.data.fd = clifd;
		if (epoll_ctl(this->epfd, EPOLL_CTL_ADD, clifd, &ev) < 0)
		{
			clear_fd();
			exit(1);
		}
		allFd.insert(std::make_pair(clifd, fd_origin));
	}
}

void AllServers::nonblock(int sockfd)
{
  int opts;
  opts = fcntl(sockfd, F_GETFL);
   if(opts < 0)
  {
		clear_fd();
    	exit(1);
   }
  opts = (opts | O_NONBLOCK);
  if(fcntl(sockfd, F_SETFL, opts) < 0)
   {
    perror("fcntl(F_SETFL)\n");
    exit(1);
  }
}

