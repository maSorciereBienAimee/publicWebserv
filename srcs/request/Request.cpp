#include "Request.hpp"
#include <stdexcept>

Request::Request(std::string str, std::string path) : _orig_req(str), _path(path)
{
    _method = "";
	_version = "";
	_body = "";
    _port = 0;
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
    size_t i;
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

void Request::versionChecker()
{
    if ( _version.length() < 8 || _version.length() > 9)
    {
 //       std::cout << "lenght is " << _version.length() << std::endl;
        status = 400;
    }
    std::string check = "HTTP/";
    unsigned int i;
    for (i = 0; i < check.length(); i++)
    {
        if (_version[i] != check[i])
        {
            status = 400;
            return;
        }
    }
    if (!(isdigit(_version[i])) || _version[i] == '0')
    {
        status = 400;
        return;
    }
    if (_version[i] > '1')
    {
        status = 505;
    }
    i++;
    if (_version[i] != '.')
    {
        status = 400;
        return;
    }
    i++;
    if (!(isdigit(_version[i])))
    {
        status = 400;
        return;
    }
    if (_version[i] > '1')
        status = 505;
    return;
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
    while (line[break_1 + 1] == ' ')
        break_1++;
    break_1 += 1;
    //REMOVE METHOD AND SPACE FROM LINE
    line = line.substr(break_1, end - break_1);
    int break_2 = line.find_first_of(' ');
    while (line[break_2 + 1] == ' ')
        break_2++;
    //PATH IS FROM 0 UNTIL FIRST SPACE
    pathDecoder(_path);
    break_2 += 1;
    //VERSION IS FROM AFTER PREV SPACE UNTIL END
    _version = line.substr(break_2,  line.length() - break_2);
    versionChecker();
	return end;
}

std::string Request::parseHeaders(const std::string &str, int end)
{
    /*** HEADERS ***/
    std::string tmp = str;
    std::string key;
    std::string val;
    unsigned int split;
    //DELETE FIRST LINE FROM STR
    tmp = tmp.erase(0, (size_t)end + 2);
    int ptr = 0;
    int ptr_end = tmp.find_first_of('\n');
    //UNTIL WE GET TO AN EMPTY LINE
    while (ptr_end != ptr + 1)
    {
        //COLLECT KEY AND VALUE AND INSERT IN MAP HEADERS
        split = tmp.find_first_of(':');
        std::string::size_type t = split;
        if (t != std::string::npos)
        {
            key = tmp.substr(0, split);
            if (split + 1 < (tmp.length() - 1))
                split++;
            while (tmp[split] == ' ')
                split++;
            if (!tmp[split])
                val = "";
            else
            {
                val = tmp.substr(split, (ptr_end - 1) - split);
                for (std::string::iterator it = val.end() - 1; it != val.begin(); it--)
                {
                    if (*it == ' ')
                    {
                        val.erase(it);
                    }
                }

            }
            std::pair<std::string, std::string> tmp_pair(key, val);
            _headers.insert(tmp_pair);
            tmp = tmp.erase(0, ptr_end + 1);
        }
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
    //TMP:PRINTER
    printer();
}

void Request::printer(void)
{
    std::cout << std::endl << YELLOW << "--- REQUEST IS : ---" << std::endl;
    std::cout << "METHOD : " << _method << std::endl;
    std::cout << "PATH : " << _path << std::endl;
    std::cout << "VERSION : " << _version << std::endl;
    int i = 0;
    int end = _headers.size();
    std::cout << "HEADERS : " << std::endl;
    std::map<std::string, std::string>::iterator it;
    it = _headers.begin();
    while (i < end)
    {
        std::cout << it->first << ":" << it->second << std::endl;
        i++;
        it++;
    }
    std::cout << "BODY : " << std::endl;
	std::string body_cut;
	if (_body.size() > 30)
		body_cut = _body.substr(0, 30) + "...";
	else
		body_cut = _body;
    std::cout << "|" << body_cut << "|" << RESET << std::endl;
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
        return "";
    }
    try
    {
       return _headers.at(key);
    }
    catch (const std::exception &e)
    {
         return "";
    }
}

int const &Request::getStatus() const
{
	return (this->status);
}

const std::string& Request::getBody() const
{
	return (this->_body);
}
