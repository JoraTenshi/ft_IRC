#include "../../inc/Server.hpp"

/**
 * @brief This handles the invites to channels, checking if the user is allowed to invite others and if the channel exists.
 * 
 * @param user 
 */
void Server::InviteCmd(User &user)
{
    std::string response = "";

    //Server response if user does not input arguments
    if (user.getMessage().getArgs().size() < 2)
    {
        //ERR_NEEDMOREPARAMS
        response = ":" + user.getHostname() + " 461 " + user.getNickname() + " INVITE :Not enough parameters\r\n";
        send(user.getFd(), response.c_str(), response.size(), 0);
        std::cout << "[ SERVER ] Message sent to client " << user.getFd() << "( " + user.getHostname() + " )" << std::endl;
        return;
    }

    //Server response if user already in channel
    for (std::vector<User>::iterator it = _channels[user.getMessage().getArgs()[0]].getUsers().begin(); it != _channels[user.getMessage().getArgs()[0]].getUsers().end(); it++)
    {
        if (it->getNickname() == user.getMessage().getArgs()[0])
        {
            //ERR_USERONCHANNEL
            response = ":" + user.getHostname() + " 443 " + user.getNickname() + " " + user.getMessage().getArgs()[0] + " :User already in channel\r\n";
            send(user.getFd(), response.c_str(), response.size(), 0);
            std::cout << "[ SERVER ] Message sent to client " << user.getFd() << "( " + user.getHostname() + " )" << std::endl;
            return;
        }
    }

    //Server response if that channel does not exist
    if (_channels.find(user.getMessage().getArgs()[0]) == _channels.end())
    {
        //ERR_NOSUCHCHANNEL
        response = ":" + user.getHostname() + " 403 " + user.getNickname() + " " + user.getMessage().getArgs()[0] + " :No such channel\r\n";
        send(user.getFd(), response.c_str(), response.size(), 0);
        std::cout << "[ SERVER ] Message sent to client " << user.getFd() << "( " + user.getHostname() + " )" << std::endl;
        return;
    }

    //Server response if user is not an operator of the channel
    if (std::find(_channels[user.getMessage().getArgs()[0]].getOps().begin(), _channels[user.getMessage().getArgs()[0]].getOps().end(),
        user) == _channels[user.getMessage().getArgs()[0]].getOps().end())
    {
        //ERR_CHANOPRIVSNEEDED
        response = ":" + user.getHostname() + " 482 " + user.getNickname() + " " + _channels[user.getMessage().getArgs()[0]].getName() + " :You're not channel operator\r\n";
        send(user.getFd(), response.c_str(), response.size(), 0);
        std::cout << "[ SERVER ] Message sent to client " << user.getFd() << "( " + user.getHostname() + " )" << std::endl;
        return;
    }

    //Server response if user is already in the channel
    if (std::find(_channels[user.getMessage().getArgs()[0]].getUsers().begin(), _channels[user.getMessage().getArgs()[0]].getUsers().end(),
        user) != _channels[user.getMessage().getArgs()[0]].getUsers().end())
    {
        //ERR_USERONCHANNEL
        response = ":" + user.getHostname() + " 443 " + user.getNickname() + " " + _channels[user.getMessage().getArgs()[0]].getName() + " :User already in channel\r\n";
        send(user.getFd(), response.c_str(), response.size(), 0);
        std::cout << "[ SERVER ] Message sent to client " << user.getFd() << "( " + user.getHostname() + " )" << std::endl;
        return;
    }

    //RPL_INVITING
    response = ":" + user.getHostname() + " 341 " + user.getNickname() + " " + user.getMessage().getArgs()[0] + " " + _channels[user.getMessage().getArgs()[0]].getName() + "\r\n";
    send(user.getFd(), response.c_str(), response.size(), 0);
    std::cout << "[ SERVER ] Message sent to client " << user.getFd() << "( " + user.getHostname() + " )" << std::endl;
    for (std::map<int, User>::iterator it = _users.begin(); it != _users.end(); it++)
    {
       if (it->second.getNickname() == user.getMessage().getArgs()[0])
        {
            std::string inviteResponse = ":" + user.getHostname() + " INVITE " + user.getMessage().getArgs()[0] + " :" + _channels[user.getMessage().getArgs()[0]].getName() + "\r\n";
            send(it->second.getFd(), inviteResponse.c_str(), inviteResponse.size(), 0);
            std::cout << "[ SERVER ] Invite message sent to client " << it->second.getFd() << "( " + it->second.getHostname() + " )" << std::endl;
            return;
        }
    }
}