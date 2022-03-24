#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <map>
#include <list>
#include <vector>

class Request
{
		public:
			Request(const std::string& str);
			~Request();
			Request&	operator=(const Request&);

			/*** GETTERS ***/
			const std::map<std::string, std::string>&			getHeaders() const;
			const std::string&									getMethod() const;
			const std::string&									getPath() const;
			const int&											getStatus() const;
			void error_checker(void);
			int parseRequestLine(const std::string & str);
			std::string parseHeaders(const std::string &str, int end);
			void	pathDecoder(std::string path);
			/*const std::map<std::string, std::string>&			getEnv() const;
			const std::string&									getVersion() const;
			int													getRet() const;
			const std::string&									getBody() const;
			int													getPort() const;
			const std::string&									getQuery() const;
			const std::string&									getRaw() const;
			const std::list<std::pair<std::string, float> >&	getLang() const;*/

			/*** SETTERS **/
			/*void	setBody(const std::string& line);
			void	setRet(int);
			void	setMethod(const std::string &method);*/

			/*** FUNCTIONS ****/
			void		parse(const std::string& str);


            private:

			Request(void);
            void printer(void);
            std::string									_method;
			std::string									_version;
			std::map<std::string, std::string>			_headers;
            std::string									_path;
			int											status;
			//std::map<std::string, std::string>			_env_for_cgi;
			int											_ret;
			std::string									_body;
			int											_port;
			//std::string									_path;
			//std::string									_query;
			//std::list<std::pair<std::string, float> >	_lang;
			const std::string&							_orig_req;

};

#endif
