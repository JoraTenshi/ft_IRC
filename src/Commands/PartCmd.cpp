#include "../../inc/Server.hpp"

/**
 * @brief This handles parting a channel, checking if the user is connected to the channel and removing them from it.
 * 
 * @param user 
 */
void Server::PartCmd(User &user)
{
	std::string response;
	bool found;

	if (user.getMessage().getArgs().size() < 1)
	{
		//ERR_NEEDMOREPARAMS
		response = ":" + user.getHostname() + "461 " + user.getNickname() + " PART :Not enough parameters\r\n";
		send(user.getFd(), response.c_str(), response.size(), 0);
		std::cout << " [ SERVER ] Message sent to client " << user.getFd() << "( " << user.getHostname() << " )" << response;
		return;
	}

	if (_channels.find(user.getMessage().getArgs()[0]) == _channels.end())
	{
		//ERR_NOSUCHCHANNEL
		response = "!" + user.getHostname() + "403 " + user.getNickname() + " " + user.getMessage().getArgs()[0] + " :No such channel\r\n";
		send(user.getFd(), response.c_str(), response.size(), 0);
		std::cout << " [ SERVER ] Message sent to client " << user.getFd() << "( " << user.getHostname() << " )" << response;
		return;
	}

	found = false;
	for (std::vector<User>::iterator it = _channels[user.getMessage().getArgs()[0]].getUsers().begin(); it != _channels[user.getMessage().getArgs()[0]].getUsers().end(); it++)
	{
		if (it->getNickname() == user.getNickname())
		{
			_channels[user.getMessage().getArgs()[0]].getUsers().erase(it);
			found = true;
			break;
		}
	}

	if (!found)
	{
		//ERR_NOTONCHANNEL
		response = ":" + user.getHostname() + "442 " + user.getNickname() + " " + user.getMessage().getArgs()[0] + " :You're not on that channel\r\n";
		send(user.getFd(), response.c_str(), response.size(), 0);
		std::cout << " [ SERVER ] Message sent to client " << user.getFd() << "( " << user.getHostname() << " )" << response;
		return;
	}

	response = ":" + user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname() + " PART " + user.getMessage().getArgs()[0] + "\r\n";
	if (user.getMessage().getMsg().empty())
		response += user.getNickname() + " is leaving\r\n";
	else
		response += " " + user.getMessage().getMsg() + "\r\n";
	send(user.getFd(), response.c_str(), response.size(), 0);
	std::cout << " [ SERVER ] Message sent to client " << user.getFd() << "( " << user.getHostname() << " )" << response;

	for (std::vector<User>::iterator it = _channels[user.getMessage().getArgs()[0]].getUsers().begin(); it != _channels[user.getMessage().getArgs()[0]].getUsers().end(); it++)
	{
		send(it->getFd(), response.c_str(), response.size(), 0);
		std::cout << " [ SERVER ] Message sent to client " << it->getFd() << "( " << it->getHostname() << " )" << response;
	}

	if (_channels[user.getMessage().getArgs()[0]].getUsers().size() == 0)
		_channels.erase(user.getMessage().getArgs()[0]);
	else if (_channels[user.getMessage().getArgs()[0]].getUsers().size() >= 1 && _channels[user.getMessage().getArgs()[0]].getOps().size() == 0)
	{
		_channels[user.getMessage().getArgs()[0]].getOps().push_back(_channels[user.getMessage().getArgs()[0]].getUsers()[0]);
		response = ":" + user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname() + " MODE " + user.getMessage().getArgs()[0] + " +o " + user.getNickname() + "\r\n";
		send(user.getFd(), response.c_str(), response.size(), 0);
		std::cout << " [ SERVER ] Message sent to client " << user.getFd() << "( " << user.getHostname() << " )" << response;
	}
}