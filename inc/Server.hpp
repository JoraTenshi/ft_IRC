#ifndef SERVER_HPP
# define SERVER_HPP

# include "User.hpp"
# include <cstring>
# include <iostream>
# include <netinet/in.h>
# include <sys/socket.h>
# include <unistd.h>
# include <fcntl.h>
# include <csignal>
# include <cstdlib>
# include <vector>
# include <poll.h>
# include <map>
# include <arpa/inet.h>

extern int g_signal;

class Server {
    private:
        int                         _port;
        std::string                 _password;
        int                         _serverSocket;
        std::vector<struct pollfd>  _pollFds;
        std::map<int, User>       _users;

        void    setupServerSocket(void);
        void    runMainLoop(void);
        void    newConnection(void);
        void    newUser(int userSocket, const std::string &hostname);
        void    checkUpdate(User &user);
        //void    disconnectUser(User &user);
    public:
        Server(int port, const std::string &password);
        ~Server(void);

        void    start(void);
        void    stop(void);
};

#endif