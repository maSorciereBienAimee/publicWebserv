#ifndef _LOCATIONBLOCK_HPP_
#define _LOCATIONBLOCK_HPP_

#include <vector>
#include <string>
#include <iostream>
#include "../tools/tools.hpp"
#include "parseConfig.hpp"
#include "serverBlock.hpp"


class serverLocation
{

   
	private:
	
		std::vector<std::string>	_index;
		std::vector<std::string>	_methods;
		std::string					_root;
		std::string					_cgi_ext;
		std::string					_cgi_bin;
		std::string				    _language; //not implemented
		std::string				    _auth_basic_file;
		std::string				    _location_path;
		bool      			    	_auto_index;
		bool    			    	_auth_basic;
		int							_body_size;
		std::string                 _redirection;
		std::string                 _uploadLoc;
		bool						_bodyIsSetL;


	public:

		serverLocation();
		serverLocation(const serverLocation &cpy);
		~serverLocation();
		serverLocation &operator=(const serverLocation &other);

		void	setIndex(std::vector<std::string> const &index);
		void	setAI(bool autoindex);
		void	setAuthBasic(bool auth);

		void	setMethods(std::vector<std::string> const &methods);
		void	setLocationPath(std::string const& path);
		void	setBody(int size);
		void	setCgiExt(std::string ext);
		void	setCgiBin(std::string path);
		void	setRedir(std::string path);
		void	setAuthUsrFile(std::string path);
		void	setRootLoc(std::string path);
		void	setUploadLoc(std::string path);
		void	setBodySetLoc(bool val);


		bool						getAI(void) const;
		bool						getAuthBasic(void) const;
		std::vector<std::string> 	getIndex(void) const;
		std::vector<std::string> 	getMethods(void) const;
		std::string					getLocationPath(void) const;
		int							getBody(void) const;
		std::string					getCgiExt(void) const;
		std::string					getCgiBin(void) const;
		std::string					getRedir(void) const;
		std::string					getAuthUsrFile(void) const;
		std::string					getRootLoc(void) const;
		std::string					getUploadLoc(void) const;
		bool						getBodySetLoc(void) const;

};



#endif
