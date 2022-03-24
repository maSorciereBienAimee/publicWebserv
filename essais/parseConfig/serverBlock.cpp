#include "serverBlock.hpp"
 
serverBlock::serverBlock() : _port(80), _index(), _name(), _host(0), _locations() {}

serverBlock::~serverBlock() {}

serverBlock::serverBlock(const serverBlock& cpy) : _port(cpy._port), _index(cpy._index), _name(cpy._name), _host(cpy._host), _locations(cpy._locations) {}

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

serverBlock serverBlock::operator=(const serverBlock& other)
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
	return  (*this);
}

void						serverBlock::setHost(const uint32_t host) { this->_host = host; }
void						serverBlock::setName(const std::string name) { this->_name = name; }		
void						serverBlock::setPort(const uint16_t port) { this->_port = port; }
void						serverBlock::setIndex(std::vector<std::string> const &index) { this->_index = index; }
void						serverBlock::setHostStr(std::string str) { this->_hostStr = str; }
void						serverBlock::setPortStr(std::string str) { this->_portStr = str; }
void						serverBlock::setRootServer(std::string str) { this->_server_root = str; }
void						serverBlock::setError(std::string str) { this->_error = str; }
void						serverBlock::setCgiBin(std::string str) { this->_cgi_bin_s = str; }
void						serverBlock::setCgiExt(std::string ext) { this->_cgi_ext_s = ext; }


uint32_t					serverBlock::getHost(void) const { return (this->_host); }
std::string					serverBlock::getName(void) const { return (this->_name); }
uint16_t					serverBlock::getPort(void) const { return (this->_port); }
std::vector<std::string> 	serverBlock::getIndex(void) const { return (this->_index); }
std::string					serverBlock::getHostStr(void) const { return (this->_hostStr); }
std::string					serverBlock::getPortStr(void) const { return (this->_portStr); }
std::string					serverBlock::getRootServer(void) const { return (this->_server_root); }
std::string					serverBlock::getError(void) const { return (this->_error); }
std::string					serverBlock::getCgiBin(void) const { return (this->_cgi_bin_s); }
std::string					serverBlock::getCgiExt(void) const { return (this->_cgi_ext_s); }


bool serverBlock::operator==(const serverBlock& other)
{
	return (_host == other._host && _name == other._name && _port == other._port);
}
