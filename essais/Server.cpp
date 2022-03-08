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

void Server::processNewLine(std::string request, int i)
{
	int n;
	int size = 1024;
	char buf[size];
	n = 1;
	//while (request.find("\r\n\r\n") != std::string::npos)
	while (n > 0)
	{
		n = recv(this->events[i].data.fd, buf, size - 1, 0);
		request += buf;
	}
}

/*
void Server::processTransferEncoding(std::string request)
{

}
*/

/*
void Server::processContentLength(std::string request)
{

}
*/

void Server::readData(int i)
{
	int n;
	int size = 1024;
	char buf[size];
	std::string request;
	int inutile;
	n = recv(this->events[i].data.fd, buf, size - 1, 0);
	if (n == 0)
	{
		std::cout << "connexion close by client" << std::endl;
		close (this->events[i].data.fd);
		epoll_ctl(this->epfd, EPOLL_CTL_DEL, this->events[i].data.fd, NULL);
	}
	if (n < 0)
	{
		std::cout << "error with recv" << std::endl;
		close (this->events[i].data.fd);
		epoll_ctl(this->epfd, EPOLL_CTL_DEL, this->events[i].data.fd, NULL);
	}
	else
	{
		request += buf;
//		if (request.find("Transfer-Encoding") != std::string::npos)
		//	this->processTransferEncoding(request);
//		else if (request.find("Content-Length") != std::string::npos)
		//	this->processContentLength(request);
//		else
			this->processNewLine(request, i);
		
		this->pseudoReponse(request, i);
		close(this->events[i].data.fd);
		epoll_ctl(this->epfd, EPOLL_CTL_DEL, this->events[i].data.fd, NULL);
	}
}


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


void Server::pseudoReponse(std::string request, int i) //destinee a etre suprimee quand la class reponse sera faite
{
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
	Request Req(request);
	std::string goodresponse = "HTTP/1.1 200 ok\nContent-Type: text/html\nContent-Length: " + lenstr +"\n\n" + bufStr;
	free(b);
	char badrequest[1024] = {"HTTP/1.1 400 Bad Request\r\n\r\n<HTML><HEAD><meta http-equiv=\"content-type\" content=\"text/html;charset=utf-8\">\r\n<TITLE>Bad Request</TITLE></HEAD><BODY>\r\n<H1>Bad Request</H1>\r\n</BODY></HTML>\r\n\r\n"};
	char badresponse[1024] = {"HTTP/1.1 404 Not Found\r\n\r\n<HTML><HEAD><meta http-equiv=\"content-type\" content=\"text/html;charset=utf-8\">\r\n<TITLE>Not Found</TITLE></HEAD><BODY>\r\n<H1>Not Found</H1>\r\n</BODY></HTML>\r\n\r\n"};
		
		
		if (Req.getMethod() != "GET")	
			send(this->events[i].data.fd, badrequest, strlen(badrequest), 0);	
		else if (Req.getPath() == "/" || Req.getPath() == "/index.html")
			send(this->events[i].data.fd, goodresponse.c_str(), strlen(goodresponse.c_str()), 0);
		else
			send(this->events[i].data.fd, badresponse, strlen(badresponse), 0);
} //fin de la fonction  a supprimer quand Response sera faite
