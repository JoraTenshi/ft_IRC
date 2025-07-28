#include "../../inc/Server.hpp"

/**
 * @brief This handles quitting the server, sending a message to all users in the channels the user is part of.
 * 
 * @param user 
 */
void Server::QuitCmd(User &user) {
    std::string quitMsg = ":" + user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname() + " QUIT";
    if (!user.getMessage().getMsg().empty())
        quitMsg += " :" + user.getMessage().getMsg();
    else
        quitMsg += " :Client quit";
    quitMsg += "\r\n";

    std::vector<Channel *> channels = findChannels(user);
    for (size_t i = 0; i < channels.size(); ++i) {
        std::vector<User> chUsers = channels[i]->getUsers();
        for (std::vector<User>::iterator userIt = chUsers.begin(); userIt != chUsers.end(); ++userIt) {
            if (userIt->getFd() != user.getFd()) {
                send(userIt->getFd(), quitMsg.c_str(), quitMsg.length(), 0);
                std::cout << " [ SERVER ] Message sent to client " << userIt->getFd() << "( " << userIt->getHostname() << " )" << quitMsg;
            }
        }
    }

    disconnectUser(user);
}