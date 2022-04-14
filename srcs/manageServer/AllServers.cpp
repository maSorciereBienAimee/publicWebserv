#include "AllServers.hpp"

int stop = 0;

AllServers::AllServers(void) //default constructor, may be never used
{
	this->epfd = -1;
	this->events = (struct epoll_event *)calloc(MAX_CLIENT, sizeof(struct epoll_event));
}

AllServers::~AllServers(void) //destructor to complete
{	
	clear_fd();
	free(events);
}

AllServers::AllServers(std::string path) //constructor wich accept the argv[1] of main, that is the path of the config file
{
	parseConfig config;
	
	this->events = (struct epoll_event *)calloc(MAX_CLIENT, sizeof(struct epoll_event));	//alloc event where will be stock event that epoll_wait will receive (alloc 5000, maybe add a realloc if event > 5000 latter in code)
	this->epfd = -1;									//epfd is the file descriptor of the creation of an epoll, init here at -1 to say that is not create yet
	try
	{
			config.parsing(path, this->serverBlocks);                                      //launch parsing of the config file and stock data of each server in a vector<serverBlock>
	}
	catch(const OurException& e)
	{
		clear_fd();
		free(events);
		std::cerr << RED << e.what() << RESET << std::endl;
		exit(1);
	}
	std::vector<serverBlock> blocks = serverBlocks;
	std::vector<serverBlock>::iterator it = serverBlocks.begin();
	int i = 0;
	int j;
	for (; it != serverBlocks.end(); it++, i++)
	{
		j = 0;
		for (std::vector<serverBlock>::iterator it2 = blocks.begin(); j < i; it2++, j++)
		{
			//SEGFAULT WHEN ENTER IN THE CONDITION 
			if (it2->getPortStr() == it->getPortStr() && (it2->getHostStr() == it->getHostStr()
				|| it2->getName() == it->getName()))
			{
				std::cout << RED << it->getHostStr() << ":" << it->getPortStr() << " already set, " << it->getName() << " is delete" << RESET << std::endl;
				this->serverBlocks.erase(it);
				i--;
				it--;
				break ;
			}
		}
	}
	for (std::vector<serverBlock>::iterator it = serverBlocks.begin(); it != serverBlocks.end(); it++)
        	this->servers.push_back(Server(*it));						//for each data receive earliest in the vector serverBlock, create a server, it initiate in Server class a private member called infoServer and take data parsed before, then, after create, push it in a vector<Server>
	for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
	{	
                it->connect();									// for each server in the vector, connect (create connection socket, initiate and bind to the right port and host, and then launch listen() to listen event
		allListen.push_back(it->getListen());						// get the listenFd of each server and stocck it in a vector<int>, the listenfd is the file descriptor of the socket connexion of the server
	}
}

void AllServers::init()			//init epoll
{
	struct epoll_event ev;
	
	this->epfd = epoll_create(MAX_CLIENT);	//create an epoll, and stock the fd that it create
	if (!this->epfd)
	{
		std::cout << RED << "epoll_create failed" << RESET << std::endl;
		exit(1);
	}
	for (std::vector<int>::iterator it = allListen.begin(); it != allListen.end(); it++) //for each listendFd (that come from server class), we create add an epoll_event with flag EPOLLIN (read flag) EPOLLERR(error flag) and EPOLLHUP( hang up flag) 
	{										     // instanciated this first epoll_event allow to know if an event happened in these particular socket, if yes, that means data are ready to be read
		ev.events = EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLHUP;
		ev.data.ptr = NULL;
		ev.data.u32 = 0;
		ev.data.u64 = 0;
		ev.data.fd = *it;
		if (epoll_ctl(this->epfd, EPOLL_CTL_ADD, *it, &ev) < 0)			//epfd = fd of epoll, EPOLL_CTL_ADD = add event flag, *it, the actual listenfd that we want to observ, &ev, event to add
		{
			std::cout << RED << "epoll_ctl failed" << RESET << std::endl;
			clear_fd();
			exit(1);
		}
	}
}
void AllServers::clear_fd()
{
	for (std::map<int,int>::iterator it = allFd.begin(); it != allFd.end(); it++)
	{
		epoll_ctl(this->epfd, EPOLL_CTL_DEL, it->first, NULL);
		close(it->first);
	}
	if (this->epfd != -1)
	{
		close(this->epfd);
		this->epfd = -1;
	}
}

