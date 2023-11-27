#include "./WebServer.hpp"

int main(int argc, char *argv[]) {
    http::WebServer server = http::WebServer(0, 8080);
    return 0;
}