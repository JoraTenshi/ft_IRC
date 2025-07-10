#include "../../inc/Server.hpp"

void Server:: executeCommand(User &user)
{
	std::cout << "[ CLIENT ] Message received from client: " << user.getFd() << "( " << user.getHostname() << " ) - " << user.getMessage().getInput();
	for (size_t i = 0; i < user.getMessage().getArgs().size(); ++i)
		std::cout << " " << user.getMessage().getArgs()[i];
	if (!user.getMessage().getMsg().empty())
		std::cout << " : " << user.getMessage().getMsg();
	std::cout << std::endl;

	if (user.getMessage().getCmd() == "PASS")
		PassCmd(user);
	else if (!user.getPassword())
		std::cout << " [ SERVER ] Client " << user.getFd() << " has not sent correct password. Denied." << std::endl;
	else if (user.getMessage().getCmd() == "NICK")
		NickCmd(user);
	else if (user.getMessage().getCmd() == "USER")
		UserCmd(user);
	else if (!user.isAuthenticated())
		std::cout << "[ SERVER ] Client " << user.getFd() << " is not authenticated. Denied." << std::endl;
	else if (user.getMessage().getCmd() == "JOIN")
		JoinCmd(user);
}