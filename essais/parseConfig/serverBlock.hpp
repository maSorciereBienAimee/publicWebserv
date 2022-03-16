#ifndef _SERVERBLOCK_HPP_
#define _SERVERBLOCK_HPP_

#include <vector>
#include <string>
#include <iostream>
#include "serverLocation.hpp"

/* 	
	Class which contains one server block
	Inside there is the config attributs of the server 
	And the location block: info of the location block will be in the locationBlock class
	At the end we'll have one vector containing all the servers blocks
*/

class serverBlock {

	private:

		uint32_t					_host;
		std::string					_name;
		uint16_t					_port;
		std::vector<std::string> 	_index;
		std::vector<serverLocation>	_locations;

	public: 

		serverBlock();
		~serverBlock();
		serverBlock(const serverBlock& cpy);
		serverBlock 				operator=(const serverBlock& other);

		serverBlock	const &				getServerBlock();

		void						setHost(const uint32_t host);
		void						setName(const std::string name);
		void						setPort(const uint16_t port);
		void						setIndex(std::vector<std::string> const &index);

		uint32_t					getHost(void) const;
		std::string					getName(void) const;
		uint16_t					getPort(void) const;
		std::vector<std::string> 	getIndex(void) const;

		bool operator==(const serverBlock& other);
};


#endif
