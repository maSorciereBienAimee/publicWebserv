#include <iostream>

std::string Chunk_decoder(std::string str)
{
    //FROM BEGINNING OF REQUEST UNTIL THE END OF FIRST EMPTY LINE
	std::string	head = str.substr(0,str.find("\r\n\r\n") + 4);
    //FROM AFTER NEW LINE TO END OF MESSAGE
	std::string	coded = str.substr(str.find("\r\n\r\n") + 4, str.length() - 1);
	
    std::string	subchunk = coded;
	std::string	body;
	// GET CHUNK SIZE FROM BASE 16 
    int			chunksize = strtol(coded.c_str(), NULL, 16);
	size_t		i = 0;

	while (chunksize)
	{
        //FIND END OF INITAL CHUNKSIZE LABEL
		i = coded.find("\r\n", i) + 2;
        // SKIP PAST INITIAL SIZE \R\N LABEL
		body += coded.substr(i, chunksize);
		//SIZE ADDED IS CHUNKSIZE + THE \R\N AT END WHICH ISN'T INCLUDED IN CHUNKSIZE
        i += chunksize + 2;
		//TAKE NEXT 100
        subchunk = coded.substr(i, coded.length() - i);
		//GET SIZE OF NEXT CHUNK
        chunksize = strtol(subchunk.c_str(), NULL, 16);
	}
    std::cout << "MY BODY IS" << body << std::endl;
	std::string decoded = head + body + "\r\n\r\n";
    return (decoded);
}

int main(void)
{
    std::string test1 = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nTransfer-Encoding: chunked\r\n\r\n7\r\nMozilla\r\n9\r\nDeveloper\r\n7\r\nNetwork\r\n0\r\n\r\n";
    std::string test2 = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nTransfer-Encoding: chunked\r\n\r\n4\r\nWiki\r\n6\r\npedia \r\nE\r\nin \r\n\r\nchunks.\r\n0\r\n\r\n";
    
    Chunk_decoder(test1);
    Chunk_decoder(test2);

    return 0;

}

