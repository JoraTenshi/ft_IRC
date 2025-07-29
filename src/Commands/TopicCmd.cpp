#include "../../inc/Server.hpp"

/**
 * @brief This handles the topic command for channels, allowing users to set or view the topic of a channel if they have the appropriate permissions.
 * 
 * @param user 
 */
void Server::TopicCmd(User &user)
{
    std::string response = "";
	bool userInChannel = false;

	if (user.getMessage().getArgs().empty() || user.getMessage().getArgs()[0] == "TOPIC")
	{
		//ERR_NEEDMOREPARAMS
		response = ":" + user.getHostname() + " 461 " + user.getNickname() + " TOPIC :Not enough parameters\r\n";
		send(user.getFd(), response.c_str(), response.size(), 0);
		std::cout << "[ SERVER ] Message sent to client " << user.getFd() << "( " + user.getHostname() + " )" << std::endl;
		return;
	}

	if (_channels.find(_channels[user.getMessage().getArgs()[0]].getName()) == _channels.end())
	{
		//ERR_NOSUCHCHANNEL
		response = ":" + user.getHostname() + " 403 " + user.getNickname() + " " + _channels[user.getMessage().getArgs()[0]].getName() + " :No such channel\r\n";
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
		response = ":" + user.getHostname() + " 442 " + user.getNickname() + " " + _channels[user.getMessage().getArgs()[0]].getName() + " :You're not on that channel\r\n";
		send(user.getFd(), response.c_str(), response.size(), 0);
		std::cout << "[ SERVER ] Message sent to client " << user.getFd() << "( " + user.getHostname() + " )" << std::endl;
		return;
	}

	if (user.getMessage().getArgs().size() == 1 && user.getMessage().getMsg().empty())
	{
		if (_channels[user.getMessage().getArgs()[0]].getTopic().empty())
		{
			//RPL_NOTOPIC
			response = ":" + user.getHostname() + " 331 " + user.getNickname() + " " + _channels[user.getMessage().getArgs()[0]].getName() + " :No topic is set\r\n";
			send(user.getFd(), response.c_str(), response.size(), 0);
			std::cout << "[ SERVER ] Message sent to client " << user.getFd() << "( " + user.getHostname() + " )" << std::endl;
		}
		else
		{
			//RPL_TOPIC
			response = ":" + user.getHostname() + " 332 " + user.getNickname() + " " + _channels[user.getMessage().getArgs()[0]].getName() + " :" + _channels[user.getMessage().getArgs()[0]].getTopic() + "\r\n";
			send(user.getFd(), response.c_str(), response.size(), 0);
			std::cout << "[ SERVER ] Message sent to client " << user.getFd() << "( " + user.getHostname() + " )" << std::endl;
		}
		return ;
	}

	if (_channels[user.getMessage().getArgs()[0]].getMode().find('t') == std::string::npos ||
		std::find(_channels[user.getMessage().getArgs()[0]].getOps().begin(), _channels[user.getMessage().getArgs()[0]].getOps().end(), user) == _channels[user.getMessage().getArgs()[0]].getOps().end())
	{
		//ERR_CHANOPRIVSNEEDED
		response = ":" + user.getHostname() + " 482 " + user.getNickname() + " " + _channels[user.getMessage().getArgs()[0]].getName() + " :You're not channel operator\r\n";
		send(user.getFd(), response.c_str(), response.size(), 0);
		std::cout << "[ SERVER ] Message sent to client " << user.getFd() << "( " + user.getHostname() + " )" << std::endl;
		return;
	}
	else
	{
		std::string topic = "";
		if (user.getMessage().getMsg().empty())
		    topic = user.getMessage().getArgs()[1];
		else
			topic = user.getMessage().getMsg();
		_channels[user.getMessage().getArgs()[0]].setTopic(topic);
		//RPL_TOPIC
		response = ":" + user.getHostname() + " 332 " + user.getNickname() + " " + _channels[user.getMessage().getArgs()[0]].getName() + " :" + topic + "\r\n";
		for (std::vector<User>::iterator it = _channels[user.getMessage().getArgs()[0]].getUsers().begin(); it != _channels[user.getMessage().getArgs()[0]].getUsers().end(); it++)
		{
			send(it->getFd(), response.c_str(), response.size(), 0);
			std::cout << "[ SERVER ] Message sent to client " << it->getFd() << "( " + it->getHostname() + " )" << std::endl;
		}
		return ;
	}
}