int AllServers::is_it_equal(int fd) 						//this function allow to know if the fd in parameter correspond to one of the listenFd of a server, if it's the case, it return the fd, else return -1;
{
	for (std::vector<int>::iterator it = allListen.begin(); it != allListen.end(); it++)
	{
		if (fd == *it)
			return (fd);
	}
	return (-1);
}

void AllServers::loop() //it's the principal running function here that will make works all server
{
	int res;
	int ret;
	int data;
	signal(SIGINT, handler_sigquit);
	while (1)
	{
		usleep(8000);
		res = epoll_wait(this->epfd, this->events, MAX_CLIENT, 0); // epoll_wait is waiting for something happen
		if (stop == 1)
		{
			for (std::map<int, Server *>::iterator it = toSend.begin(); it != toSend.end(); it++)
			{
				close(it->first);
				epoll_ctl(epfd, EPOLL_CTL_DEL, it->first, NULL);
			}
			break ;
		}


		for (int i = 0; i < res; i++)
		{
			if ((ret = is_it_equal(this->events[i].data.fd)) != -1)
				addFd(ret);
		}
		std::map<int, int> copy = allFd;
		for (std::map<int, int>::iterator itC =copy.begin(); itC != copy.end(); itC++)
		{
			for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
			{
				if (it->getListen() == itC->second)
				{
					std::map<int,int>::iterator it2 = allFd.find(itC->first);
					data = it->readData(itC->first);
					if (data == -1)
					{
						if (toSend.find(itC->first) != toSend.end())
							toSend.erase(itC->first);
						close(itC->first);
						epoll_ctl(epfd, EPOLL_CTL_DEL, itC->first, NULL);
						allFd.erase(it2);
						break;
					}
					else if (data == -3)
						toSend.insert(std::make_pair(itC->first, &(*it)));
					else if (data == 1)
					{
						if (toSend.find(itC->first) == toSend.end())
							toSend.insert(std::make_pair(itC->first, &(*it)));
						allFd.erase(it2);
						break;
					}
				}
			}
		}
		std::map<int, Server *> stock = toSend;
		int res;
		int f;
		for (std::map<int, Server *>::iterator it2 = stock.begin(); it2 != stock.end(); it2++)
		{
			std::string rep;
			f = it2->first;
			if (it2->second->getRep() == "HTTP/1.1 100 CONTINUE\r\n\r\n")
			{
				rep = it2->second->getRep();
				std::cout << std::endl << CYAN << "REPLY IS :" << std::endl << rep << RESET << std::endl;
			}
			else
				rep = it2->second->getCutReply();
			res = send(it2->first, rep.c_str(), rep.size(), 0);
			if ( res >= 0)
				std::cout << GREEN << "send succeed" << RESET << std::endl;
			if ( res == -1)
			{
				close(it2->first);
				epoll_ctl(epfd, EPOLL_CTL_DEL, it2->first, NULL);
				std::cout << RED << "send failed" << RESET << std::endl;
				toSend.erase(f);
			}
			if (it2->second->getOk() == 0)
			{
				close(it2->first);
				epoll_ctl(epfd, EPOLL_CTL_DEL, it2->first, NULL);
				toSend.erase(f);
			}
		}
	}
}

void AllServers::addFd(int fd_origin)
{
	int clifd;
	struct epoll_event ev;

	clifd = accept(fd_origin, NULL, NULL); // return an fd that correspond to the first connection request in the queue of the fd in parameter
	if (clifd > 0)
	{
		nonblock(clifd);	          //get this new fd non blocking
		ev.events = EPOLLIN | EPOLLOUT | EPOLLET;	  // init an event with these flag, EPOLLIN for read, EPOLLET to hang despite available data is presente
		ev.data.ptr = NULL;
		ev.data.u32 = 0;
		ev.data.u64 = 0;
		ev.data.fd = clifd;
		if (epoll_ctl(this->epfd, EPOLL_CTL_ADD, clifd, &ev) < 0) //add this new event in epoll_event list
		{
			clear_fd();
			exit(1);
		}
		allFd.insert(std::make_pair(clifd, fd_origin));   //stock in a map<int,int>. key = clifd, the new fd, and value correspond to the listenfd of the server corresponding
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

void	handler_sigquit(int sig)
{
	if (sig)
		stop = 1;
}
