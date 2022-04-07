#ifndef _SERVERBLOCK_HPP_
#define _SERVERBLOCK_HPP_

#include <vector>
#include <string>
#include <iostream>
#include <stdint.h>
#include "serverLocation.hpp"
#include "parseConfig.hpp"

/* 	
	Class which contains one server block
	Inside there is the config attributs of the server 
	And the location block: info of the location block will be in the locationBlock class
	At the end we'll have one vector containing all the servers blocks
*/
class serverLocation;

class serverBlock {

	private:

		uint32_t					_host;
		std::string 				_hostStr;
		std::string					_portStr;
		std::string					_name;
		uint16_t					_port;
		std::vector<std::string> 	_index;
		std::string					_cgi_ext_s;
		std::string					_cgi_bin_s;	
		std::string				    _server_root;
		std::vector<std::string>	_methods_s; 
		bool      			    	_auto_index_s; 
		int							_body_size_s; 
		bool    			    	_auth_basic_s; 
		std::string				    _auth_basic_file_s; 
		std::string                 _redirection_s; 
		std::string					_upload_s;
		bool						_bodyIsSet;
		bool						_errorIsSet;
		std::string				    _errorPath;
		int							_errorCode;


	public: 

		std::vector<serverLocation>				_locations;

		serverBlock();
		~serverBlock();
		serverBlock(const serverBlock& cpy);
		serverBlock 								operator=(const serverBlock& other);
		serverBlock	const &							getServerBlock();
		std::vector<serverLocation> const& 			getLocation() const;
		void										setHost(const uint32_t host);
		void										setHostStr(std::string const& str);
		void										setPortStr(std::string const& str);
		void										setName(std::string const& name);
		void										setPort(const uint16_t port);
		void										setIndex(std::vector<std::string> const &index);
		void										setRootServer(std::string const& str);
		void										setCgiBin(std::string const& str);
		void										setCgiExt(std::string const& ext);
		void										setMethods_s(std::vector<std::string> const &methods);
		void										setAI_s(bool autoindex);
		void										setBody_s(int size);
		void										setAuthBasic_s(bool auth);
		void										setAuthUsrFile_s(std::string path);
		void										setRedir_s(std::string path);
		void										setUpload_s(std::string path);
		void										setBodySet_s(bool val);
		//
		void										setErrorSet(bool val);
		void										setErrorPath(std::string const& str);
		void										setErrorCode(int code);
		




		uint32_t									getHost(void) const;
		std::string	 const&							getName(void) const;
		uint16_t									getPort(void) const;
		std::vector<std::string> 					getIndex(void) const;
		std::string	const&							getHostStr(void) const;
		std::string	const&							getPortStr(void) const;
		std::string	const&							getRootServer(void) const;
		std::string	const&							getCgiBin(void) const;
		std::string	const&							getCgiExt(void) const;
		std::vector<std::string> 					getMethods_s(void) const;
		bool										getAI_s(void) const;
		int											getBody_s(void) const;
		bool										getAuthBasic_s(void) const;
		std::string									getAuthUsrFile_s(void) const;
		std::string									getRedir_s(void) const;
		std::string									getUpload_s(void) const;
		bool										getBodySet_s(void) const;

		bool										getErrorSet(void) const;
		std::string	const&							getErrorPath(void) const;
		int											getErrorCode(void) const;

		bool operator==(const serverBlock& other);
};


#endif
