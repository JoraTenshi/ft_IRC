#include "../../inc/Server.hpp"

//hay que revisar esto esta hecho a priori por copilot

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
    
    // Si es un canal (empieza con #)
    if (target[0] == '#')
    {
        if (_channels.find(target) == _channels.end())
        {
            // Canal no existe
            response = ":" + user.getHostname() + " 315 " + user.getNickname() + " " + target + " :End of /WHO list\r\n";
            send(user.getFd(), response.c_str(), response.size(), 0);
            std::cout << " [ SERVER ] Message sent to client " << user.getFd() << "( " << user.getHostname() << " )" << response;
            return;
        }

        std::vector<User> users = _channels[target].getUsers();
        std::vector<User> ops = _channels[target].getOps();

        // Enviar RPL_WHOREPLY (352) para cada usuario del canal
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

            // RPL_WHOREPLY: 352 nick channel username hostname server nickname flags :hopcount realname
            response = ":" + user.getHostname() + " 352 " + user.getNickname() + " " + target + " " + 
                      it->getUsername() + " " + it->getHostname() + " " + user.getHostname() + " " + 
                      it->getNickname() + " H" + (isOp ? "@" : "") + " :0 " + it->getRealname() + "\r\n";
            
            send(user.getFd(), response.c_str(), response.size(), 0);
            std::cout << " [ SERVER ] Message sent to client " << user.getFd() << "( " << user.getHostname() << " )" << response;
        }
    }
    else
    {
        // WHO para un usuario específico
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

    // RPL_ENDOFWHO (315) - Fin de la lista WHO
    response = ":" + user.getHostname() + " 315 " + user.getNickname() + " " + target + " :End of /WHO list\r\n";
    send(user.getFd(), response.c_str(), response.size(), 0);
    std::cout << " [ SERVER ] Message sent to client " << user.getFd() << "( " << user.getHostname() << " )" << response;
}