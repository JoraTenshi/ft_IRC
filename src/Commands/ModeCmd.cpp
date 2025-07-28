#include "../../inc/Server.hpp"

/**
 * @brief This handles the mode command for channels, allowing channel operators to set modes like invite-only, topic-protected, and more.
 * 		It also handles channel operators trying to remove their own operator status.
 * 
 * @param user 
 */
void Server::ModeCmd(User &user)
{
	std::string response = "";
	bool userInChannel = false;
	std::string mode = "";

	//Server response if user does not input arguments
	if (user.getMessage().getArgs().empty() ||user.getMessage().getArgs()[0] == "MODE")
	{
		//ERR_NEEDMOREPARAMS
		response = ":" + user.getHostname() + " 461 " + user.getNickname() + " MODE :Not enough parameters\r\n";
		send(user.getFd(), response.c_str(), response.size(), 0);
		std::cout << "[ SERVER ] Message sent to client " << user.getFd() << " ( " << user.getHostname() << " )" << response;
		return;
	}

	//Server response if channel does not exist
	if (_channels.find(user.getMessage().getArgs()[0]) == _channels.end())
	{
		//ERR_NOSUCHCHANNEL
		response = ":" + user.getHostname() + " 403 " + user.getNickname() + " " + user.getMessage().getArgs()[0] + " :No such channel\r\n";
		send(user.getFd(), response.c_str(), response.size(), 0);
		std::cout << "[ SERVER ] Message sent to client " << user.getFd() << " ( " << user.getHostname() << " )" << response;
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
	//Server response if user is not in channel
	if (!userInChannel)
	{
		//ERR_NOTONCHANNEL
		response = ":" + user.getHostname() + " 442 " + user.getNickname() + " " + user.getMessage().getArgs()[0] + " :You're not on that channel\r\n";
		send(user.getFd(), response.c_str(), response.size(), 0);
		std::cout << "[ SERVER ] Message sent to client " << user.getFd() << " ( " << user.getHostname() << " )" << response;
		return;
	}

	//Server response if user is trying to get channel mode without setting it
	if (user.getMessage().getArgs().size() == 1 || (user.getMessage().getArgs().size() == 2 && user.getMessage().getArgs()[1].empty()))
	{
		//RPL_CHANNELMODEIS
		response = ":" + user.getHostname() + " 324 " + user.getNickname() + " " + user.getMessage().getArgs()[0] + " :";
		if (_channels[user.getMessage().getArgs()[0]].getMode().empty())
			response += " \r\n";
		else
			response += " " + _channels[user.getMessage().getArgs()[0]].getMode() + "\r\n";
		send(user.getFd(), response.c_str(), response.size(), 0);
		std::cout << "[ SERVER ] Message sent to client " << user.getFd() << " ( " << user.getHostname() << " )" << response;
		return;
	}

	//Server response if user is not channel operator
	if (_channels[user.getMessage().getArgs()[0]].getOps().empty() || 
		std::find(_channels[user.getMessage().getArgs()[0]].getOps().begin(), _channels[user.getMessage().getArgs()[0]].getOps().end(), user) == _channels[user.getMessage().getArgs()[0]].getOps().end())
	{
		//ERR_CHANOPRIVSNEEDED
		response = ":" + user.getHostname() + " 482 " + user.getNickname() + " " + user.getMessage().getArgs()[0] + " :You're not channel operator\r\n";
		send(user.getFd(), response.c_str(), response.size(), 0);
		std::cout << "[ SERVER ] Message sent to client " << user.getFd() << " ( " << user.getHostname() << " )" << response;
		return;
	}

	//Server response if user tries to set mode without parameters
	if (user.getMessage().getArgs()[1].size() != 2 || (user.getMessage().getArgs()[1][0] != '+' && user.getMessage().getArgs()[1][0] != '-'))
	{
		//RPL_CHANNELMODEIS
		response = ":" + user.getHostname() + " 324 " + user.getNickname() + " " + user.getMessage().getArgs()[0];
		if (!_channels[user.getMessage().getArgs()[0]].getMode().empty())
			response += " :" + _channels[user.getMessage().getArgs()[0]].getMode() + "\r\n";
		else
			response += " :\r\n";
		send(user.getFd(), response.c_str(), response.size(), 0);
		std::cout << "[ SERVER ] Message sent to client " << user.getFd() << " ( " << user.getHostname() << " )" << response;
		return;
	}

	mode = user.getMessage().getArgs()[1].substr(1);
	if (user.getMessage().getArgs()[1][0] == '+')
	{
		if (mode == "i")
		{
			mode = _channels[user.getMessage().getArgs()[0]].getMode();
			if (mode.find('i') != std::string::npos)
				mode.erase(mode.find('i'), 1);
			_channels[user.getMessage().getArgs()[0]].setMode(mode);
			response = ":" + user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname() + " MODE " + user.getMessage().getArgs()[0] + " +i\r\n";
		}
		else if (mode == "t")
		{
			mode = _channels[user.getMessage().getArgs()[0]].getMode();
			if (mode.find('t') != std::string::npos)
				mode.erase(mode.find('t'), 1);
			_channels[user.getMessage().getArgs()[0]].setMode(mode);
			response = ":" + user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname() + " MODE " + user.getMessage().getArgs()[0] + " +t\r\n";
		}
		else if (mode == "k")
		{
			mode = _channels[user.getMessage().getArgs()[0]].getMode();
			if (mode.find('k') != std::string::npos)
				mode.erase(mode.find('k'), 1);
			_channels[user.getMessage().getArgs()[0]].setMode(mode);
			if (user.getMessage().getArgs().size() < 3)
			{
				//ERR_NEEDMOREPARAMS
				response = ":" + user.getHostname() + " 461 " + user.getNickname() + " MODE :Not enough parameters\r\n";
				send(user.getFd(), response.c_str(), response.size(), 0);
				std::cout << "[ SERVER ] Message sent to client " << user.getFd() << " ( " << user.getHostname() << " )" << response;
				return;
			}
			_channels[user.getMessage().getArgs()[0]].setPass(user.getMessage().getArgs()[2]);
			response = ":" + user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname() + " MODE " + user.getMessage().getArgs()[0] + " +k\r\n";
		}
		else if (mode == "o")
		{
			if (user.getMessage().getArgs().size() != 3)
			{
				//ERR_NEEDMOREPARAMS
				response = ":" + user.getHostname() + " 461 " + user.getNickname() + " MODE :Not enough parameters\r\n";
				send(user.getFd(), response.c_str(), response.size(), 0);
				std::cout << "[ SERVER ] Message sent to client " << user.getFd() << " ( " << user.getHostname() << " )" << response;
				return;
			}
			userInChannel = false;
			for (std::vector<User>::iterator it = _channels[user.getMessage().getArgs()[0]].getUsers().begin(); it != _channels[user.getMessage().getArgs()[0]].getUsers().end(); it++)
			{
				if (it->getNickname() == user.getMessage().getArgs()[2])
					userInChannel = true;
			}
			if (!userInChannel)
			{
				//ERR_USERNOTINCHANNEL
				response = ":" + user.getHostname() + " 401 " + user.getNickname() + " " + user.getMessage().getArgs()[2] + " " + user.getMessage().getArgs()[0] + " :User not in that channel\r\n";
				send(user.getFd(), response.c_str(), response.size(), 0);
				std::cout << "[ SERVER ] Message sent to client " << user.getFd() << " ( " << user.getHostname() << " )" << response;
				return;
			}

			int newOp = 0;
			for (std::vector<User>::iterator it = _channels[user.getMessage().getArgs()[0]].getOps().begin(); it != _channels[user.getMessage().getArgs()[0]].getOps().end(); it++)
			{
				if (it->getNickname() == user.getMessage().getArgs()[2])
					newOp = 1;
			}
			if (!newOp)
			{
				std::cout << "User could not be given operator status" << std::endl;
				return ;
			}

			std::vector<User> chUsers = _channels[user.getMessage().getArgs()[0]].getUsers();
			std::vector<User> chOps = _channels[user.getMessage().getArgs()[0]].getOps();
			chOps.push_back(user);
			_channels[user.getMessage().getArgs()[0]].setOps(chOps);
			for (std::vector<User>::iterator it = chUsers.begin(); it != chUsers.end(); it++)
			{
				response = ":" + user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname() + " MODE " + user.getMessage().getArgs()[0] + " +o " + user.getMessage().getArgs()[2] + "\r\n";
				send(it->getFd(), response.c_str(), response.size(), 0);
				std::cout << "[ SERVER ] Message sent to client " << it->getFd() << " ( " << it->getHostname() << " )" << response;
			}
			return ;
		}
		else if (mode == "l")
		{
			if (user.getMessage().getArgs().size() != 3)
			{
				//ERR_NEEDMOREPARAMS
				response = ":" + user.getHostname() + " 461 " + user.getNickname() + " MODE :Not enough parameters\r\n";
				send(user.getFd(), response.c_str(), response.size(), 0);
				std::cout << "[ SERVER ] Message sent to client " << user.getFd() << " ( " << user.getHostname() << " )" << response;
				return;
			}
			int maxUsers = std::atoi(user.getMessage().getArgs()[2].c_str());
			if (maxUsers < 1)
			{
				mode = _channels[user.getMessage().getArgs()[0]].getMode();
				if (mode.find('l') != std::string::npos)
					mode.erase(mode.find('l'), 1);
				_channels[user.getMessage().getArgs()[0]].setMode(mode);
				_channels[user.getMessage().getArgs()[0]].setMaxUsers(0);
				response = ":" + user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname() + " MODE " + user.getMessage().getArgs()[0] + " -l\r\n";
			}
			else
			{
				mode = _channels[user.getMessage().getArgs()[0]].getMode();
				if (mode.find('l') != std::string::npos)
					mode.erase(mode.find('l'), 1);
				_channels[user.getMessage().getArgs()[0]].setMode(mode + "l");
				_channels[user.getMessage().getArgs()[0]].setMaxUsers(maxUsers);
				response = ":" + user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname() + " MODE " + user.getMessage().getArgs()[0] + " +l " + user.getMessage().getArgs()[2] + "\r\n";
			}
		}
	}
	else if (user.getMessage().getArgs()[1][0] == '-')
	{
		if (mode == "i")
		{
			mode = _channels[user.getMessage().getArgs()[0]].getMode();
			if (mode.find('i') != std::string::npos)
				mode.erase(mode.find('i'), 1);
			_channels[user.getMessage().getArgs()[0]].setMode(mode);
			response = ":" + user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname() + " MODE " + user.getMessage().getArgs()[0] + " -i\r\n";
		}
		else if (mode == "t")
		{
			mode = _channels[user.getMessage().getArgs()[0]].getMode();
			if (mode.find('t') != std::string::npos)
				mode.erase(mode.find('t'), 1);
			_channels[user.getMessage().getArgs()[0]].setMode(mode);
			response = ":" + user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname() + " MODE " + user.getMessage().getArgs()[0] + " -t\r\n";
		}
		else if (mode == "k")
		{
			mode = _channels[user.getMessage().getArgs()[0]].getMode();
			if (mode.find('k') != std::string::npos)
				mode.erase(mode.find('k'), 1);
			_channels[user.getMessage().getArgs()[0]].setMode(mode);
			_channels[user.getMessage().getArgs()[0]].setPass("");
			response = ":" + user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname() + " MODE " + user.getMessage().getArgs()[0] + " -k\r\n";
		}
		else if (mode == "o")
		{
			if (user.getMessage().getArgs().size() != 3)
			{
				//ERR_NEEDMOREPARAMS
				response = ":" + user.getHostname() + " 461 " + user.getNickname() + " MODE :Not enough parameters\r\n";
				send(user.getFd(), response.c_str(), response.size(), 0);
				std::cout << "[ SERVER ] Message sent to client " << user.getFd() << " ( " << user.getHostname() << " )" << response;
				return;
			}
			userInChannel = false;
			for (std::vector<User>::iterator it = _channels[user.getMessage().getArgs()[0]].getUsers().begin(); it != _channels[user.getMessage().getArgs()[0]].getUsers().end(); it++)
			{
				if (it->getNickname() == user.getMessage().getArgs()[2])
				{
					userInChannel = true;
					break;
				}
			}
			if (!userInChannel)
			{
				//ERR_USERNOTINCHANNEL
				response = ":" + user.getHostname() + " 401 " + user.getNickname() + " " + user.getMessage().getArgs()[2] + " " + user.getMessage().getArgs()[0] + " :User is not in channel\r\n";
				send(user.getFd(), response.c_str(), response.size(), 0);
				std::cout << "[ SERVER ] Message sent to client " << user.getFd() << " ( " << user.getHostname() << " )" << response;
				return;
			}
			for (std::vector<User>::iterator it = _channels[user.getMessage().getArgs()[0]].getOps().begin(); it != _channels[user.getMessage().getArgs()[0]].getOps().end(); it++)
			{
				if (*it == user)
				{
					if (_channels[user.getMessage().getArgs()[0]].getOps().size() == 1)
					{
						_channels[user.getMessage().getArgs()[0]].getOps().push_back(_channels[user.getMessage().getArgs()[0]].getUsers()[0]);
						response = ":" + user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname() + " MODE " + user.getMessage().getArgs()[0] + " +o " + _channels[user.getMessage().getArgs()[0]].getUsers()[0].getNickname() + "\r\n";
						send(user.getFd(), response.c_str(), response.size(), 0);
						std::cout << "[ SERVER ] Message sent to client " << user.getFd() << " ( " << user.getHostname() << " )" << response;
					}
					_channels[user.getMessage().getArgs()[0]].rmOps(*it);
					response = ":" + user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname() + " MODE " + user.getMessage().getArgs()[0] + " -o " + user.getMessage().getArgs()[2] + "\r\n";
					send(it->getFd(), response.c_str(), response.size(), 0);
					std::cout << "[ SERVER ] Message sent to client " << it->getFd() << " ( " << it->getHostname() << " )" << response;
					return;
				}
			}
		}
		else if (mode == "l")
		{
			mode = _channels[user.getMessage().getArgs()[0]].getMode();
			if (mode.find('l') != std::string::npos)
				mode.erase(mode.find('l'), 1);
			_channels[user.getMessage().getArgs()[0]].setMode(mode);
			_channels[user.getMessage().getArgs()[0]].setMaxUsers(0);
			response = ":" + user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname() + " MODE " + user.getMessage().getArgs()[0] + " -l\r\n";
		}
		else
		{
			//RPL_CHANNELMODEIS
			response = ":" + user.getHostname() + " 324 " + user.getNickname() + " " + user.getMessage().getArgs()[0];
			if (!_channels[user.getMessage().getArgs()[0]].getMode().empty())
				response += " :" + _channels[user.getMessage().getArgs()[0]].getMode() + "\r\n";
			else
				response += " :\r\n";
			send(user.getFd(), response.c_str(), response.size(), 0);
			std::cout << "[ SERVER ] Message sent to client " << user.getFd() << " ( " << user.getHostname() << " )" << response;
			return;
		}
	}
	std::vector<User> chUsers = _channels[user.getMessage().getArgs()[0]].getUsers();
	for (std::vector<User>::iterator it = chUsers.begin(); it != chUsers.end(); it++)
	{
		response = ":" + user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname() + " MODE " + user.getMessage().getArgs()[0] + " " + user.getMessage().getArgs()[1] + "\r\n";
		send(it->getFd(), response.c_str(), response.size(), 0);
		std::cout << "[ SERVER ] Message sent to client " << it->getFd() << " ( " << it->getHostname() << " )" << response;
	}
}