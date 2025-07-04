#include "../inc/Server.hpp"

Server::Server(int port, const std::string &password)
    : _port(port), _password(password), _serverSocket(-1) {}

Server::~Server(void) {
    stop();
}

void    Server::start(void) {
    setupServerSocket();
    runMainLoop();
}

void    Server::stop(void) {
    if (_serverSocket != -1) {
        close(_serverSocket);
        _serverSocket = -1;
    }

    std::cout << "Server stopped." << std::endl;
}

void    Server::setupServerSocket(void) {
    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverSocket < 0)
        throw std::runtime_error("Failed to create socket");

    int opt = 1;
    if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        close(_serverSocket);
        throw std::runtime_error("Failed to set socket options");
    }

    if (fcntl(_serverSocket, F_SETFL, O_NONBLOCK) < 0) {
        close(_serverSocket);
        throw std::runtime_error("Failed to set non-blocking socket");
    }

    struct sockaddr_in  serverAddress;
    std::memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(_port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(_serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress))) {
        close(_serverSocket);
        throw std::runtime_error("Failed to bind socket");
    }

    if (listen(_serverSocket, 10) < 0) {
        close(_serverSocket);
        throw std::runtime_error("Failed to listen on socket");
    }

    std::cout << "Server listening on port " << _port << std::endl;
}

void    Server::runMainLoop(void) {
    while (1) {
        if (g_signal == SIGINT) {
            std::cout << "\nShutting down server..." << std::endl;
            stop();
            exit(0);
        }
    }
}