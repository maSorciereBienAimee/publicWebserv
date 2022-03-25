#include "serverBlock.hpp"
 
serverBlock::serverBlock() :  _host(0), _hostStr(), _portStr(), _name(), _port(80), _index(), _cgi_ext_s(), _cgi_bin_s(), _error(), _server_root(), _locations() ,
_methods_s(), _auto_index_s(0), _body_size_s(), _auth_basic_s(), _auth_basic_file_s(), _redirection_s() {}

serverBlock::~serverBlock() {}

serverBlock::serverBlock(const serverBlock& cpy) : 
 _host(cpy._host), _hostStr(cpy._hostStr), _portStr(cpy._portStr), _name(cpy._name), _port(cpy._port), _index(cpy._index), _cgi_ext_s(cpy._cgi_ext_s), _cgi_bin_s(cpy._cgi_bin_s), _error(cpy._error), _server_root(cpy._server_root), _locations(cpy._locations),
  _methods_s(cpy._methods_s), _auto_index_s(cpy._auto_index_s), _body_size_s(cpy._body_size_s), _auth_basic_s(cpy._auth_basic_s),
  _auth_basic_file_s(cpy._auth_basic_file_s), _redirection_s(cpy._redirection_s)  {}

serverBlock		const & serverBlock::getServerBlock()
{
	std::cout << "yo" << std::endl;
	for (std::vector<serverLocation>::iterator it = _locations.begin(); it !=_locations.end(); it++)
			std::cout << it->getLocationPath() << std::endl;
	return (*this);
}

std::vector<serverLocation> const& serverBlock::getLocation() const
{
	return _locations;
}

serverBlock 	serverBlock::operator=(const serverBlock& other)
{
	_host = other._host;
	_name = other._name;
	_port = other._port;
	_index = other._index;
	_locations = other._locations;
	_hostStr = other._hostStr;
	_portStr = other._portStr;
	_error = other._error;
	_cgi_bin_s = other._cgi_bin_s;
	_cgi_ext_s = other._cgi_ext_s;
	_methods_s = other._methods_s;
	_auto_index_s = other._auto_index_s;
	_body_size_s = other._body_size_s;
	_auth_basic_s = other._auth_basic_s;
  	_auth_basic_file_s = other._auth_basic_file_s;
	_redirection_s = other._redirection_s;

	return  (*this);
}

void							serverBlock::setHost(const uint32_t host) { this->_host = host; }
void							serverBlock::setName(std::string const& name) { this->_name = name; }		
void							serverBlock::setPort(const uint16_t port) { this->_port = port; }
void							serverBlock::setIndex(std::vector<std::string> const &index) { this->_index = index; }
void							serverBlock::setHostStr(std::string const& str) { this->_hostStr = str; }
void							serverBlock::setPortStr(std::string const& str) { this->_portStr = str; }
void							serverBlock::setRootServer(std::string const& str) { this->_server_root = str; }
void							serverBlock::setError(std::string const& str) { this->_error = str; }
void							serverBlock::setCgiBin(std::string const& str) { this->_cgi_bin_s = str; }
void							serverBlock::setCgiExt(std::string const& ext) { this->_cgi_ext_s = ext; }

void							serverBlock::setMethods_s(std::vector<std::string> const &methods) { this->_methods_s = methods; }
void							serverBlock::setAI_s(bool autoindex) { this->_auto_index_s = autoindex; }
void							serverBlock::setBody_s(int size) { this->_body_size_s = size; }
void							serverBlock::setAuthBasic_s(bool auth) { this->_auth_basic_s = auth; }
void							serverBlock::setAuthUsrFile_s(std::string path) { this->_auth_basic_file_s = path; }
void							serverBlock::setRedir_s(std::string path) { this->_redirection_s = path; }


uint32_t						serverBlock::getHost(void) const { return (this->_host); }
std::string	const&				serverBlock::getName(void) const { return (this->_name); }
uint16_t						serverBlock::getPort(void) const { return (this->_port); }
std::vector<std::string> 		serverBlock::getIndex(void) const { return (this->_index); }
std::string	const&				serverBlock::getHostStr(void) const { return (this->_hostStr); }
std::string	const&				serverBlock::getPortStr(void) const { return (this->_portStr); }
std::string	const&				serverBlock::getRootServer(void) const { return (this->_server_root); }
std::string	const&				serverBlock::getError(void) const { return (this->_error); }
std::string	const&				serverBlock::getCgiBin(void) const { return (this->_cgi_bin_s); }
std::string	const&				serverBlock::getCgiExt(void) const { return (this->_cgi_ext_s); }
bool							serverBlock::getAuthBasic_s(void) const { return (this->_auth_basic_s); }
std::vector<std::string> 		serverBlock::getMethods_s(void) const { return (this->_methods_s); } 
bool							serverBlock::getAI_s(void) const { return (this->_auto_index_s); }
int								serverBlock::getBody_s(void) const { return (this->_body_size_s); }
std::string						serverBlock::getAuthUsrFile_s(void) const { return (this->_auth_basic_file_s); }
std::string						serverBlock::getRedir_s(void) const { return (this->_redirection_s); }

bool 							serverBlock::operator==(const serverBlock& other)
{
	return (_host == other._host && _name == other._name && _port == other._port);
}
