#include "common.h"

int main(int argc, char **argv) {
    
    int                 listenfd, connfd, n;
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
        struct sockaddr_in addr;
        socklen_t addr_len;

        //accept blocks until an incoming connection arrives
        //it returns a "file descriptor" to the connection
        printf("waiting for a connection on port %d\n", SERVER_PORT);
        fflush(stdout);
        connfd = accept(listenfd, (SA *)NULL, NULL);

        //zero out the receive buffer to make sure it ends up null terminated
        memset(recvline, 0, MAXLINE);
        //Now read the client's message.
        while((n = read(connfd, recvline, MAXLINE-1)) > 0)
        {
            fprintf(stdout, "\n%s\n\n%s", bin2hex(recvline, n),recvline);

            //hacky way to detect the end of message.
            if (recvline[n-1] == '\n') {
                break;
            }
            memset(recvline,0,MAXLINE);
        }
        if (n<0)
            err_n_die("read error");

        //now send a response
        snprintf((char*)buff, sizeof(buff), "HTTP/1.0 200 OK\r\n\r\nHELLO");

        //note: normally, you may want to chech the results from write and close
        // in case errors occur. For now. I'm ignoring them.
        write(connfd, (char*)buff, strlen((char*)buff));
        close(connfd);
    }
}