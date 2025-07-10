#include "../../inc/Server.hpp"

void Server::UserCmd(User &user)
{
	std::string response = "";

	if (user.isAuthenticated())
	    response = ":" + user.getHostname() + " 462 " + user.getNickname() + " :You may not reregister\r\n";
	else if (user.getMessage().getArgs().size() < 3)
		response = ":" + user.getHostname() + " 461 " + user.getNickname() + " USER :Not enough parameters\r\n";
	else if (!user.getPassword() || user.getNickname() == "")
		return ;
	
	if (!response.empty())
	{
		send(user.getFd(), response.c_str(), response.size(), 0);
		std::cout << "[ SERVER ] Message sent to client " << user.getFd() << " ( " << user.getHostname() << " )" << response;
		return ;
	}
	user.setUsername(user.getMessage().getArgs()[0]);
	if (user.getMessage().getArgs().size() > 1)
		user.setRealname(user.getMessage().getArgs()[1]);
	else
		user.setRealname("Unknown");
	response = ":" + user.getNickname() + "!@" + user.getHostname() + " USER :" + user.getUsername() + " " + user.getRealname() + "\r\n";
	user.setAuthenticated(true);

	/**
	 * @brief The first message sent after client registration, this message introduces the client to the network.
	 * 
	 */
	response = ":" + user.getHostname() + " 001 " + user.getNickname() + " :Welcome to the IRC server: "
			+ user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname() + "\r\n";
	send(user.getFd(), response.c_str(), response.size(), 0);
	std::cout << "[ SERVER ] Message sent to client " << user.getFd() << " ( " << user.getHostname() << " )" << response;

	/**
	 * @brief Part of the post-registration greeting, returns the name and software/version of the server the client is currently connected to.
	 * 
	 */
	response = ":" + user.getHostname() + " 002 " + user.getNickname() + " :Your host is " + user.getHostname() + ", running version 1.0\r\n";
	send(user.getFd(), response.c_str(), response.size(), 0);
	std::cout << "[ SERVER ] Message sent to client " << user.getFd() << " ( " << user.getHostname() << " )" << response;

	/**
	 * @brief Part of the post-registration greeting, returns a human-readable date/time that the server was started or created. 
	 * 
	 */
	response = ":" + user.getHostname() + " 003 " + user.getNickname() + " :This server was created 42-42-4242 42:42:42\r\n";
	send(user.getFd(), response.c_str(), response.size(), 0);
	std::cout << "[ SERVER ] Message sent to client " << user.getFd() << " ( " << user.getHostname() << " )" << response;

	/**
	 * @brief Part of the post-registration greeting. Clients SHOULD discover available features using RPL_ISUPPORT tokens rather than the mode letters listed in this reply.
	 * 
	 */
	response = ":" + user.getHostname() + " 004 " + user.getNickname() + " :" + user.getHostname() + " localhost 1.0 :IRC server\r\n";
	send(user.getFd(), response.c_str(), response.size(), 0);
	std::cout << "[ SERVER ] Message sent to client " << user.getFd() << " ( " << user.getHostname() << " )" << response;

	/**
	 * @brief As the maximum number of message parameters to any reply is 15, the maximum number of RPL_ISUPPORT tokens that can be advertised is 13.
	 * 		To counter this, a server MAY issue multiple RPL_ISUPPORT numerics. A server MUST issue at least one RPL_ISUPPORT numeric after client 
	 * 		registration has completed. It MUST be issued before further commands from the client are processed.
	 * 
	 */
	response = ":" + user.getHostname() + " 005 " + user.getNickname() + " :DEFAULT:are supported by this server\r\n";
	send(user.getFd(), response.c_str(), response.size(), 0);
	std::cout << "[ SERVER ] Message sent to client " << user.getFd() << " ( " << user.getHostname() << " )" << response;

	/**
	 * @brief Indicates the start of the Message of the Day to the client.
	 * 		The text used in the last param of this message may vary, and SHOULD be displayed as-is by IRC clients to their users.
	 * 
	 */
	response = ":" + user.getHostname() + " 375 " + user.getNickname() + " :" + user.getHostname() + " Message of the Day - \r\n";
	send(user.getFd(), response.c_str(), response.size(), 0);
	std::cout << "[ SERVER ] Message sent to client " << user.getFd() << " ( " << user.getHostname() << " )" << response;

	/**
	 * @brief When sending the Message of the Day to the client, servers reply with each line of the MOTD as this numeric.
	 * 		MOTD lines MAY be wrapped to 80 characters by the server.
	 * 
	 */
	response = ":" + user.getHostname() + " 372 " + user.getNickname() + " :Life before death. Strenght before weakness. Journey before destination.\r\n";
	send(user.getFd(), response.c_str(), response.size(), 0);
	std::cout << "[ SERVER ] Message sent to client " << user.getFd() << " ( " << user.getHostname() << " )" << response;

	/**
	 * @brief Indicates the end of the Message of the Day to the client. The text used in the last param of this message may vary.
	 * 
	 */
	response = ":" + user.getHostname() + " 376 " + user.getNickname() + " :End of Message of the Day\r\n";
	send(user.getFd(), response.c_str(), response.size(), 0);
	std::cout << "[ SERVER ] Message sent to client " << user.getFd() << " ( " << user.getHostname() << " )" << response;
}