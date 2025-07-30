#include "../../inc/Server.hpp"

/**
 * @brief This handles the kick command for channels, cheks if channel exists, if user in channel and if user is an operator, as well as that there is still operators in channel
 * 
 * @param user 
 */
void Server::KickCmd(User &user)
{
	std::string response = "";
	int kickedIndex;
	bool userInChannel = false;
	bool kickedInChannel = false;

	if (user.getMessage().getArgs().empty() || user.getMessage().getArgs()[0] == "KICK")
	{
		//ERR_NEEDMOREPARAMS
		response = ":" + user.getHostname() + " 461 " + user.getNickname() + " KICK :Not enough parameters\r\n";
		send(user.getFd(), response.c_str(), response.size(), 0);
		std::cout << "[ SERVER ] Message sent to client " << user.getFd() << "( " + user.getHostname() + " )" << std::endl;
		return;
	}

	if (_channels.find(user.getMessage().getArgs()[0]) == _channels.end())
	{
		//ERR_NOSUCHCHANNEL
		response = ":" + user.getHostname() + " 403 " + user.getNickname() + " " + user.getMessage().getArgs()[0] + " :No such channel\r\n";
		send(user.getFd(), response.c_str(), response.size(), 0);
		std::cout << "[ SERVER ] Message sent to client " << user.getFd() << "( " + user.getHostname() + " )" << std::endl;
		return;
	}

	for (std::vector<User>::iterator it = _channels[user.getMessage().getArgs()[0]].getUsers().begin(); it != _channels[user.getMessage().getArgs()[0]].getUsers().end(); it++)
	{
		if (*it == user)
		{
			userInChannel = true;
			break;
		}
	}
	
	if (!userInChannel)
	{
		//ERR_NOTONCHANNEL
		response = ":" + user.getHostname() + " 442 " + user.getNickname() + " " + user.getMessage().getArgs()[0] + " :You're not on that channel\r\n";
		send(user.getFd(), response.c_str(), response.size(), 0);
		std::cout << "[ SERVER ] Message sent to client " << user.getFd() << "( " + user.getHostname() + " )" << std::endl;
		return;
	}
	
	if (std::find(_channels[user.getMessage().getArgs()[0]].getOps().begin(),
		_channels[user.getMessage().getArgs()[0]].getOps().end(), user) == _channels[user.getMessage().getArgs()[0]].getOps().end())
	{
		//ERR_CHANOPRIVSNEEDED
		response = ":" + user.getHostname() + " 482 " + user.getNickname() + " " + user.getMessage().getArgs()[0] + " :You're not channel operator\r\n";
		send(user.getFd(), response.c_str(), response.size(), 0);
		std::cout << "[ SERVER ] Message sent to client " << user.getFd() << " ( " << user.getHostname() << " )" << response;
		return;
	}

	for (std::vector<User>::iterator it = _channels[user.getMessage().getArgs()[0]].getUsers().begin(); it != _channels[user.getMessage().getArgs()[0]].getUsers().end(); it++)
	{
		if (it->getNickname() == user.getMessage().getArgs()[1])
		{
			kickedInChannel = true;
			kickedIndex = it - _channels[user.getMessage().getArgs()[0]].getUsers().begin();
			break;
		}
	}

	if (!kickedInChannel)
	{
		//ERR_USERNOTINCHANNEL
		response = ":" + user.getHostname() + " 441 " + user.getNickname() + " " + user.getMessage().getArgs()[1] + " " + user.getMessage().getArgs()[0] + " :They aren't on that channel\r\n";
		send(user.getFd(), response.c_str(), response.size(), 0);
		std::cout << "[ SERVER ] Message sent to client " << user.getFd() << "( " << user.getHostname() << " )" << std::endl;
		return;
	}

	response = ":" + user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname() + " KICK " + _channels[user.getMessage().getArgs()[0]].getName() + " " + user.getMessage().getArgs()[1] + " :";
	if (user.getMessage().getArgs().size() > 2)
		response += user.getMessage().getArgs()[2] + "\r\n";
	else
		response += "Kicked by " + user.getNickname() + "\r\n";

	for (std::vector<User>::iterator it = _channels[user.getMessage().getArgs()[0]].getUsers().begin(); it != _channels[user.getMessage().getArgs()[0]].getUsers().end(); it++)
	{
		send(it->getFd(), response.c_str(), response.size(), 0);
		std::cout << "[ SERVER ] Message sent to client " << it->getFd() << " ( " << it->getHostname() << " )" << response;
	}
	_channels[user.getMessage().getArgs()[0]].rmOps(_channels[user.getMessage().getArgs()[0]].getUsers()[kickedIndex]);
	_channels[user.getMessage().getArgs()[0]].rmUser(_channels[user.getMessage().getArgs()[0]].getUsers()[kickedIndex]);
	if (_channels[user.getMessage().getArgs()[0]].getUsers().empty())
		_channels.erase(user.getMessage().getArgs()[0]);
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