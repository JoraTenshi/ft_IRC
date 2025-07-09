#include "../../inc/Server.hpp"

void Server:: executeCommand(User &user)
{
	//bool authenticated = user.isAuthenticated();

	std::cout << "[ CLIENT ] Message received from client: " << user.getFd() << "( " << user.getHostname() << " ) - " << user.getMessage().getInput();
	for (size_t i = 0; i < user.getMessage().getArgs().size(); ++i)
		std::cout << " " << user.getMessage().getArgs()[i];
	if (!user.getMessage().getMsg().empty())
		std::cout << " : " << user.getMessage().getMsg();
	std::cout << std::endl;

	if (user.getMessage().getCmd() == "JOIN")
		JoinCmd(user);
}