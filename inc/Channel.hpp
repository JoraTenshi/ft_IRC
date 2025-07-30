#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "User.hpp"
# include "Server.hpp"
# include <vector>
# include <algorithm>

/**
 * @brief Class to store all information about a channel, including its name, password, topic, mode, users, operators, and maximum number of users.
 * 
 */
class Channel
{
	private:
		std::string _name;				//Channel name
		std::string _pass;				//Channel password (if any)
		std::string _topic;				//Channel topic
		std::string _mode;				//Channel mode (e.g., private, secret, etc.)
		std::vector<User> _users;		//List of users in the channel
		std::vector<User> _ops;			//List of operators in the channel, minimum one user must be an operator
		std::vector<User> _invited;		//List of users invited to the channel
		int _maxUsers;					//Maximum number of users allowed in the channel

	public:
		Channel();
		Channel(const std::string &name, std::vector<User> ops);
		Channel(const Channel &src);
		~Channel();
		Channel &operator=(const Channel &src);

		const std::string &getName() const;
		const std::string &getPass() const;
		std::vector<User> &getUsers();
		std::vector<User> &getOps();
		int getMaxUsers() const;
		const std::string &getTopic() const;
		const std::string &getMode() const;
		const std::vector<User> &getInvited() const;

		void setName(const std::string &name);
		void setPass(const std::string &pass);
		void setUsers(const std::vector<User> &users);
		void setOps(const std::vector<User> &ops);
		void setMaxUsers(int maxUsers);
		void setTopic(const std::string &topic);
		void setMode(const std::string &mode);
		void setInvited(const std::vector<User> &invited);

		bool	isUser(User user) const;
		void	rmInvited(User user);
		void	rmUser(User &user);
		void	rmOps(User &user);
		void	updateOps(User &user);
};

#endif