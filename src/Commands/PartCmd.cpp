#include "../../inc/Server.hpp"

/**
 * @brief This handles parting a channel, checking if the user is connected to the channel and removing them from it.
 * 
 * @param user 
 */
void Server::PartCmd(User &user)
{
	std::string response;
	int partedIndex;
	bool found;

	if (user.getMessage().getArgs().empty() || user.getMessage().getArgs()[0] == "PART")
	{
		//ERR_NEEDMOREPARAMS
		response = ":" + user.getHostname() + " 461 " + user.getNickname() + " PART :Not enough parameters\r\n";
		send(user.getFd(), response.c_str(), response.size(), 0);
		std::cout << "[ SERVER ] Message sent to client " << user.getFd() << "( " << user.getHostname() << " )" << response;
		return;
	}

	if (_channels.find(user.getMessage().getArgs()[0]) == _channels.end())
	{
		//ERR_NOSUCHCHANNEL
		response = "!" + user.getHostname() + "403 " + user.getNickname() + " " + user.getMessage().getArgs()[0] + " :No such channel\r\n";
		send(user.getFd(), response.c_str(), response.size(), 0);
		std::cout << "[ SERVER ] Message sent to client " << user.getFd() << "( " << user.getHostname() << " )" << response;
		return;
	}

	found = false;
	for (std::vector<User>::iterator it = _channels[user.getMessage().getArgs()[0]].getUsers().begin(); it != _channels[user.getMessage().getArgs()[0]].getUsers().end(); it++)
	{
		if (*it == user)
		{
			found = true;
			partedIndex = it - _channels[user.getMessage().getArgs()[0]].getUsers().begin();
			break;
		}
	}

	if (!found)
	{
		//ERR_NOTONCHANNEL
		response = ":" + user.getHostname() + "442 " + user.getNickname() + " " + user.getMessage().getArgs()[0] + " :You're not on that channel\r\n";
		send(user.getFd(), response.c_str(), response.size(), 0);
		std::cout << "[ SERVER ] Message sent to client " << user.getFd() << "( " << user.getHostname() << " )" << response;
		return;
	}

	response = ":" + user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname() + " PART " + user.getMessage().getArgs()[0] + "\r\n";
	if (user.getMessage().getMsg().empty())
		response += user.getNickname() + " is leaving\r\n";
	else if (user.getMessage().getArgs().size() > 1)
		response += " " + user.getMessage().getArgs()[1] + "\r\n";
	else
		response += "\r\n";
	
	for (std::vector<User>::iterator it = _channels[user.getMessage().getArgs()[0]].getUsers().begin(); it != _channels[user.getMessage().getArgs()[0]].getUsers().end(); it++)
	{
		send(it->getFd(), response.c_str(), response.size(), 0);
		std::cout << "[ SERVER ] Message sent to client " << it->getFd() << "( " << it->getHostname() << " )" << response;
	}
	_channels[user.getMessage().getArgs()[0]].rmOps(_channels[user.getMessage().getArgs()[0]].getUsers()[partedIndex]);
	_channels[user.getMessage().getArgs()[0]].rmUser(_channels[user.getMessage().getArgs()[0]].getUsers()[partedIndex]);
	if (_channels[user.getMessage().getArgs()[0]].getUsers().empty())
	{
		_channels.erase(user.getMessage().getArgs()[0]);
		return;
	}
	else if (_channels[user.getMessage().getArgs()[0]].getOps().size() == 0)
	{
		_channels[user.getMessage().getArgs()[0]].getOps().push_back(_channels[user.getMessage().getArgs()[0]].getUsers()[0]);
		std::vector<User> chOps = _channels[user.getMessage().getArgs()[0]].getOps();
		chOps.push_back(_channels[user.getMessage().getArgs()[0]].getUsers()[0]);
		_channels[user.getMessage().getArgs()[0]].setOps(chOps);
		for (std::vector<User>::iterator it = _channels[user.getMessage().getArgs()[0]].getUsers().begin(); it != _channels[user.getMessage().getArgs()[0]].getUsers().end(); it++)
		{
			response = ":" + user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname() + " MODE " + user.getMessage().getArgs()[0] + " +o "
					+ _channels[user.getMessage().getArgs()[0]].getUsers()[0].getNickname() + "\r\n";
			send(it->getFd(), response.c_str(), response.size(), 0);
			std::cout << "[ SERVER ] Message sent to client " << it->getFd() << " ( " << it->getHostname() << " )" << response;
		}
	}
}