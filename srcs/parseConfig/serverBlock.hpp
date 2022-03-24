#ifndef _SERVERBLOCK_HPP_
#define _SERVERBLOCK_HPP_

#include <vector>
#include <string>
#include <iostream>
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
		std::string				    _error;
		std::string				    _server_root;

	public: 

		std::vector<serverLocation>				_locations;

		serverBlock();
		~serverBlock();
		serverBlock(const serverBlock& cpy);
		serverBlock 									operator=(const serverBlock& other);
		serverBlock	const &								getServerBlock();
		std::vector<serverLocation> const& 			getLocation() const;
		void										setHost(const uint32_t host);
		void										setHostStr(std::string const& str);
		void										setPortStr(std::string const& str);
		void										setName(std::string const& name);
		void										setPort(const uint16_t port);
		void										setIndex(std::vector<std::string> const &index);
		void										setRootServer(std::string const& str);
		void										setError(std::string const& str);
		void										setCgiBin(std::string const& str);
		void										setCgiExt(std::string const& ext);

		uint32_t									getHost(void) const;
		std::string	 const&							getName(void) const;
		uint16_t									getPort(void) const;
		std::vector<std::string> 					getIndex(void) const;
		std::string	const&							getHostStr(void) const;
		std::string	const&							getPortStr(void) const;
		std::string	const&							getRootServer(void) const;
		std::string	const&							getError(void) const;
		std::string	const&							getCgiBin(void) const;
		std::string	const&							getCgiExt(void) const;

		bool operator==(const serverBlock& other);
};


#endif
