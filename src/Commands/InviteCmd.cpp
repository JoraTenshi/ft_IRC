#include "../../inc/Server.hpp"

void Server::InviteCmd(User &user)
{
    std::string response = "";

    if (user.getMessage().getArgs().size() < 2)
    {
        response = ":" + user.getHostname() + " 461 " + user.getNickname() + " INVITE :Not enough parameters\r\n";
        send(user.getFd(), response.c_str(), response.size(), 0);
        std::cout << "[ SERVER ] Message sent to client " << user.getFd() << "( " + user.getHostname() + " )" << std::endl;
        return;
    }

    for (std::vector<User>::iterator it = _channels[user.getMessage().getArgs()[0]].getUsers().begin(); it != _channels[user.getMessage().getArgs()[0]].getUsers().end(); it++)
    {
        if (it->getNickname() == user.getMessage().getArgs()[0])
        {
            response = ":" + user.getHostname() + " 403 " + user.getNickname() + " " + user.getMessage().getArgs()[0] + " :User already in channel\r\n";
            send(user.getFd(), response.c_str(), response.size(), 0);
            std::cout << "[ SERVER ] Message sent to client " << user.getFd() << "( " + user.getHostname() + " )" << std::endl;
            return;
        }
    }

    if (_channels.find(user.getMessage().getArgs()[0]) == _channels.end())
    {
        response = ":" + user.getHostname() + " 442 " + user.getNickname() + " " + user.getMessage().getArgs()[0] + " :No such channel\r\n";
        send(user.getFd(), response.c_str(), response.size(), 0);
        std::cout << "[ SERVER ] Message sent to client " << user.getFd() << "( " + user.getHostname() + " )" << std::endl;
        return;
    }

    if (std::find(_channels[user.getMessage().getArgs()[0]].getOps().begin(), _channels[user.getMessage().getArgs()[0]].getOps().end(),
        user) == _channels[user.getMessage().getArgs()[0]].getOps().end())
    {
        response = ":" + user.getHostname() + " 482 " + user.getNickname() + " " + _channels[user.getMessage().getArgs()[0]].getName() + " :You're not channel operator\r\n";
        send(user.getFd(), response.c_str(), response.size(), 0);
        std::cout << "[ SERVER ] Message sent to client " << user.getFd() << "( " + user.getHostname() + " )" << std::endl;
        return;
    }

    if (std::find(_channels[user.getMessage().getArgs()[0]].getUsers().begin(), _channels[user.getMessage().getArgs()[0]].getUsers().end(),
        user) != _channels[user.getMessage().getArgs()[0]].getUsers().end())
    {
        response = ":" + user.getHostname() + " 443 " + user.getNickname() + " " + _channels[user.getMessage().getArgs()[0]].getName() + " :User already in channel\r\n";
        send(user.getFd(), response.c_str(), response.size(), 0);
        std::cout << "[ SERVER ] Message sent to client " << user.getFd() << "( " + user.getHostname() + " )" << std::endl;
        return;
    }

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