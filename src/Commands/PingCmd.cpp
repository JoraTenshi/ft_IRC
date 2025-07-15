#include "../../inc/Server.hpp"

void Server::PingCmd(User &user)
{
	std::string response;

	if (user.getMessage().getArgs().size() != 1 || user.getMessage().getArgs()[0] == "PING")
		response = ":" + user.getHostname() + " 461 " + user.getNickname() + " PING :Not enough parameters\r\n";
	else
		response = ":" + user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname() + " PONG " + user.getMessage().getArgs()[0] + "\r\n";

	send(user.getFd(), response.c_str(), response.size(), 0);
	std::cout << "[ SERVER ] Message sent to client " << user.getFd() << " ( " << user.getHostname() << " )" << response;
}