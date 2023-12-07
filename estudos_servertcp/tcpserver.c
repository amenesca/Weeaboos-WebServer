#include "common.h"

int main(int argc, char **argv) {
    
    int                 listenfd, connfd, bytes_read;
    int                 messagesize = 0;
    struct sockaddr_in  serveraddr;
    uint8_t             buff[MAXLINE+1];
    uint8_t             recvline[MAXLINE+1];

    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_n_die("socket error.");

    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVER_PORT);

    if((bind(listenfd, (SA *)&serveraddr, sizeof(serveraddr))) < 0)
        err_n_die("bind error.");

    if((listen(listenfd, 10)) < 0)
        err_n_die("listen error.");

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
            err_n_die("read error");
        recvline[messagesize-1] = 0;
        printf("REQUEST: %s\n", recvline);
        fflush(stdout);
        //now send a response
        snprintf((char*)buff, sizeof(buff), "HTTP/1.0 200 OK\r\n\r\nHELLO");

        //note: normally, you may want to chech the results from write and close
        // in case errors occur. For now. I'm ignoring them.
        write(connfd, (char*)buff, strlen((char*)buff));
        close(connfd);
        memset(recvline,0,MAXLINE);
        messagesize = 0;
        bytes_read = 0;
  }
}
