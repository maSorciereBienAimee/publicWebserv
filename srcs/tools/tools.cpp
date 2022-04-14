#include "tools.hpp"
#include "../parseConfig/serverLocation.hpp"

namespace tools
{

	void    printVector(std::vector<std::string> content)
	{
		std::vector<std::string>::iterator it;
		//std::cout << "_____CONTENT IS______\n";
		for (it = content.begin(); it != content.end(); it++)
			std::cout << (*it) << "\n";
		std::cout << "___________\n";

	}

	void    printServerBlock(serverBlock content)
	{
		std::cout << "_____ServerBlock IS______\n";

			std::cout<< "HOST : " << content.getHost() << "\n";
			std::cout<< "NAME : " << content.getName() << "\n";
			std::cout<< "HOST STR : " << content.getHostStr() << "\n";
			std::cout<< "PORT : " << content.getPort() << "\n";
			std::cout<< "PORT STR : " << content.getPortStr() << "\n";
			std::cout<< "ROOT SERVER : " << content.getRootServer() << "\n";
			std::cout<< "CGI BIN : " << content.getCgiBin() << "\n";
			std::cout<< "CGI EXT : " << content.getCgiExt() << "\n";
			std::cout<< "AUTOINDEX : " << content.getAI_s() << "\n";
			std::cout<< "BODY SIZE : " << content.getBody_s() << "\n";
			std::cout<< "AUTH BASIC : " << content.getAuthBasic_s() << "\n";
			std::cout<< "AUTH USR FILE : " << content.getAuthUsrFile_s() << "\n";
			std::cout<< "REDIRECTION : " << content.getRedir_s() << "\n";
			std::cout<< "UPLOAD : " << content.getUpload_s() << "\n";
			std::cout<< "IS BODY SET = : " << content.getBodySet_s() << "\n";
			std::cout << "ERROR PATH = [" << content.getErrorPath() << "]\n";
			std::cout << "ERROR CODE PATH = [" << content.getErrorCode() << "]\n";
			std::cout << "ERROR IS SET PATH = [" << content.getErrorSet() << "]\n";
			
			std::cout<< "INDEX content : \n";
			printVector(content.getIndex());
			std::cout<< "METHODS content : \n";
			printVector(content.getMethods_s());
		std::cout << "___________\n";
	}

	void    printLocationBlock(std::vector<serverLocation> content)
	{
		std::vector<serverLocation>::iterator it;
		std::cout << "_____LocationBlock IS______\n";
		for (it = content.begin(); it != content.end(); it++)
		{
			std::cout<< "LOCATION PATH LOC : " << (*it).getLocationPath() << "\n";
			std::cout<< "BODY SIZE LOC : " << (*it).getBody() << "\n";
			std::cout<< "AUTOINDEX LOC : " << (*it).getAI() << "\n";
			std::cout<< "AUTH BASIC LOC : " << (*it).getAuthBasic() << "\n";
			std::cout<< "AUTH USER FILE LOC : " << (*it).getAuthUsrFile() << "\n";
			std::cout<< "CGIEXT LOC : " << (*it).getCgiExt() << "\n";
			std::cout<< "CGIBIN LOC : " << (*it).getCgiBin() << "\n";
			std::cout<< "REDIRECTION LOC : " << (*it).getRedir() << "\n";
			std::cout<< "ROOT LOC : " << (*it).getRootLoc() << "\n";
			std::cout<< "IS BODY SET : " << (*it).getBodySetLoc() << "\n";
			std::cout<< "INDEX content : \n";
			printVector((*it).getIndex());
			std::cout<< "METHODS content : \n";
			printVector((*it).getMethods());

		}
		std::cout << "___________\n";
}

	bool    isSpaces(std::string str)
	{

		for (int i = 0; str[i]; i++)
		{
			if (!isspace(str[i]))
				return (false);
		}
		return (true);
	}

	std::string removeDuplicateSpaces(std::string const &str)
	{
		std::string s;
		std::string word;
		std::istringstream ss(str);

		while (ss >> word) {
			if (!s.empty()) {
				s += ' ';
			}
			s += word;
		}
		return s;
	}

	unsigned int    strToIp(std::string strIp)
	{
		size_t  sep = 0;
		unsigned int   n;
		unsigned char  m[4];
		size_t  start = 0;
		if (strIp == "localhost")
			strIp = "127.0.0.1";
		for (unsigned int i = 3 ; i != std::numeric_limits<uint32_t>::max(); i--) {
			sep = strIp.find_first_of('.', sep);
			std::string str = strIp.substr(start, sep);
			n = atoi(str.c_str());
			m[i] = static_cast<unsigned char>(n);
			sep++;
			start = sep;
		}
		unsigned final = *(reinterpret_cast<unsigned int *>(m));
		return final;
	}

