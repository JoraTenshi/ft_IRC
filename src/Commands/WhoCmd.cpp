#include "../../inc/Server.hpp"

void Server::WhoCmd(User &user)
{
    std::string response;
    
    if (user.getMessage().getArgs().size() < 1)
    {
        // ERR_NEEDMOREPARAMS
        response = ":" + user.getHostname() + " 461 " + user.getNickname() + " WHO :Not enough parameters\r\n";
        send(user.getFd(), response.c_str(), response.size(), 0);
        std::cout << " [ SERVER ] Message sent to client " << user.getFd() << "( " << user.getHostname() << " )" << response;
        return;
    }

    std::string target = user.getMessage().getArgs()[0];
    
    if (target[0] == '#')
    {
        if (_channels.find(target) == _channels.end())
        {
            //ERR_NOSUCHCHANNEL
            response = ":" + user.getHostname() + " 315 " + user.getNickname() + " " + target + " :End of /WHO list\r\n";
            send(user.getFd(), response.c_str(), response.size(), 0);
            std::cout << " [ SERVER ] Message sent to client " << user.getFd() << "( " << user.getHostname() << " )" << response;
            return;
        }

        std::vector<User> users = _channels[target].getUsers();
        std::vector<User> ops = _channels[target].getOps();

        for (std::vector<User>::iterator it = users.begin(); it != users.end(); ++it)
        {
            bool isOp = false;
            for (std::vector<User>::iterator opIt = ops.begin(); opIt != ops.end(); ++opIt)
            {
                if (opIt->getNickname() == it->getNickname())
                {
                    isOp = true;
                    break;
                }
            }

            // RPL_WHOREPLY
            response = ":" + user.getHostname() + " 352 " + user.getNickname() + " " + target + " " + 
                      it->getUsername() + " " + it->getHostname() + " " + user.getHostname() + " " + 
                      it->getNickname() + " H" + (isOp ? "@" : "") + " :0 " + it->getRealname() + "\r\n";
            
            send(user.getFd(), response.c_str(), response.size(), 0);
            std::cout << " [ SERVER ] Message sent to client " << user.getFd() << "( " << user.getHostname() << " )" << response;
        }
    }
    else
    {
        bool found = false;
        for (std::map<int, User>::iterator it = _users.begin(); it != _users.end(); ++it)
        {
            if (it->second.getNickname() == target)
            {
                found = true;
                response = ":" + user.getHostname() + " 352 " + user.getNickname() + " * " + 
                          it->second.getUsername() + " " + it->second.getHostname() + " " + user.getHostname() + " " + 
                          it->second.getNickname() + " H :0 " + it->second.getRealname() + "\r\n";
                
                send(user.getFd(), response.c_str(), response.size(), 0);
                std::cout << " [ SERVER ] Message sent to client " << user.getFd() << "( " << user.getHostname() << " )" << response;
                break;
            }
        }
    }

    // RPL_ENDOFWHO
    response = ":" + user.getHostname() + " 315 " + user.getNickname() + " " + target + " :End of /WHO list\r\n";
    send(user.getFd(), response.c_str(), response.size(), 0);
    std::cout << " [ SERVER ] Message sent to client " << user.getFd() << "( " << user.getHostname() << " )" << response;
}