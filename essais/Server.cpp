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

std::string Server::processContentLength(std::string request)
{
	std::string tmp = request.substr(request.find("Content-Length: ") + 15, request.length());
	int index = request.find("\r\n\r\n");
	std::string tmp1 = tmp.substr(0, index - 1);
	std::string body = request.substr(index + 4, request.length() - (index + 4));

	if (body.length() != atoi(tmp1.c_str()))
		std::cout << "ERROR: Body length not as expected" << atoi(tmp1.c_str()) << std::endl; 
	return(request + "\r\n\r\n");
}


std::string Server::processContent(std::string request, int i)
{
	int size = 24;
	std::string bufStr;
	char buf[size];
	int check = 0;
	check = recv(this->events[i].data.fd, buf, size - 1, 0);
	while (check > 0)
	{
		request += buf;
		memset(buf, '\0', size);
		check = recv(this->events[i].data.fd, buf, size - 1, 0);
	}
	request += buf;
	if (check == 0)
	{
		std::cout << "connexion close by client" << std::endl;
		epoll_ctl(this->epfd, EPOLL_CTL_DEL, this->events[i].data.fd, NULL);
		close (this->events[i].data.fd);
	}
	if (request.find("Content-Length: ") == std::string::npos && (request.find_last_of("\r\n\r\n") != request.length() - 1))
	{
		int i = 0;
		while (i < request.length() - 1)
		{
			std::cout << i << "is: " << request[i] << std::endl;
			i++;
		}
		std::cout << "find last of is" << request.find_last_of("\r\n\r\n") << "LEN IS" << request.length() - 1 << std::endl;
		std::cout << "error with recv" << std::endl;
		epoll_ctl(this->epfd, EPOLL_CTL_DEL, this->events[i].data.fd, NULL);
		close (this->events[i].data.fd);
	}
	if (request.find("Transfer-Encoding: chunked") != std::string::npos)
		return (chunkDecoder(request));
	if (request.find("Content-Length: ") != std::string::npos)
		return(processContentLength(request));
	return (request);
}

std::string Server::chunkDecoder(std::string str)
{
	std::cout << "CHUNK DECODER" << std::endl;
    //FROM BEGINNING OF REQUEST UNTIL THE END OF FIRST EMPTY LINE
	std::string	head = str.substr(0,str.find("\r\n\r\n") + 4);
    //FROM AFTER NEW LINE TO END OF MESSAGE
	std::string	coded = str.substr(str.find("\r\n\r\n") + 4, str.length() - 1);
	
    std::string	subchunk = coded;
	std::string	body;
	// GET CHUNK SIZE FROM BASE 16 
    int			chunksize = strtol(coded.c_str(), NULL, 16);
	size_t		i = 0;

	while (chunksize)
	{
        //FIND END OF INITAL CHUNKSIZE LABEL
		i = coded.find("\r\n", i) + 2;
        // SKIP PAST INITIAL SIZE \R\N LABEL
		body += coded.substr(i, chunksize);
		//SIZE ADDED IS CHUNKSIZE + THE \R\N AT END WHICH ISN'T INCLUDED IN CHUNKSIZE
        i += chunksize + 2;
		//TAKE NEXT 100
        subchunk = coded.substr(i, coded.length() - i);
		//GET SIZE OF NEXT CHUNK
        chunksize = strtol(subchunk.c_str(), NULL, 16);
	}
	std::string decoded = head + body + "\r\n\r\n";
    return (decoded);
}

void Server::readData(int i)
{
	int n;
	int size = 24;
	char buf[size];
	std::string request;
	int inutile;
	n = recv(this->events[i].data.fd, buf, size - 1, 0);
	if (n == 0)
	{
		std::cout << "connexion close by client" << std::endl;
		epoll_ctl(this->epfd, EPOLL_CTL_DEL, this->events[i].data.fd, NULL);
		close (this->events[i].data.fd);
	}
	else if (n < 0) //IF WE HAVE LARGER BUFFER DO WE NEED TO ADD: && request.find_last_of("\r\n\r\n") != request.length() - 4
	{
		std::cout << "error with recv" << std::endl;
		epoll_ctl(this->epfd, EPOLL_CTL_DEL, this->events[i].data.fd, NULL);
		close (this->events[i].data.fd);
	}
	else
	{
		request += buf;
		request = this->processContent(request, i);
		this->pseudoReponse(request, i);
		epoll_ctl(this->epfd, EPOLL_CTL_DEL, this->events[i].data.fd, NULL);
		close(this->events[i].data.fd);
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
	std::string goodresponse = "HTTP/1.1 200 ok\nContent-Type: text/html\nConnection: keep-alive\nContent-Length: " + lenstr +"\n\n" + bufStr;
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
