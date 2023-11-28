#include "./WebServer.hpp"

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;
    WebServer server = WebServer();
    server.startServer();
    return 0;
}