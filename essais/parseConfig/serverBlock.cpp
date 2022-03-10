#include "serverBlock.hpp"
 
serverBlock::serverBlock() : _port(80), _index(), _name(), _host(0) {}

serverBlock::~serverBlock() {}

serverBlock::serverBlock(const serverBlock& cpy) : _port(cpy._port), _index(cpy._index), _name(cpy._name), _host(cpy._host) {}


serverBlock serverBlock::operator=(const serverBlock& other)
{
	_host = other._host;
	_name = other._name;
	_port = other._port;
	_index = other._index;
	return  (*this);
}

void						serverBlock::setHost(const uint32_t host) { this->_host = host; }
void						serverBlock::setName(const std::string name) { this->_name = name; }		
void						serverBlock::setPort(const uint16_t port) { this->_port = port; }
void						serverBlock::setIndex(std::vector<std::string> const &index) { this->_index = index; }

uint32_t					serverBlock::getHost(void) const { return (this->_host); }
std::string					serverBlock::getName(void) const { return (this->_name); }
uint16_t					serverBlock::getPort(void) const { return (this->_port); }
std::vector<std::string> 	serverBlock::getIndex(void) const { return (this->_index); }


bool serverBlock::operator==(const serverBlock& other)
{
	return (_host == other._host && _name == other._name && _port == other._port);
}