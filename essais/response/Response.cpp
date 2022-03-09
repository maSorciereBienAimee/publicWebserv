#include "Response.hpp"

Response::Response(void) {}

Response::Response(Request R, Config C, int F) : request(R), fd(F), config(C)
{
	initDefaultError();
}

Response::~Response() {}

Response::launch()
{
	std::reponse;

	if (this->request.method == "GET")
		reponse = responseGet();
	if (this->request.method == "DELETE")
		reponse = responseGet();
	if (this->request.method == "POST")
		reponse = responseGet();
	send(fd, reponse, strlen(reponse), 0);
}

void getHeader(int size)
{
	std::stringstream ss;
	ss << length;
	std::string lenstr= ss.str();
	
	std::string header = "HTTP/1.1 " + this->errors[this->status] + "\n";
   header += "Content-Type: " + this->type + "\n"; // ?? ou recuperer
	header += "Content-Length: " + lenstr;
}

int getBody()
{
	std::ifstream is (this->request.getPath(), std::ifstream::binary);
	if (!is)
	{
		this->status = 400; // ou selon l'erreur
		this->body = "";
		return (-1);
	}
	is.seekg (0, is.end);
	int length = is.tellg();
	is.seekg (0, is.beg);
	char * b = (char *)malloc(sizeof(char) * (length + 1));
	b[length] = '\0';
	is.read (b,length);
	std::string bufStr(b);
	this->body = bufstr;
	return (length)	
}

std::string responseGet()
{
	std::string response;
	int sizeBody = getBody();
	getHeaders(sizeBody);
	response = this->header + "\r\n\r\n" + this->body;
	return (response);
}


void initErrors()
{
	errors[100] = "100 Continue";
	errors[101] = "101 Switching Protocols";
	errors[103] = "103 Early Hints";
	errors[200] = "200 OK";
	errors[201] = "201 Created";
	errors[202] = "202 Accepted";
	errors[203] = "203 Non-Authoritative Information";
	errors[204] = "204 No Content";
	errors[205] = "205 Reset Content";
	errors[206] = "206 Partial Content";
	errors[300] = "300 Multiple Choices";
	errors[301] = "301 Moved Permanently";
	errors[302] = "302 Found";
	errors[303] = "303 See Other";
	errors[304] = "304 Not Modified";
	errors[307] = "307 Temporary Redirect";
	errors[308] = "308 Permanent Redirect";
	errors[400] = "400 Bad Request";
	errors[401] = "401 Unauthorized";
	errors[402] = "402 Payment Required";
	errors[403] = "403 Forbidden";
	errors[404] = "404 Not Found";
	errors[405] = "405 Method Not Allowed";
	errors[406] = "406 Not Acceptable";
	errors[407] = "407 Proxy Authentication Required";
	errors[408] = "408 Request Timeout";
	errors[409] = "409 Conflict";
	errors[410] = "410 Gone";
	errors[411] = "411 Length Required";
	errors[412] = "412 Precondition Failed";
	errors[413] = "413 Payload Too Large";
	errors[414] = "414 URI Too Long";
	errors[415] = "415 Unsupported Media Type";
	errors[416] = "416 Range Not Satisfiable";
	errors[417] = "417 Expectation Failed";
	errors[418] = "418 I'm a teapot";
	errors[422] = "422 Unprocessable Entity";
	errors[425] = "425 Too Early";
	errors[426] = "426 Upgrade Required";
	errors[428] = "428 Precondition Required";
	errors[429] = "429 Too Many Requests";
	errors[431] = "431 Request Header Fields Too Large";
	errors[451] = "451 Unavailable For Legal Reasons";
	errors[500] = "500 Internal Server Error";
	errors[501] = "501 Not Implemented";
	errors[502] = "502 Bad Gateway";
	errors[503] = "503 Service Unavailable";
	errors[504] = "504 Gateway Timeout";
	errors[505] = "505 HTTP Version Not Supported";
	errors[506] = "506 Variant Also Negotiates";
	errors[507] = "507 Insufficient Storage";
	errors[508] = "508 Loop Detected";
	errors[510] = "510 Not Extended";
	errors[511] = "511 Network Authentication Required";
}


