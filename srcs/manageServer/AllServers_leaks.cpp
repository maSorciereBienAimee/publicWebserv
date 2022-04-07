#include "AllServers.hpp"

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
			std::cerr << e.what() << '\n';
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
			if (it2->getPortStr() == it->getPortStr() && it2->getHostStr() == it->getHostStr())
			{
				std::cout << "erase " << it->getName() << std::endl;
				this->serverBlocks.erase(it);
				i--;
				break ;
			}
		}
	}
	for (std::vector<serverBlock>::iterator it2 = serverBlocks.begin(); it2 != serverBlocks.end(); it2++)
		std::cout << it2->getName() << std::endl;
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
		std::cout << "epoll_create failed" << std::endl;
		exit(1);
	}
	for (std::vector<int>::iterator it = allListen.begin(); it != allListen.end(); it++) //for each listendFd (that come from server class), we create add an epoll_event with flag EPOLLIN (read flag) EPOLLERR(error flag) and EPOLLHUP( hang up flag)
	{										     // instanciated this first epoll_event allow to know if an event happened in these particular socket, if yes, that means data are ready to be read
		ev.events = EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLHUP;
		ev.data.fd = *it;
		if (epoll_ctl(this->epfd, EPOLL_CTL_ADD, *it, &ev) < 0)			//epfd = fd of epoll, EPOLL_CTL_ADD = add event flag, *it, the actual listenfd that we want to observ, &ev, event to add
		{
			std::cout << "epoll_ctl failed" << std::endl;
			clear_fd();
			exit(1);
		}
	}
}
void AllServers::clear_fd()
{
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
	static int count;
	int res;
	int ret;
	while (1)
	{
		usleep(8000);
		res = epoll_wait(this->epfd, this->events, MAX_CLIENT, 0); // epoll_wait is waiting for something happen
		for (int i = 0; i < res; i++)  				   //when something happen, check all the fd which are ready to read in this->events
		{
			if ((ret = is_it_equal(this->events[i].data.fd)) != -1) //check if the event that we are reading has an fd that correspond to a listenFd of a server
				addFd(ret);					//if it is the case, that means that a new event is created so we had it in epoll_event list
			else							//else we are goind to read the data
			{
				for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
				{

					if (it->getListen() == allFd[this->events[i].data.fd])    //searching here for corresponding server of the actual fd. We set it when we add the Fd to the list at creation
					{
						std::map<int,int>::iterator it2 = allFd.find(this->events[i].data.fd);

						it->readData(this->events[i].data.fd, epfd); 	  //send to read in Server class
						allFd.erase(it2);
						count++;
						if (count == 2)
							return;
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

	clifd = accept(fd_origin, NULL, NULL); // return an fd that correspond to the first connection request in the queue of the fd in parameter
	if (clifd > 0)
	{
		nonblock(clifd);	          //get this new fd non blocking
		ev.events = EPOLLIN | EPOLLOUT | EPOLLET;	  // init an event with these flag, EPOLLIN for read, EPOLLET to hang despite available data is presente
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

