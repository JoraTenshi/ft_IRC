#include "../../inc/Server.hpp"

/**
 * @brief This handles quitting the server, sending a message to all users in the channels the user is part of.
 * 
 * @param user 
 */
void Server::QuitCmd(User &user) {
    std::string quitMsg = ":" + user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname() + " QUIT";
    std::string response = "";

    if (!user.getMessage().getMsg().empty())
        quitMsg += " :" + user.getMessage().getMsg();
    else
        quitMsg += " :Client quit";
    quitMsg += "\r\n";

    std::vector<Channel *> channels = findChannels(user);
    for (size_t i = 0; i < channels.size(); ++i) 
    {
        Channel* channel = channels[i];
        std::vector<User> chUsers = channel->getUsers();
        for (std::vector<User>::iterator userIt = chUsers.begin(); userIt != chUsers.end(); ++userIt) {
            if (userIt->getFd() != user.getFd()) {
                send(userIt->getFd(), quitMsg.c_str(), quitMsg.length(), 0);
                std::cout << "[ SERVER ] Message sent to client " << userIt->getFd() << "( " << userIt->getHostname() << " )" << quitMsg;
            }
        }
        /* if (channel->getOps().size() == 0) {
            channel->getOps().push_back(channel->getUsers()[0]);
            std::vector<User> chOps = channel->getOps();
            chOps.push_back(channel->getUsers()[0]);
            channel->setOps(chOps);
            for (std::vector<User>::iterator it = channel->getUsers().begin(); it != channel->getUsers().end(); ++it) {
                response = ":" + user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname() + " MODE " + channel->getName() + " +o "
                + channel->getUsers()[0].getNickname() + "\r\n";
                send(it->getFd(), response.c_str(), response.size(), 0);
                std::cout << "[ SERVER ] Message sent to client " << it->getFd() << " ( " << it->getHostname() << " )" << response;
            }
        } */
    }
    
    disconnectUser(user);
}