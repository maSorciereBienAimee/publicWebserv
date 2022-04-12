#include "Server.hpp"
#include <iostream>
#include "../response/Response.hpp"
#include "../tools//tools.hpp"

Server::Server()
{
	this->listenfd = -1;
	this->ok = 0;
	this->isChunked = 0;
}

Server::Server(serverBlock block)
{
	this->listenfd = -1;
	this->ok = 0;
	this->isChunked = 0;
	this->infoConfig = block;
	std::cout << GREEN << "Server '" << this->infoConfig.getName() << "' is launched on " << this->infoConfig.getHostStr() << ":" << this->infoConfig.getPortStr() << RESET << std::endl;
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
		std::cout << RED << "Bind address already in use." << RESET << std::endl;
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

	if (static_cast<int>(body.length()) != atoi(tmp1.c_str()))
		std::cout << RED << "ERROR: Body length not as expected" << atoi(tmp1.c_str()) << RESET << std::endl;
	return(request + "\r\n\r\n");
}


std::string Server::processContent(int fd, int epfd, bool *max_size)
{
	std::string request;
	std::string bufStr;
	char buf[MAX_SIZE + 1];
	int check = 1;
	memset(buf, 0, MAX_SIZE);
	check = recv(fd, buf, MAX_SIZE, 0);
	request += buf;
	if (check == 0)
	{
		std::cout << PURPLE << "Connexion closed by client" << RESET << std::endl;
		close (fd);
		epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
		ok = 0;
	}
	if (check == -1)
	{
		std::cout << RED << "No datas received" << RESET << std::endl;
		close (fd);
		epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
		ok = 0;
	}
	if (check == MAX_SIZE)
	{
		std::cout << RED << "Server max request size reached..." << RESET << std::endl;
		close (fd);
		epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
		ok = 0;
	}
	if (check_size_body(request) == false)
	{
		*max_size = false;
		return request;
	}
	if (request.find("Content-Length: ") == std::string::npos && (request.find_last_of("\r\n\r\n") != request.length() - 1))
	{
		size_t i = 0;
		while (i < request.length() - 1)
			i++;
//?		std::cout << "Find last of is " << request.find_last_of("\r\n\r\n") << "LEN IS" << request.length() - 1 << std::endl;
		close (fd);
		epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
	}
	if (request.find("Transfer-Encoding: chunked") != std::string::npos)
	{
		isChunked = 1;
		return (chunkDecoder(request));
	}
	if (request.find("Content-Length: ") != std::string::npos)
	{
			isChunked = 0;
			return(processContentLength(request));
	}
	return (request);
}

std::string Server::chunkDecoder(std::string str)
{
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

bool	Server::check_size_body(std::string request)
{
	if (request == "")
		return true;
	if (request.find("\n\r\n") == std::string::npos)
		return true;
	if (this->infoConfig.getBodySet_s() == false)
		return true;
	int max_size = this->infoConfig.getBody_s();
	//IF WE HAVENOT REACHED END OF HEADERS

	//ELSE CREATE STRING WITHOUT HEADERS
	std::string	body = request.substr(request.find("\r\n\r\n") + 4, request.length() - 1);
	if (static_cast<int>(body.length()) > max_size)
	{
		std::cout << PURPLE << "MAX SIZE: " << max_size << RESET << std::endl;
		return false;
	}
	return true;
}

void Server::readData(int fd, int epfd)
{
	bool max_size_check = true;
	std::string request;
	request = this->processContent(fd, epfd, &max_size_check);
	if (request != "")
	{	
		this->launchResponse(request, max_size_check);
		this->ok = 1;
		isChunked = 0;
	}
}

std::string	Server::getHex(int n)
{
	std::stringstream ss;
	ss << std::hex << n;
	std::string str = ss.str();
	std::transform(str.begin(), str.end(), str.begin(), ::toupper);
	return (str);
}

std::string	Server::getCutReply()
{
	std::string cut;
	std::string dup = reply;
	unsigned int size_cut = 100000;
	int len; 
	std::string hexa;
	if (this->body.size() > size_cut)
	{
		if (this->ok == 1)
		{
			len = size_cut;
			hexa = getHex(len);
			cut = this->header + "\r\n\r\n" + hexa + "\r\n" +  body.substr(0, size_cut) + "\r\n";
			len = header.size() + 4 + size_cut;
			reply = dup.substr(len, dup.size() - len); 
			this->ok = 2;
			return (cut);
		}
		else if (this->ok == 2)
		{
			if (reply.size() > size_cut)
			{
				len = size_cut;
				hexa = getHex(len);
				cut = hexa + "\r\n" + reply.substr(0, size_cut) + "\r\n";
				reply = dup.substr(size_cut, dup.size() - size_cut);
				return (cut);
			}
			else
			{
				len = reply.size();
				hexa = getHex(len);
				cut = hexa + "\r\n" + reply + "\r\n";
				reply = "";
				ok = 3;
				return (cut);
			}
		}
		else if (ok == 3)
		{
			cut = "0\r\n\r\n";
			ok = 0;
			return (cut);
		}
	}
	ok = 0;
	return (this->reply);
}

void Server::launchResponse(std::string req, bool max_size_check)
{
	std::string	queryPath;
	std::string tmp = "";
	std::string simplePath = tools::getSimplePath(req, &queryPath, this->infoConfig);

	serverLocation synthese = tools::whichLocation(simplePath, this->infoConfig);
	std::string	realPath = tools::getRelativeRoot(synthese, simplePath);
	if (max_size_check == false)
	{
		tmp = req.substr(0, req.length() - 1 - request.find("\r\n\r\n") + 4);
		req = tmp;
	}
	Request marco(req, realPath);
	Cgi myCgi(marco, synthese, this->infoConfig, queryPath, simplePath, realPath);
	int status = marco.getStatus();
	if (marco.getHost() == "127.0.0.1" && this->infoConfig.getHostStr() == "localhost")
			this->infoConfig.setHostStr("127.0.0.1");
	if (marco.getHost() == "localhost" && this->infoConfig.getHostStr() == "127.0.0.1")
			this->infoConfig.setHostStr("localhost");
	if (marco.getHost() == "127.0.0.1:" + this->infoConfig.getPortStr() && this->infoConfig.getHostStr() == "localhost")
		this->infoConfig.setHostStr("127.0.0.1");
	if (marco.getHost() == ("localhost:" + this->infoConfig.getPortStr()) && this->infoConfig.getHostStr() == "127.0.0.1")
		this->infoConfig.setHostStr("localhost");

	std::string hp2 = this->infoConfig.getName() + ':' + this->infoConfig.getPortStr();
	std::string hp = this->infoConfig.getHostStr() + ':' + this->infoConfig.getPortStr();
	if (marco.getHost() != hp && marco.getHost() != hp2)
	{
		status = 400;
	}
	myCgi.setIsIt(tools::isItCgi(realPath, synthese));
	if (max_size_check == false)
		status = 413;
	Response polo(marco, status, myCgi, synthese, infoConfig);
	this->reply = polo.getReply();
	this->header = polo.getHeader();
	this->body = polo.getBody();
//	send(fd, the_reply.c_str(), the_reply.length(), 0);
}

int Server::getOk() const
{
	return (this->ok);
}

int Server::getIsChunked() const
{
	return (this->isChunked);
}

int Server::getListen() const
{
	return (this->listenfd);
}
