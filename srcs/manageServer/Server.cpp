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
		std::cout << RED << "Bind address already in use on " << this->infoConfig.getHostStr() << ":" << this->infoConfig.getPortStr() << RESET << std::endl;
		clear_fd();
		exit(1);
	}
	if (listen(this->listenfd, 1024) < 0)
	{
		clear_fd();
		exit(1);
	}
	std::cout << GREEN << "Server '" << this->infoConfig.getName() << "' is launched on " << this->infoConfig.getHostStr() << ":" << this->infoConfig.getPortStr() << RESET << std::endl;
}

int Server::contentLength(int fd, std::string buf)
{
	if ((req[fd]).find("Content-Length: ") != std::string::npos)
	{
		isChunked = 0;
		int c = (req[fd]).find("Content-Length:");
		std::string body = "";
		std::string length = (req[fd]).substr(c, (req[fd]).size() - c);
		int d = length.find("\r\n");
		std::string length2 = length.substr(0, d);
		size_t i = 0;
		for ( ; i < length2.length(); i++ )
		{
			if (std::isdigit(length2[i]))
				break;
		}
		length2 = length2.substr(i, length2.length() - i );
		i = 0;
		for ( ; i < length2.length(); i++ )
		{
			if (!(std::isdigit(length2[i])))
				break;
		}
		length2 = length2.substr(0, i);
		std::stringstream ss(length2);
		size_t x = 0;
		ss >> x;
		if ((req[fd]).find("\r\n\r\n") == std::string::npos)
				return (0);
		if ((req[fd]).find("\r\n\r\n") != std::string::npos)
			body = (req[fd]).substr((req[fd]).find("\r\n\r\n") + 4, (req[fd]).length() - 1);
		if ((req[fd]).find("100-continue") != std::string::npos && body.size() != x && req[fd] == buf)
				return (-3);
		if (body.size() != x && buf.size() >= MAX_SIZE)
			return (0);
		if (body.size() != x)
		{
			std::cout << RED << "Body size (" << body.size() << ") don't match with Content-Length (" << x << ")" << RESET << std::endl;
			return (-2);
		}
		else
		{
			req[fd] += "\r\n\r\n";
			return (1);
		}
	}
	return (-1);
}

bool Server::errorChecker(std::string request)
{
	//obligatoire, a moins: "M_P_H\r\nHost:\r\n\r\n"
	if (request.length() < 22)
	{
		std::cout << RED << "Invalid request" << RESET << std::endl;
		return false;
	}
	std::string line = "";
    std::string::size_type end = 0;
	//skip empty lines
    while (line.length() == 0)
    {
        end = request.find_first_of('\r');
		if (end == std::string::npos)
			return false;
        line = request.substr(0, end);
    }
	if (std::count(line.begin(), line.end(), ' ') < 2)
	{
		std::cout << RED << "Invalid request line" << RESET << std::endl;
		return false;
	}
	//obligatoire: "M_P_H\r\n"
	if (line.length() < 9)
	{
		std::cout << RED << "Invalid request line" << RESET << std::endl;
		return false;
	}
	int sp = 0;
	for (unsigned int i = 0; i < line.length(); i++)
	{
		if (line[i] == ' ')
		{
			if (i == 0)
				sp++;
			else if (line[i - 1] != ' ')
				sp++;
		}
	}
	if (sp != 2)
	{
		std::cout << RED << "Invalid request line" << RESET << std::endl;
		return false;
	}
	if (request.find("\r\nHost:") == std::string::npos)
	{
		std::cout << RED << "missing host field" << RESET << std::endl;
		return false;
	}
	return true;
}

int Server::processContent(int fd, bool *max_size)
{
	std::string bufStr;
	char buf[MAX_SIZE + 1];
	int check = 1;
	int a;
	memset(buf, 0, MAX_SIZE);
	check = recv(fd, buf, MAX_SIZE, 0);
	req[fd] += buf;
	bufStr = buf;

	if ((req[fd]).find("\r\n\r\n") == std::string::npos)
	{
		if (req[fd] == "\r\n")
			req[fd] = "";
		return (0);
	}
	if (check == 0)
	{
		std::cout << PURPLE << "Connection closed by client" << RESET << std::endl;
		ok = 0;
		return (-1);
	}
	if (check == -1)
	{
		a = contentLength(fd, bufStr);
		if (a == -2)
			return (-1);
		if (a != -1)
			return (a);
		a = chunkDecoder(fd);
		if (a != -1)
			return (a);
		if ((req[fd]).find("Content-Length: ") == std::string::npos
				&& ((req[fd]).find_last_of("\r\n\r\n") != (req[fd]).length() - 1))
			return (1);
		ok = 0;
		return (-1);
	}
	if (errorChecker(req[fd]) == false)
	{
		return (-1);
	}
	if (check_size_body(req[fd]) == false)
	{
		*max_size = false;
		return (1);
	}
	a = contentLength(fd, bufStr);
	if (a == -2)
		return (-1);
	if (a != -1)
		return (a);
	a = chunkDecoder(fd);
	if (a != -1)
		return (a);
	if ((req[fd]).find("Content-Length: ") == std::string::npos
			&& ((req[fd]).find_last_of("\r\n\r\n") != (req[fd]).length() - 1))
		return (1);
	return (1);
}

int Server::chunkDecoder(int fd)
{
	if ((req[fd]).find("Transfer-encoding: chunked") != std::string::npos)
	{
	    //FROM BEGINNING OF REQUEST UNTIL THE END OF FIRST EMPTY LINE
		int len = (req[fd]).find("\r\n\r\n") + 4;
		if ((req[fd]).find("0\r\n\r\n") == std::string::npos)
				return (0);
		std::string	head = (req[fd]).substr(0,len);
   	 //FROM AFTER NEW LINE TO END OF MESSAGE
		std::string	coded = (req[fd]).substr(len, (req[fd]).size() - len);
	
		std::string	subchunk = coded;
		std::string	body;
		// GET CHUNK SIZE FROM BASE 16
		int chunksize = strtol(coded.c_str(), NULL, 16);
		size_t	i = 0;
	
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
		req[fd] = head + body + "\r\n\r\n";
		return (1);
	}
    return (-1);
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

int Server::readData(int fd)
{
	bool max_size_check = true;
	std::string str = "";
	std::map<int, std::string>::iterator ite = req.find(fd);
	int result;
	
	if (ite != req.end())
			req.insert(std::make_pair(fd, str));
	result = this->processContent(fd,&max_size_check);
	if (result == -1)
			req[fd] = "";
	if (result == -3)
	{
		this->ok = 1;
		this->reply = "HTTP/1.1 100 CONTINUE\r\n\r\n";
		this->header = "HTTP/1.1 100 CONTINUE\r\n\r\n";
		this->body = "";
	}
	if (result == 1)
	{	
		this->launchResponse(req[fd], max_size_check);
		this->ok = 1;
		std::map<int, std::string>::iterator it = req.find(fd);
		req.erase(it);
	}
	return (result);
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

std::string Server::getRep() const
{
	return (this->reply);
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