	bool isValidDirective(std::string const &dir)
    {
        if (dir == "index" || dir == "root" || dir == "server_name"
        || dir == "listen" || dir == "host" || dir == "error"
        || dir == "cgi_extension" || dir == "cgi_bin"
		|| dir == "methods" || dir == "client_max_body_size" || dir == "auth_basic"
		|| dir == "auth_basic_user_file" || dir == "redirection" || dir == "upload" )
            return (true);
        return (false);
    }
    bool isValidDirectiveLocation(std::string const &dir)
    {
        if (dir == "index" || dir == "methods" || dir == "cgi_extension"
        || dir == "cgi_bin" || dir == "language" || dir == "autoindex" || dir == "root"
        || dir == "client_max_body_size" || dir == "auth_basic" || dir == "auth_basic_user_file" || dir == "redirection" || dir == "upload")
            return (true);
        return (false);
    }

std::string getRelativeRoot(serverLocation loc, std::string simple)
{
	std::string ret;
	std::string temp;
	std::string root = loc.getRootLoc();
	int x = root.size() - 1;
	int len = loc.getLocationPath().size();
	size_t l;
	if (( l = loc.getLocationPath().find("/*.")) != std::string::npos)
			len = len - (len - l);

	temp = simple.substr(len, simple.size() - len);
	if (temp[0] != '/')
			temp = "/" + temp;
	if (x > -1 && temp[0] == '/' && root[x] == '/')
		root.erase(root.end() - 1);
	ret = root + temp;
	if (ret[ret.size() - 1] == '/')
		ret.erase(ret.end() - 1);
	return (ret);
}

int searchInConfig(std::string str, std::vector<serverLocation> location, serverLocation *loc)
{
	for (std::vector<serverLocation>::iterator it = location.begin(); it != location.end(); it++)
	{
		if ((*it).getLocationPath() == str)
		{
			(*loc).setIndex((*it).getIndex());
			(*loc).setAI((*it).getAI());
			(*loc).setAuthBasic((*it).getAuthBasic());
			(*loc).setMethods((*it).getMethods());
			(*loc).setBody((*it).getBody());
			(*loc).setCgiExt((*it).getCgiExt());
			(*loc).setCgiBin((*it).getCgiBin());
			(*loc).setRedir((*it).getRedir());
			(*loc).setAuthUsrFile((*it).getAuthUsrFile());
			(*loc).setRootLoc((*it).getRootLoc());
			(*loc).setLocationPath((*it).getLocationPath());
			(*loc).setUploadLoc((*it).getUploadLoc());
			(*loc).setBodySetLoc((*it).getBodySetLoc());
			return (1);
		}
	}
	return (0);
}

serverLocation	searchLocation(std::string path, serverBlock block)
	{
		struct stat	stock;
		std::vector<serverLocation> location = block.getLocation();
		serverLocation ret;
		std::string root = block.getRootServer();
		std::string	realPath = block.getRootServer() + path;

		ret.setIndex(block.getIndex());
		ret.setAI(block.getAI_s());
		ret.setAuthBasic(block.getAuthBasic_s());
		ret.setMethods(block.getMethods_s());
		ret.setBody(block.getBody_s());
		ret.setCgiExt(block.getCgiExt());
		ret.setCgiBin(block.getCgiBin());
		ret.setRedir(block.getRedir_s());
		ret.setAuthUsrFile(block.getAuthUsrFile_s());
		ret.setRootLoc(block.getRootServer());
		ret.setUploadLoc(block.getUpload_s());
		ret.setBodySetLoc(block.getBodySet_s());


		if (stat(root.c_str(), &stock) != 0)
			return (ret);
		if (stat(realPath.c_str(), &stock) == 0)
		{
			if (S_ISDIR(stock.st_mode))
			{
				std::string::iterator it = path.end();
				if (path.size() > 0)
						it--;
				if (path != "/" && *it == '/')
				{
					std::string tmp = path.substr(0, path.size() - 1);
					path = tmp;
				}
				if (searchInConfig(path, location, &ret) == 1)
						return (ret);
				return (ret);
			}
		}
		int i = path.size();
		std::string str = path;
		std::string ext = tools::getExtension(path);
		std::string newPath = path;
		std::string withoutExt = path;
		std::string::iterator it = str.begin();
		while (it != str.end())
			it++;
		while (1)
		{
			for (; *it != '/'; it--)
			{
				i--;
				if (it == str.begin())
					break;
			}
			str.erase(it, str.end());
			realPath = block.getRootServer() + str;
			if (stat(realPath.c_str(), &stock) == 0)
				break;
			if (str == "/")
				break ;
		}
		withoutExt = str;
		if (withoutExt == "")
				withoutExt = "/";
		if (ext != "")
		{
			newPath = str + "/" + "*" + ext;
			if (searchInConfig(newPath, location, &ret) == 1)
			{
				if (ret.getCgiExt() == ext)
					return (ret);
			}
		}
		if (searchInConfig(withoutExt, location, &ret) == 1)
			return (ret);
		return (ret);
	}

