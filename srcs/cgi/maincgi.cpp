#include "cgiHandler.hpp"
#include <sys/stat.h>

int main (int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;
    cgiHandler cgi;
    int fd = open("./navegador", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1)
    {
        std::cerr << "Error opening file: " << strerror(errno) << std::endl;
        return 1;  // Return an error code to indicate failure
    }
    cgi.configCgi(fd, envp);
    close(fd);
    if (chmod("../../cgi-bin/index.py", 0755) == -1)
    {
        std::cerr << "Error changing file permissions: " << strerror(errno) << std::endl;
        return 1;  // Return an error code to indicate failure
    }
    return(0);
}