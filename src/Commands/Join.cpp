#include "../../inc/Server.hpp"

void Server::JoinCmd(User &user)
{
	std::string response;

	//Server response if user does not input arguments
	if (user.getMessage().getArgs().size() < 1)
	{
		response = ":" + user.getHostname() + " 461 " + user.getNickname() + " JOIN :Not enough parameters\r\n";
		send(user.getFd(), response.c_str(), response.size(), 0);
		std::cout << "[ SERVER ] Message sent to client: " << user.getFd() << "( " << user.getHostname() << " ) - " << response;
		return;
	}

	//Server response if user inputs a channel without a #
	if (user.getMessage().getArgs()[0][0] != '#')
	{
		response = ":" + user.getHostname() + " 403 " + user.getNickname() + " JOIN :No such channel\r\n";
		send(user.getFd(), response.c_str(), response.size(), 0);
		std::cout << "[ SERVER ] Message sent to client: " << user.getFd() << "( " << user.getHostname() << " ) - " << response;
		return;
	}

	//Server response if channel does not exist and User inputs a channel name
	if (this->_channels.find(user.getMessage().getArgs()[0]) == this->_channels.end() && user.getMessage().getArgs().size() == 1)
	{
		Channel newChannel(user.getMessage().getArgs()[0], std::vector<User>(1, user));
		newChannel.getUsers().push_back(user);
		newChannel.getOps().push_back(user);
		_channels[user.getMessage().getArgs()[0]] = newChannel;
		response = ":" + user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname() + " JOIN " + user.getMessage().getArgs()[0] + "\r\n";
		send(user.getFd(), response.c_str(), response.size(), 0);
		std::cout << "[ SERVER ] Message sent to client: " << user.getFd() << "( " << user.getHostname() << " ) - " << response;
		return;
	}

	//Server responde if channel exists but is invite only
	if (this->_channels.find(user.getMessage().getArgs()[0]) != this->_channels.end() && 
		this->_channels[user.getMessage().getArgs()[0]].getMode().find('i') != std::string::npos)
	{
		if (std::find(this->_channels[user.getMessage().getArgs()[0]].getInvited().begin(), 
			this->_channels[user.getMessage().getArgs()[0]].getInvited().end(), user) == this->_channels[user.getMessage().getArgs()[0]].getInvited().end())
		{
			response = ":" + user.getHostname() + " 473 " + user.getNickname() + " JOIN " + user.getMessage().getArgs()[0] + " :Cannot join channel (+i)\r\n";
			send(user.getFd(), response.c_str(), response.size(), 0);
			std::cout << "[ SERVER ] Message sent to client: " << user.getFd() << "( " << user.getHostname() << " ) - " << response;
			return;
		}
	}

	//Server response if channel exists but has user limit and it is full
	if (this->_channels.find(user.getMessage().getArgs()[0]) != this->_channels.end() && 
		this->_channels[user.getMessage().getArgs()[0]].getMaxUsers() > 0 && 
		this->_channels[user.getMessage().getArgs()[0]].getUsers().size() == this->_channels[user.getMessage().getArgs()[0]].getMaxUsers())
	{
		response = ":" + user.getHostname() + " 471 " + user.getNickname() + " JOIN " + user.getMessage().getArgs()[0] + " :Cannot join channel (+l)\r\n";
		send(user.getFd(), response.c_str(), response.size(), 0);
		std::cout << "[ SERVER ] Message sent to client: " << user.getFd() << "( " << user.getHostname() << " ) - " << response;
		return;
	}

	//Server response if channel exists and user is already in it
	if (this->_channels.find(user.getMessage().getArgs()[0]) != this->_channels.end() && 
		std::find(this->_channels[user.getMessage().getArgs()[0]].getUsers().begin(), 
		this->_channels[user.getMessage().getArgs()[0]].getUsers().end(), user) != this->_channels[user.getMessage().getArgs()[0]].getUsers().end())
	{
		response = ":" + user.getHostname() + " 443 " + user.getNickname() + " JOIN " + user.getMessage().getArgs()[0] + " :You are already in that channel\r\n";
		send(user.getFd(), response.c_str(), response.size(), 0);
		std::cout << "[ SERVER ] Message sent to client: " << user.getFd() << "( " << user.getHostname() << " ) - " << response;
		return;
	}

	//Server response if channel has a password and user did not provide it
	if (this->_channels.find(user.getMessage().getArgs()[0]) != this->_channels.end() && 
		!this->_channels[user.getMessage().getArgs()[0]].getPass().empty() && 
		user.getMessage().getArgs().size() < 2)
	{
		response = ":" + user.getHostname() + " 475 " + user.getNickname() + " JOIN " + user.getMessage().getArgs()[0] + " :Cannot join channel (+k)\r\n";
		send(user.getFd(), response.c_str(), response.size(), 0);
		std::cout << "[ SERVER ] Message sent to client: " << user.getFd() << "( " << user.getHostname() << " ) - " << response;
		return;
	}

	//Server response if user provided a channel, that channel exists and all above conditions are met
	_channels[user.getMessage().getArgs()[0]].getUsers().push_back(user);
	std::vector<User> users = _channels[user.getMessage().getArgs()[0]].getUsers();

	response = ":" + user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname() + " JOIN " + user.getMessage().getArgs()[0] + "\r\n";
	for (std::vector<User>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if (it->getFd() != user.getFd())
		{
			send(it->getFd(), response.c_str(), response.size(), 0);
			std::cout << "[ SERVER ] Message sent to client: " << it->getFd() << "( " << it->getHostname() << " ) - " << response;
		}
	}

	if (!_channels[user.getMessage().getArgs()[0]].getTopic().empty())
	{
		response = ":" + user.getHostname() + " 332 " + user.getNickname() + " " + user.getMessage().getArgs()[0] + " :" + _channels[user.getMessage().getArgs()[0]].getTopic() + "\r\n";
		send(user.getFd(), response.c_str(), response.size(), 0);
		std::cout << "[ SERVER ] Message sent to client: " << user.getFd() << "( " << user.getHostname() << " ) - " << response;
	}
	response = ":" + user.getHostname() + " 353 " + user.getNickname() + " JOIN " + user.getMessage().getArgs()[0] + " :";
	for (std::vector<User>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if (it == users.begin())
		{
			if (it->getNickname() == user.getMessage().getArgs()[0])
				response += "@" + it->getNickname();
			else
				response += " " + it->getNickname();
			continue;
		}
		if (it->getNickname() == user.getMessage().getArgs()[0])
			response += " @" + it->getNickname();
		else
			response += " " + it->getNickname();
	}
	response += "\r\n";

	std::string eon = ":" + user.getHostname() + " 366 " + user.getNickname() + " JOIN " + user.getMessage().getArgs()[0] + " :End of /NAMES list\r\n";
	for (std::vector<User>::iterator it = users.begin(); it != users.end(); ++it)
	{
		send(it->getFd(), response.c_str(), response.size(), 0);
		std::cout << "[ SERVER ] Message sent to client: " << it->getFd() << "( " << it->getHostname() << " ) - " << response;
		send(it->getFd(), eon.c_str(), eon.size(), 0);
		std::cout << "[ SERVER ] Message sent to client: " << it->getFd() << "( " << it->getHostname() << " ) - " << eon;
	}
}