	int	isItCgi(std::string path, serverLocation location)
	{
		struct stat	stock;
		if (stat(path.c_str(), &stock) != 0)
				return (0);
		if (location.getCgiExt() != "" && location.getCgiExt() == getExtension(path))
				return (1);
		return (0);
	}

	std::string getExtension(std::string fileName)
	{
		int i = fileName.size();
		std::string::iterator it = fileName.begin();
		std::string str = "";
		while (it != fileName.end())
			it++;
		for (; *it != '.'; it--)
		{
			i--;
			if (it == fileName.begin())
				break;
		}
		if (*it == '.')
			str = fileName.substr(i);
		else
			str = "";
		return (str);
	}

	std::string getMimeType(std::string fileName)
	{
		std::string finalRes;
		std::map<std::string, std::string> _mimeMap = tools::mime();
		std::string str = tools::getExtension(fileName);
		std::map<std::string, std::string>::iterator it2 = _mimeMap.find(str);
		if (it2 != _mimeMap.end())
			finalRes = it2->second;
		else
			finalRes = _mimeMap["unknown"];
		return (finalRes);
	}

	std::vector<std::string>	getDirAI(std::string const& path) //add path & server
	{
		std::vector<std::string> data;
		DIR *Dir;
		struct dirent *DirEntry;
		std::string newPath = path;
		Dir = opendir(newPath.c_str()); //change to path
		if (Dir == NULL)
		{
			std::cout << RED << "Could not open the directory" << RESET << std::endl;
			return (std::vector<std::string>());
		}
		while((DirEntry = readdir(Dir)) != NULL)
		{
			data.push_back(DirEntry->d_name);
		}
		closedir(Dir);
		return (data);
	}

	std::string pathEncoder(std::string path)
	{
		std::string new_path = "";
		char c;
		int ic;
		char hex_c[3];
		int len = path.length();

		for(int i=0; i < len; i++)
		{
			c = path[i];
			ic = c;

			if ( c == ' ')
				new_path += '+';
			else if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
				new_path += c;
			else
			{
				sprintf(hex_c, "%X", c);
				if(ic < 16)
					new_path += "%0";
				else
					new_path += "%";
				new_path += hex_c;
			}
		}
		return new_path;
 }

	std::string	genreateAI(std::string const& locationPath, std::string const& host, std::string const& port, std::string const& path)
	{
		std::string buff;
		std::vector<std::string> dataAI = tools::getDirAI(path);  // change to do a function generate autoindex(host, port, path,)
		if (dataAI.empty())
			std::cerr << RED << "AUTOINDEX: Could not open directory" << RESET << std::endl;

		buff =	"<!DOCTYPE html>\n<html>\n<body>\n<h1>\nAUTOINDEX</h1>\n<style>html { color-scheme: light dark; }\nbody { width: 35em; margin: left auto;\nfont-family: Tahoma, Verdana, Arial, sans-serif;\n}\n</style>\n";
		for (std::vector<std::string>::iterator it = dataAI.begin(); it != dataAI.end(); it++)
		{

			if (locationPath == "/")
				buff += "\t\t<p><a href=\"http://" +  host + ":" + port + locationPath + pathEncoder(*it) + "\">" + (*it) + "</a></p>\n";
			else
				buff += "\t\t<p><a href=\"http://" +  host + ":" + port + locationPath + "/" + pathEncoder(*it) + "\">" + (*it) + "</a></p>\n";
		}
		buff += "</body>\n</html>\n";
		return (buff);
	}

