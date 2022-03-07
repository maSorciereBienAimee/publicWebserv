#include "Request.hpp"

Request::Request(const std::string& str) : _orig_req(str)
{
	this->parse(str);
}

Request::~Request()
{
}

Request&	Request::operator=(const Request& copy)
{
    _method = copy._method;
    _path = copy._path;
	_version = copy._version;
    _body = copy._body;
    int i = 0;
    int end = _headers.size();
    std::map<std::string, std::string> tmp = copy.getHeaders();
    std::map<std::string, std::string>::iterator it = tmp.begin();
    while (i < end)
    {
        _headers.insert(*it);
        i++;
        it++;
    }
    
	return *this;
}

const std::map<std::string, std::string> &Request::getHeaders(void) const
{
    return (_headers);
}

int Request::parse(const std::string &str)
{
    /*** LINE1 ***/
    //todo ERROR HANDLING
    std::string line;
    int end = str.find_first_of('\r');
    line = str.substr(0, end );
    int break_1 = line.find_first_of(' ');
    _method = line.substr(0, break_1);
    break_1 += 1;
    line = line.substr(break_1, line.length());
    int break_2 = line.find_first_of(' ');
    _path = line.substr(0, break_2);
    break_2 += 1;
    _version = line.substr(break_2, end );
    
    

    /*** HEADERS ***/
    std::string tmp = str;
    std::string key;
    std::string val;
    int split;
    tmp = tmp.erase(0, (size_t)end + 2);
    int ptr = 0;
    int ptr_end = tmp.find_first_of('\n');
    int count = 0;
    while (ptr_end != ptr + 1)
    {
        split = tmp.find_first_of(':');
        key = tmp.substr(0, split);
        val = tmp.substr(split + 2, ((ptr_end - 1) - (split + 2)));
        std::pair<std::string, std::string> tmp_pair(key, val);
        _headers.insert(tmp_pair);
        tmp = tmp.erase(0, ptr_end + 1);
        ptr_end = tmp.find_first_of('\n');
        
    }
    
    while (tmp.find_first_of('\n') == 1)
    {
        tmp = tmp.erase(0, 1);
    }
    _body = tmp.substr(0, tmp.find_first_of('\n'));
    printer();
}

void Request::printer(void)
{
    std::cout << "------REQUEST INFO RECOVERED IN REQUEST CLASS------" << std::endl;
    std::cout << "METHOD" << std::endl;
    std::cout  << "|" << _method <<  "|" <<std::endl;
    std::cout << "PATH" << std::endl;
    std::cout  << "|" << _path << "|" << std::endl;
    std::cout << "VERSION" << std::endl;
    std::cout << "|" << _version << "|" << std::endl;
    int i = 0;
    int end = _headers.size();
    std::cout << "HEADERS" << std::endl;
    std::map<std::string, std::string>::iterator it;
    it = _headers.begin();
    while (i < end)
    {
        std::cout << "HEADER " << i << "->KEY: |" << it->first << "| VALUE: |" << it->second << "|" << std::endl;
        i++;
        it++;
    }
    std::cout << "BODY" << std::endl;
    std::cout << "|" << _body << "|" << std::endl;
}