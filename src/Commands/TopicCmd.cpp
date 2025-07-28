#include "../../inc/Server.hpp"

void Server::TopicCmd(User &user)
{
    std::string response = "";

	if (user.getMessage().getArgs().empty() || user.getMessage().getArgs()[0] == "TOPIC")
	{
		response = ":" + user.getHostname() + " 461 " + user.getNickname() + " TOPIC :Not enough parameters\r\n";
		send(user.getFd(), response.c_str(), response.size(), 0);
		std::cout << "[ SERVER ] Message sent to client " << user.getFd() << "( " + user.getHostname() + " )" << std::endl;
		return;
	}

	for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); it++)
	{
		if (it->first == _channels[user.getMessage().getArgs()[0]].getName())
		{
			response = ":" + user.getHostname() + " 403 " + user.getNickname() + " " + _channels[user.getMessage().getArgs()[0]].getName() + " :No topic is set\r\n";
			send(user.getFd(), response.c_str(), response.size(), 0);
			std::cout << "[ SERVER ] Message sent to client " << user.getFd() << "( " + user.getHostname() + " )" << std::endl;
			return;
		}
	}

	if (_channels.find(_channels[user.getMessage().getArgs()[0]].getName()) == _channels.end())
	{
		response = ":" + user.getHostname() + " 442 " + user.getNickname() + " " + _channels[user.getMessage().getArgs()[0]].getName() + " :No such channel\r\n";
		send(user.getFd(), response.c_str(), response.size(), 0);
		std::cout << "[ SERVER ] Message sent to client " << user.getFd() << "( " + user.getHostname() + " )" << std::endl;
		return;
	}

	if (user.getMessage().getArgs().size() == 1 && user.getMessage().getMsg().empty())
	{
		if (_channels[user.getMessage().getArgs()[0]].getTopic().empty())
		{
			response = ":" + user.getHostname() + " 331 " + user.getNickname() + " " + _channels[user.getMessage().getArgs()[0]].getName() + " :No topic is set\r\n";
			send(user.getFd(), response.c_str(), response.size(), 0);
			std::cout << "[ SERVER ] Message sent to client " << user.getFd() << "( " + user.getHostname() + " )" << std::endl;
		}
		else
		{
			response = ":" + user.getHostname() + " 332 " + user.getNickname() + " " + _channels[user.getMessage().getArgs()[0]].getName() + " :" + _channels[user.getMessage().getArgs()[0]].getTopic() + "\r\n";
			send(user.getFd(), response.c_str(), response.size(), 0);
			std::cout << "[ SERVER ] Message sent to client " << user.getFd() << "( " + user.getHostname() + " )" << std::endl;
		}
		return ;
	}

	if (_channels[user.getMessage().getArgs()[0]].getMode().find('t') == std::string::npos ||
		std::find(_channels[user.getMessage().getArgs()[0]].getOps().begin(), _channels[user.getMessage().getArgs()[0]].getOps().end(), user) == _channels[user.getMessage().getArgs()[0]].getOps().end())
	{
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
		response = ":" + user.getHostname() + " 332 " + user.getNickname() + " " + _channels[user.getMessage().getArgs()[0]].getName() + " :" + topic + "\r\n";
		for (std::vector<User>::iterator it = _channels[user.getMessage().getArgs()[0]].getUsers().begin(); it != _channels[user.getMessage().getArgs()[0]].getUsers().end(); it++)
		{
			send(it->getFd(), response.c_str(), response.size(), 0);
			std::cout << "[ SERVER ] Message sent to client " << it->getFd() << "( " + it->getHostname() + " )" << std::endl;
		}
		return ;
	}
}