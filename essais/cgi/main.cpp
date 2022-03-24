#include "Cgi.hpp"

int main(int argc, char **argv)
{
	Cgi myCgi;
	if (argc > 1)
	{
		std::string str(argv[1]);
		myCgi.getMimeType(str);
	}
}
