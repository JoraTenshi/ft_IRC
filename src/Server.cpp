#include "../inc/Server.hpp"

Server::Server(int port, const std::string &password)
    : _port(port), _password(password), _serverSocket(-1) {}

Server::~Server(void) {
    stop();
}

void    Server::start(void) {
    setupServerSocket();
}

void    Server::stop(void) {
    if (_serverSocket != -1) {
        close(_serverSocket);
        _serverSocket = -1;
    }

    std::cout << "Server stopped." << std::endl;
}