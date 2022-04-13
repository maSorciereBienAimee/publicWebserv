#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <list>
#include <vector>
# define BLACK "\33[30;30m"
# define RED "\33[30;31m"
# define GREEN "\33[30;32m"
# define YELLOW "\33[30;33m"
# define BLUE "\33[30;34m"
# define PURPLE "\33[30;35m"
# define CYAN "\33[30;36m"
# define WHITE "\33[30;37m"
# define RESET "\x1b[0m"
class Request
{
		public:
			Request(std::string str, std::string root);
			~Request();
			Request&	operator=(const Request&);

			/*** GETTERS ***/
			const std::map<std::string, std::string>&			getHeaders() const;
			const std::string&									getMethod() const;
			const std::string&									getPath() const;
			const int&											getStatus() const;
			int parseRequestLine(const std::string & str);
			std::string parseHeaders(const std::string &str, int end);
			void	pathDecoder(std::string path);
			std::string getHost() const;
			const std::string&									getBody() const;

			/*** FUNCTIONS ****/
			void		parse(const std::string& str);


            private:

			Request(void);
            void printer(void);
            std::string									_method;
			std::string									_version;
			std::map<std::string, std::string>			_headers;
			int											status;
			int											_ret;
			std::string									_body;
			int											_port;
			const std::string							_orig_req;
			std::string									_path;
			void versionChecker(void);

};

#endif
