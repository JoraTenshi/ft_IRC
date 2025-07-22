#ifndef SERVER_HPP
# define SERVER_HPP

# include "Channel.hpp"
# include <cstring>
# include <iostream>
# include <netinet/in.h>
# include <sys/socket.h>
# include <unistd.h>
# include <fcntl.h>
# include <csignal>
# include <cstdlib>
# include <algorithm>
# include <vector>
# include <poll.h>
# include <map>
# include <arpa/inet.h>

extern int g_signal;

class Channel;
class Server {
    private:
        int                        		_port;
        std::string                 	_password;
        int                         	_serverSocket;
        std::vector<struct pollfd>  	_pollFds;
        std::map<int, User>       		_users;
        std::map<std::string, Channel> 	_channels;

    public:
        Server(int port, const std::string &password);
        ~Server(void);
        
        void    start(void);
        void    stop(void);
        void    executeCommand(User &user);
        
        void    setupServerSocket(void);
        void    runMainLoop(void);
        void    newConnection(void);
        void    newUser(int userSocket, const std::string &hostname);
        void    checkUpdate(User &user);
        void    disconnectUser(User &user);
        std::vector<Channel *> findChannels(User &aux);
        void    rmrInvited(User &user);

        void    PassCmd(User &user);
        void    NickCmd(User &user);
        void    UserCmd(User &user);
        void    PingCmd(User &user);
        void    JoinCmd(User &user);
        void    PartCmd(User &user);
        void    PrivMsgCmd(User &user);
        void    ModeCmd(User &user);
        void    TopicCmd(User &user);
        void    InviteCmd(User &user);
};

#endif