#ifndef USER_HPP
# define USER_HPP

# include <iostream>
# include "Message.hpp"
# include <string>
# include <poll.h>


/**
 * @brief User class will be used to store all the information of a user connected to the IRC server. It also has
 *        a Message attribute to handle the messages sent by the user, that is a pointer to a Message object.
 * 
 */
class User
{
	private:
		std::string _nickname; 				// User's nickname
		std::string _username; 				// User's username
		std::string _realname; 				// User's real name
		std::string _password; 				// User's password
		std::string _hostname; 				// User's IP address
		int 		_fd; 					// File descriptor for the user's socket
		bool 		_isAuthenticated; 		// Flag to check if the user is authenticated
		Message 	_message; 				// Message object to handle user messages

	public:
		User();
		User(int fd);
		User(const User &src);
		~User();
		User &operator=(const User &src);

		std::string getNickname() const;
		std::string getUsername() const;
		std::string getRealname() const;
		std::string getPassword() const;
		std::string getHostname() const;
		const Message &getMessage() const;
		Message &getMessage();
		int getFd() const;
		bool isAuthenticated() const;

		void setNickname(const std::string &nickname);
		void setUsername(const std::string &username);
		void setRealname(const std::string &realname);
		void setPassword(const std::string &password);
		void setHostname(const std::string &hostname);
		void setFd(int fd);
		void setAuthenticated(bool authenticated);
};

std::ostream &operator<<(std::ostream &out, const User &user);

#endif