	std::string getIndex(std::vector<std::string> files)
	{
		std::vector<std::string>::iterator it;

		for (it = files.begin(); it != files.end(); it++)
		{
			size_t pos = (*it).find("index");
			if  (pos != std::string::npos)
				return(*it);
		}
		return ("");
	}

	std::map<std::string, std::string> mime()
	{
		std::map<std::string, std::string> _mimeMap;
		_mimeMap.insert(std::make_pair("unknown", "application/octet-stream"));
		_mimeMap.insert(std::make_pair(".aac", "audio/aac"));
		_mimeMap.insert(std::make_pair(".abw", "application/x-abiword"));
		_mimeMap.insert(std::make_pair(".arc", "application/octet-stream"));
		_mimeMap.insert(std::make_pair(".avi", "video/x-msvideo"));
		_mimeMap.insert(std::make_pair(".azw", "application/vnd.amazon.ebook"));
		_mimeMap.insert(std::make_pair(".bin", "application/octet-stream"));
		_mimeMap.insert(std::make_pair(".bmp", "image/bmp"));
		_mimeMap.insert(std::make_pair(".bz", "application/x-bzip"));
		_mimeMap.insert(std::make_pair(".bz2", "application/x-bzip2"));
		_mimeMap.insert(std::make_pair(".cpp", "text/x-c"));
		_mimeMap.insert(std::make_pair(".c", "text/x-c"));
		_mimeMap.insert(std::make_pair(".csh", "application/x-csh"));
		_mimeMap.insert(std::make_pair(".css", "text/css"));
		_mimeMap.insert(std::make_pair(".csv", "text/csv"));
		_mimeMap.insert(std::make_pair(".doc", "application/msword"));
		_mimeMap.insert(std::make_pair(".docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document"));
		_mimeMap.insert(std::make_pair(".eot", "application/vnd.ms-fontobject"));
		_mimeMap.insert(std::make_pair(".epub", "application/epub+zip"));
		_mimeMap.insert(std::make_pair(".gif", "image/gif"));
		_mimeMap.insert(std::make_pair(".html", "text/html"));
		_mimeMap.insert(std::make_pair(".htm", "text/html"));
		_mimeMap.insert(std::make_pair(".ico", "image/x-icon"));
		_mimeMap.insert(std::make_pair(".ics", "text/calendar"));
		_mimeMap.insert(std::make_pair(".jar", "application/java-archive"));
		_mimeMap.insert(std::make_pair(".jpeg", "image/jpeg"));
		_mimeMap.insert(std::make_pair(".jpg", "image/jpeg"));
		_mimeMap.insert(std::make_pair(".js", "application/javascript"));
		_mimeMap.insert(std::make_pair(".json", "application/json"));
		_mimeMap.insert(std::make_pair(".mid", "audio/midi"));
		_mimeMap.insert(std::make_pair(".midi", "audio/midi"));
		_mimeMap.insert(std::make_pair(".mpeg", "video/mpeg"));
		_mimeMap.insert(std::make_pair(".mpkg", "application/vnd.apple.installer+xml"));
		_mimeMap.insert(std::make_pair(".odp", "application/vnd.oasis.opendocument.presentation"));
		_mimeMap.insert(std::make_pair(".ods", "application/vnd.oasis.opendocument.spreadsheet"));
		_mimeMap.insert(std::make_pair(".odt", "application/vnd.oasis.opendocument.text"));
		_mimeMap.insert(std::make_pair(".oga", "audio/ogg"));
		_mimeMap.insert(std::make_pair(".ogv", "video/ogg"));
		_mimeMap.insert(std::make_pair(".ogx", "application/ogg"));
		_mimeMap.insert(std::make_pair(".otf", "font/otf"));
		_mimeMap.insert(std::make_pair(".png", "image/png"));
		_mimeMap.insert(std::make_pair(".pdf", "application/pdf"));
		_mimeMap.insert(std::make_pair(".php", "application/x-php"));
		_mimeMap.insert(std::make_pair(".ppt", "application/vnd.ms-powerpoint"));
		_mimeMap.insert(std::make_pair(".pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation"));
		_mimeMap.insert(std::make_pair(".py", "text/x-python"));
		_mimeMap.insert(std::make_pair(".pyc", "application/x-python-code"));
		_mimeMap.insert(std::make_pair(".rar", "application/x-rar-compressed"));
		_mimeMap.insert(std::make_pair(".rtf", "application/rtf"));
		_mimeMap.insert(std::make_pair(".sh", "application/x-sh"));
		_mimeMap.insert(std::make_pair(".svg", "image/svg+xml"));
		_mimeMap.insert(std::make_pair(".swf", "application/x-shockwave-flash"));
		_mimeMap.insert(std::make_pair(".tar", "application/x-tar"));
		_mimeMap.insert(std::make_pair(".tif", "image/tiff"));
		_mimeMap.insert(std::make_pair(".tiff", "image/tiff"));
		_mimeMap.insert(std::make_pair(".ts", "application/typescript"));
		_mimeMap.insert(std::make_pair(".ttf", "font/ttf"));
		_mimeMap.insert(std::make_pair(".txt", "text/plain"));
		_mimeMap.insert(std::make_pair(".vsd", "application/vnd.visio"));
		_mimeMap.insert(std::make_pair(".wav", "audio/x-wav"));
		_mimeMap.insert(std::make_pair(".weba", "audio/webm"));
		_mimeMap.insert(std::make_pair(".webm", "video/webm"));
		_mimeMap.insert(std::make_pair(".webp", "image/webp"));
		_mimeMap.insert(std::make_pair(".woff", "font/woff"));
		_mimeMap.insert(std::make_pair(".woff2", "font/woff2"));
		_mimeMap.insert(std::make_pair(".xhtml", "application/xhtml+xml"));
		_mimeMap.insert(std::make_pair(".xls", "application/vnd.ms-excel"));
		_mimeMap.insert(std::make_pair(".xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"));
		_mimeMap.insert(std::make_pair(".xml", "application/xml"));
		_mimeMap.insert(std::make_pair(".xul", "application/vnd.mozilla.xul+xml"));
		_mimeMap.insert(std::make_pair(".zip", "application/zip"));
		_mimeMap.insert(std::make_pair(".7z", "application/x-7z-compressed"));
			return (_mimeMap);
	}

std::string searchCorrectPath(std::string query, serverBlock block)
{
	std::string root = block.getRootServer();
	std::string path = root + query;
	struct stat s;
	std::string ret;
	std::string::iterator it = path.begin();
	int i = 0;

	while (1)
	{
		while (*it != '/' && *it != '?' && it != path.end())
		{
				it++;
				i++;
		}
		ret = path.substr(0, i);
	
		if (stat(ret.c_str(), &s) == 0)
		{
			if (S_ISDIR(s.st_mode))
			{
				it++;
				i++;
				continue ;
			}
			else
			{
				ret = path.substr(root.size(), i - root.size());
				return (ret);
			}
		}
		else
			return (query);
	}
	return (query);
}

std::string getSimplePath(std::string req, std::string *query, serverBlock block)
{
	std::string path;
	std::string root = block.getRootServer();
	struct stat s;
	int deb = 0;
	int fin;
	std::string::iterator it = req.begin();

	if (stat(root.c_str(), &s) != 0)
	{
		std::cout << RED << "Server root don't exist" << RESET << std::endl;
		*query = root;
		path = root;
		return (path);
	}
	while (*it != ' ' && it != req.end())
	{
		it++;
		deb++;
	}
	if (it == req.end())
	{
		*query = "";
		path = "";
		return (path);
	}
	it++;
	deb++;
	std::string::iterator it2 = it;
	fin = deb;
	while (*it2 && *it2 != ' ')
	{
		it2++;
		fin++;
	}
	*query = req.substr(deb, fin - deb);
	path = root + *query;
	if (stat(path.c_str(), &s) == 0)
		path = *query;
	else
		path = searchCorrectPath(*query, block);
	return (path);
}

serverLocation	whichLocation(std::string simple, serverBlock block)
{
	serverLocation loc;
	loc = searchLocation(simple, block);
	return (loc);
}

std::vector<std::string> getBodyBoundary(std::string body, std::string boundary)
{
	std::vector<std::string> ret;
	std::string stock = body;
	std::string temp;
	int find;
	int lenBody;
	while (1)
	{
		find = body.find(boundary);
		temp = body.substr(0, find);
		if (body.size() <= boundary.size())
			break;
		if (temp != "")
		{
			ret.push_back(temp);
			lenBody = body.size();
			body = body.substr(find, lenBody - find);
		}
		else
		{
			lenBody = body.size();
			if (lenBody - boundary.size() <= boundary.size())
					break;
			body = body.substr(boundary.size(), lenBody - boundary.size());
		}
		if (body.size() <= boundary.size())
			break;

	}
	return (ret);
}

}
