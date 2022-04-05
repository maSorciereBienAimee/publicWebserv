#include "serverLocation.hpp"

serverLocation::serverLocation() : 
_root(""), _cgi_ext(""), _cgi_bin(""), _language(""), _auth_basic_file(""), _location_path(""), _auto_index(0),
_auth_basic(0), _body_size(-1), _redirection(""), _uploadLoc(""), _bodyIsSetL(false) {}

serverLocation::serverLocation(const serverLocation &cpy) :
_index(cpy._index) , _methods(cpy._methods), _root(cpy._root), _cgi_ext(cpy._cgi_ext),
_cgi_bin(cpy._cgi_bin), _language(cpy._language), _auth_basic_file(cpy._auth_basic_file),
_location_path(cpy._location_path), _auto_index(cpy._auto_index), _auth_basic(cpy._auth_basic), _body_size(cpy._body_size),
_redirection(cpy._redirection), _uploadLoc(cpy._uploadLoc), _bodyIsSetL(cpy._bodyIsSetL) {}

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
	_uploadLoc = other._uploadLoc;
	_bodyIsSetL = other._bodyIsSetL;
	return (*this);
}

serverLocation::~serverLocation(){ return ; }

void						serverLocation::setAI(bool autoindex) { this->_auto_index = autoindex; }
void						serverLocation::setAuthBasic(bool auth) { this->_auth_basic = auth; }
void						serverLocation::setIndex(std::vector<std::string> const &index) { this->_index = index; }
void						serverLocation::setMethods(std::vector<std::string> const &methods) { this->_methods = methods; }
void						serverLocation::setLocationPath(std::string const& path) {this->_location_path = path; }
void						serverLocation::setBody(int size) { this->_body_size = size; }
void						serverLocation::setCgiExt(std::string ext) { this->_cgi_ext = ext; }
void						serverLocation::setCgiBin(std::string path) { this->_cgi_bin = path; }
void						serverLocation::setRedir(std::string path) { this->_redirection = path; }
void						serverLocation::setAuthUsrFile(std::string path) { this->_auth_basic_file = path; }
void						serverLocation::setRootLoc(std::string path) { this->_root = path; }
void						serverLocation::setUploadLoc(std::string path) { this->_uploadLoc = path; }
void						serverLocation::setBodySetLoc(bool val) { this->_bodyIsSetL = val; }


bool						serverLocation::getAI(void) const { return (this->_auto_index); }
bool						serverLocation::getAuthBasic(void) const { return (this->_auth_basic); }
std::vector<std::string> 	serverLocation::getIndex(void) const { return (this->_index); } 
std::vector<std::string> 	serverLocation::getMethods(void) const { return (this->_methods); } 
std::string					serverLocation::getLocationPath(void) const { return (this->_location_path); }
int							serverLocation::getBody(void) const { return (this->_body_size); }
std::string					serverLocation::getCgiExt(void) const { return (this->_cgi_ext); }
std::string					serverLocation::getCgiBin(void) const { return (this->_cgi_bin); }
std::string					serverLocation::getRedir(void) const { return (this->_redirection); }
std::string					serverLocation::getAuthUsrFile(void) const { return (this->_auth_basic_file); }
std::string					serverLocation::getRootLoc(void) const { return (this->_root); }
std::string					serverLocation::getUploadLoc(void) const { return (this->_uploadLoc); }
bool						serverLocation::getBodySetLoc(void) const { return (this->_bodyIsSetL); }


