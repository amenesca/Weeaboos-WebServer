#ifndef COMMON_H
#define COMMON_H

#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <string>
#include <iostream>
#include <cctype>
#include <fstream>

#define SERVER_PORT 18000

#define MAXLINE 4096
#define SA struct sockaddr

void err_n_die(const char *fmt, ...);
char *bin2hex(const unsigned char *input, size_t len);
std::string trim(const std::string& str);

#endif
