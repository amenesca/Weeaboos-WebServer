#include "common.hpp"
#include <sstream>

int main(int argc, char **argv) {
	int 				listenfd, connfd, bytes_read;
	int 				messagesize = 0;
	struct sockaddr_in 	serveraddr;
	char 				buff[MAXLINE+1];
	char 				recvline[MAXLINE+1];

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVER_PORT);

	bind(listenfd, (SA *)&serveraddr, sizeof(serveraddr));

	listen(listenfd, 10);

	for ( ; ; ) {
		struct sockaddr_in client_addr;
		socklen_t addr_len;
		char client_address[MAXLINE + 1];

		//accept blocks until an incoming connection arrives
		//it returns a "file descriptor" to the connection
		printf("waiting for a connection on port %d\n", SERVER_PORT);
		fflush(stdout);
		connfd = accept(listenfd, (SA *)&client_addr, &addr_len);

		inet_ntop(AF_INET, &client_addr, client_address, MAXLINE);
		printf("Client connection: %s\n", client_address);
		fflush(stdout);
		//zero out the receive buffer to make sure it ends up null terminated
		memset(recvline, 0, MAXLINE);
		//Now read the client's message.
		while((bytes_read = read(connfd, recvline+messagesize, sizeof(recvline)-messagesize-1)) > 0)
		{
			messagesize += bytes_read;
			//hacky way to detect the end of message.
			if (recvline[bytes_read-1] == '\n' || messagesize > MAXLINE-1) {
				break;
			}
		}
		if (bytes_read<0)
			exit(1);
		recvline[messagesize-1] = 0;
		printf("REQUEST: %s\n", recvline);
		fflush(stdout);

		// Open the file of the request
		std::string convert_char_pointer(recvline);
		std::string request_trim = trim(convert_char_pointer);
		std::ifstream request_file(request_trim);

		std::stringstream request_read;
		request_read << request_file.rdbuf();

		request_file.close();

		std::string request_str = request_read.str(); 

		//note: normally, you may want to chech the results from write and close
		// in case errors occur. For now. I'm ignoring them.
		write(connfd, request_str.c_str(), strlen((char*)request_str.c_str()));
		close(connfd);
		memset(recvline,0,MAXLINE);
		messagesize = 0;
		bytes_read = 0;
  }
}
