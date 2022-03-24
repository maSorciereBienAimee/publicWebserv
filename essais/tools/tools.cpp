#include "tools.hpp"
#include "../parseConfig/serverLocation.hpp"
namespace tools
{

	void    printVector(std::vector<std::string> content)
	{
		std::vector<std::string>::iterator it;
		std::cout << "_____CONTENT IS______\n";
		for (it = content.begin(); it != content.end(); it++)
			std::cout << (*it) << "\n";
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
        || dir == "cgi_extension" || dir == "cgi_bin")
            return (true);
        return (false);
    }
    bool isValidDirectiveLocation(std::string const &dir)
    {
        if (dir == "index" || dir == "methods" || dir == "cgi_extension"
        || dir == "cgi_bin" || dir == "language" || dir == "autoindex" || dir == "root"
        || dir == "client_max_body_size" || dir == "auth_basic" || dir == "auth_basic_user_file" || dir == "redirection")
            return (true);
        return (false);
    }

	int	isItCgi(std::string path, std::vector<serverLocation> location)
	{
		int i = path.size();
		std::string str = path;
		std::string ext = tools::getExtension(path);
		std::string newPath = "";
		std::string withoutExt = path;
		if (ext != "")
		{
			std::string::iterator it = str.begin();
			while (it != str.end())
				it++;
			for (; *it != '/'; it--)
			{
				i--;
				if (it == str.begin())
					break;
			}
			str.erase(it, str.end());
			withoutExt = str;
			if (withoutExt == "")
					withoutExt = "/";
			newPath = str + "/" + "*" + ext;
		}
		for (std::vector<serverLocation>::iterator it = location.begin(); it != location.end(); it++)
		{
			if ((*it).getLocationPath() == newPath && (*it).getCgiExt() == ext)
					return (1);
		}
		for (std::vector<serverLocation>::iterator it = location.begin(); it != location.end(); it++)
		{
			if ((*it).getLocationPath() == withoutExt && (*it).getCgiExt() == ext)
					return (1);
		}
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
		std::cout << "final = "<< finalRes << std::endl;
		return (finalRes);
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

}
