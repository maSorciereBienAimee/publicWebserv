#include "Server.hpp"
#include <iostream>
#include "Request.hpp"

Server::Server()
{
	this->listenfd = -1;
	this->epfd = -1;
	this->events = (struct epoll_event *)calloc(MAX_CLIENT, sizeof(struct epoll_event));
}

Server::~Server()
{
	if (this->listenfd != -1)
			close(this->listenfd);
	if (this->epfd != -1)
			close(this->epfd);
}

void Server::clear_fd()
{
	if (this->listenfd != -1)
			close(this->listenfd);
	if (this->epfd != -1)
			close(this->epfd);
}


void Server::connect()
{
	struct sockaddr_in srv;
	int reuse_addr = 1;

	if ((this->listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			exit(1);
	
	setsockopt(this->listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(reuse_addr));
	
	memset(srv.sin_zero, '\0', sizeof(srv.sin_zero));
	srv.sin_family = AF_INET;
	srv.sin_addr.s_addr = INADDR_ANY;
	srv.sin_port = htons(PORT);
	if (bind(this->listenfd, (struct sockaddr *) &srv, sizeof(srv)) < 0)
	{
		std::cout << "Bind address already in use \n";
		clear_fd();
		exit(1);
	}
	if (listen(this->listenfd, 1024) < 0)
	{
		clear_fd();
		exit(1);
	}
	
}

void Server::init_epoll()
{
	struct epoll_event ev;

	this->epfd = epoll_create(MAX_CLIENT);
	if (!this->epfd)
	{
		clear_fd();
		exit(1);
	}
	ev.events = EPOLLIN | EPOLLERR | EPOLLHUP;
	ev.data.fd = this->listenfd;
	if (epoll_ctl(this->epfd, EPOLL_CTL_ADD, listenfd, &ev) < 0)
	{
		clear_fd();
		exit(1);
	}
}

void Server::loop()
{
	int res;
	while (1)
	{
		usleep(8000);
		res = epoll_wait(this->epfd, this->events, MAX_CLIENT, 0);
		for (int i = 0; i < res; i++)
		{
			if(this->events[i].data.fd == this->listenfd)
				Server::addFd();
			else
				Server::readData(i);
		}
	}
}

void Server::addFd()
{
	int clifd;
	struct epoll_event ev;

	clifd = accept(this->listenfd, NULL, NULL);
	if (clifd > 0)
	{
			Server::nonblock(clifd);
		ev.events = EPOLLIN | EPOLLET;
		ev.data.fd = clifd;
		if (epoll_ctl(this->epfd, EPOLL_CTL_ADD, clifd, &ev) < 0)
		{
			clear_fd();
			exit(1);
		}
	}
}

void Server::readData(int i)
{
	int n;
	char buf[1024];
	std::string str;
	n = recv(this->events[i].data.fd, buf, 1023, 0);
	if (n == 0)
	{
		epoll_ctl(this->epfd, EPOLL_CTL_DEL, this->events[i].data.fd, NULL);
	}
	if (n < 0)
	{
		close (this->events[i].data.fd);
	}
	else
	{
		//envoyer la fonction qui donne un vrai reponse
		std::ifstream is ("index.html", std::ifstream::binary);
		if (!is)
		{
			clear_fd();
			exit(1);
		}
		is.seekg (0, is.end);
		int length = is.tellg();
		is.seekg (0, is.beg);
		char * b = (char *)malloc(sizeof(char) * (length + 1));
		b[length] = '\0';
		std::stringstream ss;
		ss << length;
		std::string lenstr= ss.str();
		is.read (b,length);
		std::string bufStr(b);
		str = buf;
		Request Req(str);
		std::string goodresponse = "HTTP/1.1 200 ok\nContent-Type: text/html\nContent-Length: " + lenstr +"\n\n" + bufStr;
		free(b);
		char badrequest[1024] = {"HTTP/1.1 400 Bad Request\r\n\r\n<HTML><HEAD><meta http-equiv=\"content-type\" content=\"text/html;charset=utf-8\">\r\n<TITLE>Bad Request</TITLE></HEAD><BODY>\r\n<H1>Bad Request</H1>\r\n</BODY></HTML>\r\n\r\n"};
		char badresponse[1024] = {"HTTP/1.1 404 Not Found\r\n\r\n<HTML><HEAD><meta http-equiv=\"content-type\" content=\"text/html;charset=utf-8\">\r\n<TITLE>Not Found</TITLE></HEAD><BODY>\r\n<H1>Not Found</H1>\r\n</BODY></HTML>\r\n\r\n"};
		
		
		if (!(buf[0]=='G') || !(buf[1]=='E') || !(buf[2]=='T') || !(buf[3]==' '))	
		{
				send(this->events[i].data.fd, badrequest, strlen(badrequest), 0);	
		}
		else if (buf[4] == '/' && buf[5] == ' ')
				send(this->events[i].data.fd, goodresponse.c_str(), strlen(goodresponse.c_str()), 0);
		else if (buf[4] == '/'
					&& buf[5] == 'i'
					&& buf[6] == 'n'
					&& buf[7] == 'd'
					&& buf[8] == 'e'
					&& buf[9] == 'x'
					&& buf[10] == '.'
					&& buf[11] == 'h'
					&& buf[12] == 't'
					&& buf[13] == 'm'
					&& buf[14] == 'l')
		{
				send(this->events[i].data.fd, goodresponse.c_str(), strlen(goodresponse.c_str()), 0);
		}
		else
		{
				send(this->events[i].data.fd, badresponse, strlen(badresponse), 0);
		}
		close(this->events[i].data.fd);
	}
}

//trouver une alternative a cette fonction
void Server::nonblock(int sockfd)
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
