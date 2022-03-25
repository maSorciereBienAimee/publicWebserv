#include "Server.hpp"
#include <iostream>
#include "../response/Response.hpp"
#include "../tools//tools.hpp"

Server::Server()
{
	this->listenfd = -1;
}
Server::Server(serverBlock block)
{
	//tools::printServerBlock(block);
	this->listenfd = -1;
	this->infoConfig = block;
}

Server::~Server()
{
	if (this->listenfd != -1)
			close(this->listenfd);
}

void Server::clear_fd()
{
	if (this->listenfd != -1)
			close(this->listenfd);
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
	srv.sin_addr.s_addr = htonl(this->infoConfig.getHost());
	srv.sin_port = htons(this->infoConfig.getPort());
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


std::string Server::processContent(std::string request, int fd, int epfd)
{
	int size = 24;
	std::string bufStr;
	char buf[size];
	int check = 0;
	check = recv(fd, buf, size - 1, 0);
	while (check > 0)
	{
		request += buf;
		memset(buf, '\0', size);
		check = recv(fd, buf, size - 1, 0);
	}
	request += buf;
	if (check == 0)
	{
		std::cout << "connexion close by client" << std::endl;
		close (fd);
		epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
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
		close (fd);
		epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
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

void Server::readData(int fd, int epfd)
{
	int n;
	int size = 24;
	char buf[size];
	std::string request;
	int inutile;
	n = recv(fd, buf, size - 1, 0);
	if (n == 0)
	{
		std::cout << "connexion close by client" << std::endl;
		close (fd);
		epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
	}
	else if (n < 0) //IF WE HAVE LARGER BUFFER DO WE NEED TO ADD: && request.find_last_of("\r\n\r\n") != request.length() - 4
	{
		std::cout << "error with recv" << std::endl;
		close (fd);
		epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
	}
	else
	{
		request += buf;
		request = this->processContent(request, fd, epfd);
		this->pseudoReponse(request, fd);
		close(fd);
		epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
	}
}


void Server::pseudoReponse(std::string req, int fd) //destinee a etre suprimee quand la class reponse sera faite
{
	Cgi myCgi;
	serverLocation synthese = tools::whichLocation(req, this->infoConfig);
	//TODO CHANGE /WEBSITE FOR LOCATION ROOT
	Request marco(req, synthese.getRootLoc() );
	int status = marco.getStatus();
	/*CHECKHOST:TODO:TOADDBACKIN
	std::string hp = this->infoConfig.getHostStr() + ':' + this->infoConfig.getPortStr();
	if (marco.getPath() != this->infoConfig.getHostStr() && marco.getPath() != hp)
		status = 400;*/

	myCgi.setIsIt(tools::isItCgi(marco.getPath(), this->infoConfig.getLocation()));
//	myCgi.init(marco, infoConfig);

	std::cout << "isCGI = " << myCgi.getIsIt() << std::endl;
//	tools::printServerBlock(infoConfig);
//	tools::printLocationBlock(infoConfig.getLocation());
	//std::cout << "LOCATION PATH IS  " << synthese.getLocationPath();
	Response polo(marco, status, myCgi, synthese);
	std::string the_reply = polo.getReply();
	send(fd, the_reply.c_str(), the_reply.length(), 0);

}

int Server::getListen() const
{
	return (this->listenfd);
}
