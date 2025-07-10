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

void signalHandler(int signum) { g_signal = signum; }

void    Server::runMainLoop(void) {
    pollfd  serverPoll;
    serverPoll.fd = _serverSocket;
    serverPoll.events = POLLIN;
    serverPoll.revents = 0;
    _pollFds.push_back(serverPoll);

    std::signal(SIGINT, signalHandler);   
 
    while (!g_signal) {
        if (poll(&_pollFds[0], _pollFds.size(), -1) < 0 && g_signal == 0)
            throw std::runtime_error("Poll failed");
        
        for (size_t i = 0; i < _pollFds.size(); ++i) {
            if (_pollFds[i].revents == POLLIN) {
                if (_pollFds[i].fd == _serverSocket)
                    newConnection();
                else
                {
                    std::cout << "Checking updates for user with fd: " << _pollFds[i].fd << std::endl;
                    checkUpdate(_users[_pollFds[i].fd]);
                }
            }
        }
    }

    if (g_signal == SIGINT) {
        std::cout << "\nShutting down server..." << std::endl;
        stop();
        exit(0);
    }
}

void    Server::newConnection(void) {
    struct sockaddr_in userAddress;
    socklen_t userLen = sizeof(userAddress);

    int userSocket = accept(_serverSocket, (struct sockaddr*)&userAddress, &userLen);
    if (userSocket < 0)
        exit(-1);
    if (fcntl(userSocket, F_SETFL, O_NONBLOCK) < 0) {
        close(userSocket);
        exit(-1);
    }

    newUser(userSocket, inet_ntoa(userAddress.sin_addr));

    std::cout << "New client connected: " << inet_ntoa(userAddress.sin_addr) 
              << ":" << ntohs(userAddress.sin_port) << " (fd: " << userSocket << ")" << std::endl;
}

void    Server::newUser(int userSocket, const std::string &hostname) {
    User user(userSocket);
    user.setHostname(hostname);
    
    _users[userSocket] = user;

    pollfd userPoll;
    userPoll.fd = userSocket;
    userPoll.events = POLLIN;
    userPoll.revents = 0;
    _pollFds.push_back(userPoll);
}

void    Server::checkUpdate(User &user) {
    char    buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    ssize_t bytes = recv(user.getFd(), buffer, sizeof(buffer) - 1, 0);
    if (bytes < 0)
        exit(-1);
    if (bytes == 0)
        disconnectUser(user);
    if (bytes > 0) {
        std::cout << "Received " << bytes << " bytes from user with fd: " << user.getFd() << std::endl;
        std::string message(buffer);
        user.getMessage().setInput(user.getMessage().getInput() + message);
        if (user.getMessage().checkCmdEnd() == false) {
            std::cout << "Incomplete command, waiting for more data..." << std::endl;
            return ;
        }
        user.getMessage().parseInput();
        executeCommand(user);
        user.getMessage().clear();
    }
}

void    Server::disconnectUser(User &user) {
    User aux = user;

    for (std::vector<pollfd>::iterator it = _pollFds.begin(); it != _pollFds.end(); ++it) {
        if (aux.getFd() == it->fd) {
            std::cout << "Client " << it->fd << " disconnected." << std::endl;
            _pollFds.erase(it);
            _users.erase(aux.getFd());
            close(aux.getFd());
            break;
        }
    }
}
