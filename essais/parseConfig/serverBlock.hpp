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
		void						setHost(const uint32_t host);
		void						setHostStr(std::string str);
		void						setPortStr(std::string str);
		void						setName(const std::string name);
		void						setPort(const uint16_t port);
		void						setIndex(std::vector<std::string> const &index);
		void						setRootServer(std::string str);
		void						setError(std::string str);
		void						setCgiBin(std::string str);
		void						setCgiExt(std::string ext);

		uint32_t					getHost(void) const;
		std::string					getName(void) const;
		uint16_t					getPort(void) const;
		std::vector<std::string> 	getIndex(void) const;
		std::string					getHostStr(void) const;
		std::string					getPortStr(void) const;
		std::string					getRootServer(void) const;
		std::string					getError(void) const;
		std::string					getCgiBin(void) const;
		std::string					getCgiExt(void) const;

		bool operator==(const serverBlock& other);
};


#endif
