#include "../../inc/Server.hpp"

void Server::executeCommand(User &user)
{
	std::cout << "[ CLIENT ] Message received from client: " << user.getFd() << "( " << user.getHostname() << " ) - " << user.getMessage().getInput();
	if (!user.getMessage().getMsg().empty())
		std::cout << " : " << user.getMessage().getMsg();
	std::cout << std::endl;

/* 	if (user.getMessage().getCmd() == "CAP")
        return; */ //pruebas para hexchat

	if (user.getMessage().getCmd() == "PASS" || user.getMessage().getCmd() == "pass")
		PassCmd(user);
	else if (!user.getPassword())
		std::cout << "[ SERVER ] Client " << user.getFd() << " has not sent correct password. Denied." << std::endl;
	else if (user.getMessage().getCmd() == "NICK" || user.getMessage().getCmd() == "nick")
		NickCmd(user);
	else if (user.getMessage().getCmd() == "USER" || user.getMessage().getCmd() == "user")
		UserCmd(user);
	else if (!user.isAuthenticated())
		std::cout << "[ SERVER ] Client " << user.getFd() << " is not authenticated. Denied." << std::endl;
	else if (user.getMessage().getCmd() == "JOIN" || user.getMessage().getCmd() == "join")
		JoinCmd(user);
	else if (user.getMessage().getCmd() == "PART" || user.getMessage().getCmd() == "part")
		PartCmd(user);
	else if (user.getMessage().getCmd() == "PRIVMSG" || user.getMessage().getCmd() == "privmsg")
		PrivMsgCmd(user);
	else if (user.getMessage().getCmd() == "PING" || user.getMessage().getCmd() == "ping")
		PingCmd(user);
 	else if (user.getMessage().getCmd() == "MODE" || user.getMessage().getCmd() == "mode")
		ModeCmd(user);
	else if (user.getMessage().getCmd() == "QUIT" || user.getMessage().getCmd() == "quit")
		QuitCmd(user);
	else if (user.getMessage().getCmd() == "TOPIC" || user.getMessage().getCmd() == "topic")
		TopicCmd(user);
	else if (user.getMessage().getCmd() == "INVITE" || user.getMessage().getCmd() == "invite")
		InviteCmd(user);
	else if (user.getMessage().getCmd() == "WHO" || user.getMessage().getCmd() == "who")
		WhoCmd(user);
	else if (user.getMessage().getCmd() == "KICK" || user.getMessage().getCmd() == "kick")
		KickCmd(user);
}