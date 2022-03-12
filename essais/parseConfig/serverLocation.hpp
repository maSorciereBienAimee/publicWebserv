#ifndef _LOCATIONBLOCK_HPP_
#define _LOCATIONBLOCK_HPP_

#include <vector>
#include <string>
#include <iostream>
#include "../tools/tools.hpp"

class serverLocation
{

   
	private:
	
		std::vector<std::string>	_index;
		std::vector<std::string>	_methods;
		std::string					_root;
		std::string					_cgi_ext;
		std::string					_cgi_bin;
		std::string				    _language;
		std::string				    _auth_basic_file;
		std::string				    _location_path;
		bool      			    	_auto_index;
		bool    			    	_auth_basic;
		int							_body_size;
		std::string                 _redirection;

	public:

		serverLocation();
		serverLocation(const serverLocation &cpy);
		~serverLocation();
		serverLocation &operator=(const serverLocation &other);

};



#endif