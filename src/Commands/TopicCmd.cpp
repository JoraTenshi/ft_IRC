#include "../../inc/Server.hpp"

void Server::TopicCmd(User &user)
{
    std::string response = "";

	if (user.getChannel().empty() || user.getMessage().getArgs()[0] == "TOPIC")
	{
		response = ":" + user.getHostname() + " 461 " + user.getNickname() + " TOPIC :Not enough parameters\r\n";
		send(user.getSocket(), response.c_str(), response.size(), 0);
		std::cout << "[ SERVER ] Message sent to client " << user.getFd() << "( " + user.getHostname() + " )" << std::endl;
		return;
	}

	for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); it++)
	{
		if (it->first == user.getChannel())
		{
			response = ":" + user.getHostname() + " 403 " + user.getNickname() + " " + user.getChannel() + " :No topic is set\r\n";
			send(user.getSocket(), response.c_str(), response.size(), 0);
			std::cout << "[ SERVER ] Message sent to client " << user.getFd() << "( " + user.getHostname() + " )" << std::endl;
			return;
		}
	}

	if (_channels.find(user.getChannel()) == _channels.end())
	{
		response = ":" + user.getHostname() + " 442 " + user.getNickname() + " " + user.getChannel() + " :No such channel\r\n";
		send(user.getSocket(), response.c_str(), response.size(), 0);
		std::cout << "[ SERVER ] Message sent to client " << user.getFd() << "( " + user.getHostname() + " )" << std::endl;
		return;
	}

	if (user.getMessage().getArgs().size() == 1 && user.getMessage().getMsg(),empty())
	{
		if (_channels[user.getChannel()].getTopic().empty())
		{
			response = ":" + user.getHostname() + " 331 " + user.getNickname() + " " + user.getChannel() + " :No topic is set\r\n";
			send(user.getSocket(), response.c_str(), response.size(), 0);
			std::cout << "[ SERVER ] Message sent to client " << user.getFd() << "( " + user.getHostname() + " )" << std::endl;
		}
		else
		{
			response = ":" + user.getHostname() + " 332 " + user.getNickname() + " " + user.getChannel() + " :" + _channels[user.getChannel()].getTopic() + "\r\n";
			send(user.getSocket(), response.c_str(), response.size(), 0);
			std::cout << "[ SERVER ] Message sent to client " << user.getFd() << "( " + user.getHostname() + " )" << std::endl;
		}
		return ;
	}

	if (_channel[user.getMessage().getArgs()].getMode().find('t') == std::string::npos || _channels[user.getChannel()].getOps().find(user.getNickname()) == _channels[user.getChannel()].getOps().end())
	{
		response = ":" + user.getHostname() + " 482 " + user.getNickname() + " " + user.getChannel() + " :You're not channel operator\r\n";
		send(user.getSocket(), response.c_str(), response.size(), 0);
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
		_channels[user.getChannel()].setTopic(topic);
		response = ":" + user.getHostname() + " 332 " + user.getNickname() + " " + user.getChannel() + " :" + topic + "\r\n";
		for (std::vector<User>::iterator it = _channels[user.getChannel()].getUsers().begin(); it != _channels[user.getChannel()].getUsers().end(); it++)
		{
			send(it->getSocket(), response.c_str(), response.size(), 0);
			std::cout << "[ SERVER ] Message sent to client " << it->getFd() << "( " + it->getHostname() + " )" << std::endl;
		}
		return ;
	}
}