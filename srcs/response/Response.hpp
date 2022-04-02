#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include <map>
#include "../request/Request.hpp"
#include "../cgi/Cgi.hpp"
#include "../parseConfig/parseConfig.hpp"
#include "../parseConfig/serverLocation.hpp"
#include "../parseConfig/serverBlock.hpp"

class Response
{
	public:
	Response(Request R, int F, Cgi myCgi, serverLocation loc, serverBlock server);
	~Response();
	void launch();
	void	_delete(std::string path);
	void setHeaders();
	void setBody();
	void _get(Request R);
	void _homepage(Request R);
	void _post(Request R);
	void _other(Request R);
	std::string getReply();
	void readIn(std::string file);
	void initErrors();
	std::string makeDate(void);
	void _notAllowed();

	private :
	Response();

	std::string _header;
	std::vector<std::string> index_vec;
	std::string body_message;
	std::string body;
	std::string body_len;
	std::string reply;
	int status;
	std::string type;
	std::string errorPath;
	std::map<std::string, std::string> extra_headers;
	std::map<int, std::string> errors;
	Request request;
	int fd;
	Cgi _cgi;
	serverLocation _loc;
	serverBlock		_server;
	bool	_autoindex;
	std::string	_path;
	std::string	_old_path;
	std::string _redir_path;
	bool 		redir;


};

#endif
