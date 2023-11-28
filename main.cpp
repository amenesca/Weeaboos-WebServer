#include "./WebServer.hpp"

int main(int argc, char *argv[]) {
    WebServer server = WebServer();
    server.startServer();
    return 0;
}