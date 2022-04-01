#include "Request.hpp"
#include <stdexcept>
//Request::Request(void) : _orig_req("yo")
//{
//}

Request::Request(std::string str, std::string root) : _orig_req(str), _root(root)
{
    status = 200;
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

void Request::pathDecoder(std::string path)
{
    std::string ret;
    char c;
    int i;
    int x;
    for (i=0; i < path.length(); i++)
    {
        if (path[i] == '%')
        {
            sscanf(path.substr(i + 1,2).c_str(), "%x", &x);
            c = static_cast<char>(x);
            ret += c;
            if (i + 2 <= path.length())
                i = i + 2;
            else
                i++;
        }
        else
            ret += path[i];
    }
    this->_path = ret;
}

int Request::parseRequestLine(const std::string &str)
{
    /*** LINE1 PARSING ***/
    std::string line = "";
    int end = 0;
    while (line.length() == 0) //IGNORE IF EMPTY LINES b4 FIRST LINE AS PER RFC RECOMMENDATION
    {
        end = str.find_first_of('\r');
        //COLLECT FIRST LINE ONLY
        line = str.substr(0, end);
    }
    int break_1 = line.find_first_of(' ');
    //METHOD IS FROM 0 UNTIL FIRST SPACE
    _method = line.substr(0, break_1);
    break_1 += 1;
    //REMOVE METHOD AND SPACE FROM LINE
    line = line.substr(break_1, end - break_1);
    int break_2 = line.find_first_of(' ');
    //PATH IS FROM 0 UNTIL FIRST SPACE
    _path = line.substr(0, break_2);
    pathDecoder(_path);
    int x = _root.length() - 1;
    // GET RID OF DOUBLE //
    if (x > -1 && _path[0] == '/' && _root[x] == '/')
        _root.erase(_root.end() - 1);
    _path = _root + _path;
    break_2 += 1;
    //VERSION IS FROM AFTER PREV SPACE UNTIL END
    _version = line.substr(break_2,  line.length() - break_2);
	return end;
}

std::string Request::parseHeaders(const std::string &str, int end)
{
    /*** HEADERS ***/
    std::string tmp = str;
    std::string key;
    std::string val;
    int split;
    //DELETE FIRST LINE FROM STR
    tmp = tmp.erase(0, (size_t)end + 2);
    int ptr = 0;
    int ptr_end = tmp.find_first_of('\n');
    int count = 0;
    //UNTIL WE GET TO AN EMPTY LINE
    while (ptr_end != ptr + 1)
    {
        //COLLECT KEY AND VALUE AND INSERT IN MAP HEADERS
        split = tmp.find_first_of(':');
        key = tmp.substr(0, split);
        val = tmp.substr(split + 2, ((ptr_end - 1) - (split + 2)));
        std::pair<std::string, std::string> tmp_pair(key, val);
        _headers.insert(tmp_pair);
        tmp = tmp.erase(0, ptr_end + 1);
        ptr_end = tmp.find_first_of('\n');

    }
    return tmp;
}

void Request::parse(const std::string &str)
{
	int ref = 0;
    std::string tmp;
    ref = parseRequestLine(str);
    tmp = parseHeaders(str, ref);
    //SKIP OVER EMPTY LINES
    while (tmp.find_first_of("\r\n") == 0)
        tmp = tmp.erase(0, 1);
    //COLLECT BODY
    _body = tmp.substr(0, tmp.length() - 4);
    error_checker();
    //TMP:PRINTER
    printer();
}

void Request::error_checker(void)
{
    const std::string con_key = "Content-Length";
    //CHECK LENGTH
    if (_headers.find(con_key) != _headers.end())
    {
        if (_body.length() != (atoi(_headers[con_key].c_str())))
            this->status = 400;
    }
}

void Request::printer(void)
{
    std::cout << "------REQUEST INFO RECOVERED IN REQUEST CLASS------" << std::endl;
    std::cout << "ROOT IS: " << _root << std::endl;
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

std::string const &Request::getMethod() const
{
	return (this->_method);
}
std::string const &Request::getPath() const
{
	return (this->_path);
}

std::string Request::getHost() const
{
    const std::string key = "Host";
    if (_headers.find("Host") == _headers.end())
    {
        std::cout << "PROB 1" << std::endl;
        return NULL;
    }
    try
    {
       return _headers.at(key);
    }
    catch (const std::exception &e)
    {
        std::cout << "PROB 2" << std::endl;
         return "";
    }
}

int const &Request::getStatus() const
{
	return (this->status);
}

void	Request::setRoot(const std::string& root)
{
    _root = root;
    return;
}

std::string	Request::getRoot(void)
{
    return _root;
}

const std::string& Request::getBody() const
{
	return (this->_body);
}
