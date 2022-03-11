#include "serverLocation.hpp"

serverLocation::serverLocation() : 
_root(""), _cgi_ext(""), _cgi_bin(""), _language(""), _auth_basic_file(""), _location_path(""), _auto_index(0), _auth_basic(0), _body_size(-1), _redirection(""){}

serverLocation::serverLocation(const serverLocation &cpy) :
_index(cpy._index) , _methods(cpy._methods), _root(cpy._root), _cgi_ext(cpy._cgi_ext),
_cgi_bin(cpy._cgi_bin), _language(cpy._language), _auth_basic_file(cpy._auth_basic_file),
_location_path(cpy._location_path), _auto_index(cpy._auto_index), _auth_basic(cpy._auth_basic), _body_size(cpy._body_size), _redirection(cpy._redirection){}

serverLocation &serverLocation::operator=(const serverLocation &other) {

	_index = other._index;
	_methods = other._methods;
	_root = other._root;
	_cgi_ext = other._cgi_ext;
	_cgi_bin = other._cgi_bin;
	_language = other._language;
	_auth_basic = other._auth_basic;
	_auth_basic_file = other._auth_basic_file;
	_location_path = other._location_path;
	_auto_index = other._auto_index;
	_body_size = other._body_size;
	_redirection = other._redirection;
	return (*this);
}

serverLocation::~serverLocation(){ return ; }