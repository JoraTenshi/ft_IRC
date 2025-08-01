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
			response += " +" + _channels[user.getMessage().getArgs()[0]].getMode() + "\r\n";
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

	if (user.getMessage().getArgs()[1].size() < 2 || (user.getMessage().getArgs()[1][0] != '+' && user.getMessage().getArgs()[1][0] != '-'))
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

    std::string modes;
    std::vector<std::string> params;
    size_t i = 1;
    for (; i < user.getMessage().getArgs().size(); ++i) {
        if (user.getMessage().getArgs()[i].empty() ||
            (user.getMessage().getArgs()[i][0] != '+' && user.getMessage().getArgs()[i][0] != '-'))
            break;
        modes += user.getMessage().getArgs()[i];
    }

    for (; i < user.getMessage().getArgs().size(); ++i)
        params.push_back(user.getMessage().getArgs()[i]);

    std::string channel = user.getMessage().getArgs()[0];
    std::string modeStr = _channels[channel].getMode();
    std::string replyModes;
    std::vector<std::string> replyParams;
    char sign = 0;
    size_t paramIdx = 0;

    for (size_t i = 0; i < modes.size(); ++i)
    {
       	if (modes[i] == '+' || modes[i] == '-')
		{
			sign = modes[i];
			if (replyModes.empty() || replyModes[replyModes.size() - 1] != sign)
				replyModes += sign;
			continue;
		}
		if (sign == 0)
			continue;


        switch (modes[i])
        {
            case 'i':
                if (sign == '+') 
				{
                    if (modeStr.find('i') == std::string::npos)
                    	modeStr += 'i';
                } 
				else
				{
                    size_t pos = modeStr.find('i');
					if (pos != std::string::npos) 
						modeStr.erase(pos, 1);
				}
				replyModes += modes[i];
                break;
            case 't':
                if (sign == '+')
				{
                    if (modeStr.find('t') == std::string::npos)
						modeStr += 't';
                } 
				else
				{
                    size_t pos = modeStr.find('t');
                    if (pos != std::string::npos)
						modeStr.erase(pos, 1);
                }
				replyModes += modes[i];
                break;
            case 'k':
                if (sign == '+') 
				{
                    if (paramIdx >= params.size()) 
					{
                        response = ":" + user.getHostname() + " 461 " + user.getNickname() + " MODE :Not enough parameters\r\n";
                        send(user.getFd(), response.c_str(), response.size(), 0);
                        return;
                    }
                    if (modeStr.find('k') == std::string::npos)
                        modeStr += 'k';
                    _channels[channel].setPass(params[paramIdx]);
                    replyParams.push_back(params[paramIdx]);
                    paramIdx++;
                } 
				else 
				{
                    size_t pos = modeStr.find('k');
                    if (pos != std::string::npos)
						modeStr.erase(pos, 1);
                    _channels[channel].setPass("");
                }
				replyModes += modes[i];
                break;
            case 'l':
                if (sign == '+') 
				{
                    if (paramIdx >= params.size())
					{
                        response = ":" + user.getHostname() + " 461 " + user.getNickname() + " MODE :Not enough parameters\r\n";
                        send(user.getFd(), response.c_str(), response.size(), 0);
                        return;
                    }
                    int maxUsers = std::atoi(params[paramIdx].c_str());
                    if (maxUsers < 1)
						break;
                    if (modeStr.find('l') == std::string::npos)
						modeStr += 'l';
                    _channels[channel].setMaxUsers(maxUsers);
                    replyParams.push_back(params[paramIdx]);
                    paramIdx++;
                }
				else
				{
                    size_t pos = modeStr.find('l');
                    if (pos != std::string::npos)
						modeStr.erase(pos, 1);
                    _channels[channel].setMaxUsers(0);
                }
				replyModes += modes[i];
                break;
            case 'o':
                if (paramIdx >= params.size())
				{
                    response = ":" + user.getHostname() + " 461 " + user.getNickname() + " MODE :Not enough parameters\r\n";
                    send(user.getFd(), response.c_str(), response.size(), 0);
                    return;
                }
				else
                {
                    std::string targetNick = params[paramIdx];
                    paramIdx++;
                    User* targetUser = NULL;
                    for (std::vector<User>::iterator it = _channels[channel].getUsers().begin();
                        it != _channels[channel].getUsers().end(); ++it)
                    {
                        if (it->getNickname() == targetNick)
						{
                            targetUser = &(*it);
                            break;
                        }
                    }
                    if (!targetUser)
					{
                        response = ":" + user.getHostname() + " 401 " + user.getNickname() + " " + targetNick + " " + channel + " :User not in that channel\r\n";
                        send(user.getFd(), response.c_str(), response.size(), 0);
                        return;
                    }
                    std::vector<User>& chOps = _channels[channel].getOps();
                    if (sign == '+')
					{
                        bool alreadyOp = false;
                        for (size_t j = 0; j < chOps.size(); ++j)
                            if (chOps[j].getNickname() == targetUser->getNickname())
                                alreadyOp = true;
                        if (!alreadyOp) chOps.push_back(*targetUser);
                        _channels[channel].setOps(chOps);
                        replyParams.push_back(targetUser->getNickname());
                    }
					else
					{
						int removedIndex = -1;
						for (std::vector<User>::iterator it = _channels[user.getMessage().getArgs()[0]].getOps().begin(); it != _channels[user.getMessage().getArgs()[0]].getOps().end(); it++)
						{
							if (it->getNickname() == user.getMessage().getArgs()[2])
							{
								removedIndex = it - _channels[user.getMessage().getArgs()[0]].getOps().begin();
								break;
							}
						}

                        if (removedIndex != -1)
						{
							_channels[user.getMessage().getArgs()[0]].rmOps(_channels[user.getMessage().getArgs()[0]].getOps()[removedIndex]);
							response = ":" + user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname() + " MODE " + user.getMessage().getArgs()[0] + " -o " + user.getMessage().getArgs()[2] + "\r\n";
							std::vector<User> chUsers = _channels[user.getMessage().getArgs()[0]].getUsers();
							for (std::vector<User>::iterator it = chUsers.begin(); it != chUsers.end(); ++it)
							{
								send(it->getFd(), response.c_str(), response.size(), 0);
								std::cout << "[ SERVER ] Message sent to client " << it->getFd() << " ( " << it->getHostname() << " )" << response;
							}
						}
						replyParams.push_back(targetUser->getNickname());

						if (_channels[channel].getOps().size() == 0 && !_channels[channel].getUsers().empty()) 
						{
							User& newOp = _channels[channel].getUsers()[0];
							chOps.push_back(newOp);
							_channels[channel].setOps(chOps);
							replyModes += "+o-";
							continue;
						}
						
					}
                }
				replyModes += modes[i];
                break;
            default:
                break;
        }
    }
    _channels[channel].setMode(modeStr);

    // Notificar a todos los usuarios del canal
    std::string finalReply = ":" + user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname() +
        " MODE " + channel + " " + replyModes;
    for (size_t i = 0, pi = 0; i < replyModes.size(); ++i) {
        if (replyModes[i] == 'k' || replyModes[i] == 'l' || replyModes[i] == 'o') {
            if (pi < replyParams.size())
                finalReply += " " + replyParams[pi++];
        }
    }
    finalReply += "\r\n";
    std::vector<User> chUsers = _channels[channel].getUsers();
    for (std::vector<User>::iterator it = chUsers.begin(); it != chUsers.end(); ++it)
        send(it->getFd(), finalReply.c_str(), finalReply.size(), 0);
}