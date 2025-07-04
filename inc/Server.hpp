#ifndef SERVER_HPP
# define SERVER_HPP

# include <cstring>
# include <iostream>
# include <netinet/in.h>
# include <sys/socket.h>
# include <unistd.h>
# include <fcntl.h>
# include <csignal>
# include <cstdlib>

extern int g_signal;

class Server {
    private:
        int         _port;
        std::string _password;
        int         _serverSocket;

        void    setupServerSocket(void);
        void    runMainLoop(void);
    public:
        Server(int port, const std::string &password);
        ~Server(void);

        void    start(void);
        void    stop(void);
};

#endif