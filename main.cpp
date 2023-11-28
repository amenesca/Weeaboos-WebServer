#include "./WebServer.hpp"

int main(int argc, char *argv[]) {
    http::WebServer server = http::WebServer();
    server.startServer();
    return 0;
}