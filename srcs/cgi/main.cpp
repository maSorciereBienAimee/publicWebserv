//#include "Cgi.hpp"
#include <iostream>
#include <string>

std::string getPathWithoutQuery(std::string str, std::string ext)
{
	std::string temp = "";
	std::string::iterator it = str.begin();
	std::string::iterator it2;
	int i = 0;
	int j = 0;
	for (; it != str.end(); it++)
	{
		temp = str.substr(i, ext.size());
		i++;
		if (temp == ext)
		{
			j = 0;
			while (j < ext.size() - 1)
			{
				it++;
				i++;
				j++;
			}
			break;
		}
	}
	it2 = it;
	j = i;
	for (; it2 != str.end(); it2++)
	{
		if (*it2 == '?')
			break;
		j++;
	}
	j--;
	temp = str.substr(0, j);
	if (temp == "/")
			temp = "";
	return (temp);
}

std::string getPathWithoutPathInfo(std::string str, std::string ext)
{
	std::string temp = "";
	std::string::iterator it = str.begin();
	int i = 0;
	int j = 0;
	for (; it != str.end(); it++)
	{
		temp = str.substr(i, ext.size());
		i++;
		if (temp == ext)
		{
			j = 0;
			while (j < ext.size() - 1)
			{
				it++;
				i++;
				j++;
			}
			break;
		}
	}
	temp = str.substr(0, i);
	if (temp == "/")
			temp = "";
	return (temp);
}

std::string getQuerry(std::string str)
{
	int res;
	std::string result = "";
	res = str.find('?');
	if (res != std::string::npos)
	{
		result = str.substr(res + 1, str.size() - res);
	}
	return (result);
}

std::string getPathInfo(std::string str, std::string ext)
{
	std::string temp = "";
	std::string::iterator it = str.begin();
	std::string::iterator it2;
	int i = 0;
	int j = 0;
	for (; it != str.end(); it++)
	{
		temp = str.substr(i, ext.size());
		i++;
		if (temp == ext)
		{
			j = 0;
			while (j < ext.size() - 1)
			{
				it++;
				i++;
				j++;
			}
			break;
		}
	}
	it2 = it;
	j = 0;
	for (; it2 != str.end(); it2++)
	{
		if (*it2 == '?')
		{
			j--;
			break;
		}
		j++;
	}
	temp = str.substr(i, j);
	if (temp == "/")
			temp = "";
	return (temp);

}

int main(int argc, char **argv)
{
//	Cgi myCgi;
//	if (argc > 1)
//	{
//		std::string str(argv[1]);
//		myCgi.getMimeType(str);
//	}
		std::string res;
		std::string res1;
		std::string res2;
		std::string res3;

		std::cout <<std::endl << "http://www.example.com/php/path_info.php/some/stuff?foo=bar" << std::endl;
		res = getPathInfo("http://www.example.com/php/path_info.php/some/stuff?foo=bar", ".php");
		res1 = getPathWithoutQuery("http://www.example.com/php/path_info.php/some/stuff?foo=bar", ".php");
		res2 = getPathWithoutPathInfo("http://www.example.com/php/path_info.php/some/stuff?foo=bar", ".php");
		res3 = getQuerry("http://www.example.com/php/path_info.php/some/stuff?foo=bar");
	std::cout << res << std::endl;
	std::cout << res1 << std::endl;
	std::cout << res2 << std::endl;
	std::cout << res3 << std::endl;

	std::cout <<std::endl <<"http://www.example.com/php/path_info.php" << std::endl;
		res = getPathInfo("http://www.example.com/php/path_info.php", ".php");
		res1 = getPathWithoutQuery("http://www.example.com/php/path_info.php", ".php");
		res2 = getPathWithoutPathInfo("http://www.example.com/php/path_info.php", ".php");
		res3 = getQuerry("http://www.example.com/php/path_info.php" );
	std::cout << res << std::endl;
	std::cout << res1 << std::endl;
	std::cout << res2 << std::endl;
	std::cout << res3 << std::endl;

	std::cout <<std::endl <<"http://www.example.com/php/path_info.php/"<< std::endl;
		res = getPathInfo("http://www.example.com/php/path_info.php/", ".php");
		res1 = getPathWithoutQuery("http://www.example.com/php/path_info.php/", ".php");
		res2 = getPathWithoutPathInfo("http://www.example.com/php/path_info.php/", ".php");
		res3 = getQuerry("http://www.example.com/php/path_info.php/");
	std::cout << res << std::endl;
	std::cout << res1 << std::endl;
	std::cout << res2 << std::endl;
	std::cout << res3 << std::endl;

	std::cout <<std::endl << "http://www.example.com/php/path_info.php/some/stuff" << std::endl;
		res = getPathInfo("http://www.example.com/php/path_info.php/some/stuff", ".php");
		res1 = getPathWithoutQuery("http://www.example.com/php/path_info.php/some/stuff", ".php");
		res2 = getPathWithoutPathInfo("http://www.example.com/php/path_info.php/some/stuff", ".php");
		res3 = getQuerry("http://www.example.com/php/path_info.php/some/stuff");
	std::cout << res << std::endl;
	std::cout << res1 << std::endl;
	std::cout << res2 << std::endl;
	std::cout << res3 << std::endl;
}
