# include "../../inc/Server.hpp"

/**
 * @brief This handles the user inputing the correct password to authenticate with the server.
 * 
 * @param user 
 */
void Server::PassCmd(User &user)
{
	std::string response = "";

	if (user.getMessage().getArgs().size() < 1 || user.getMessage().getArgs()[0] == "PASS")
		//ERR_NEEDMOREPARAMS
		response = ":" + user.getHostname() + " 461 " + user.getNickname() + " PASS :Not enough parameters\r\n";
	else if (user.getMessage().getArgs()[0] != _password)
		//ERR_PASSWDMISMATCH
		response = ":" + user.getHostname() + " 464 " + user.getNickname() + " :Password incorrect\r\n";
	else if (user.getPassword())
		//ERR_ALREADYREGISTERED
		response = ":" + user.getHostname() + " 462 " + user.getNickname() + " :You may not reregister\r\n";

	if (!response.empty())
	{
		send(user.getFd(), response.c_str(), response.size(), 0);
		std::cout << "[ SERVER ] Message sent to client " << user.getFd() << "( " << user.getHostname() << " )" << response;
	}
	else
		user.setPassword(true);
}