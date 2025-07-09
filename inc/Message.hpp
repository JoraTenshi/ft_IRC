#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <iostream>
# include <string>
# include <vector>

/**
 * @brief Class used by every instance of User class to parse the input messages from the user and process them.
 * 
 */
class Message 
{
	private:
		//This is the full inpout line we are parsing
		std::string 				_input; 
		//This is the command we are parsing, e.g. JOIN, PART, PRIVMSG, etc. It is the first word in a message.
		std::string 				_cmd;
		//This are the arguments of the command, e.g. the channel name in JOIN, the message in PRIVMSG, etc. They are between command and the message, separated by colon.
		std::vector<std::string> 	_args;
		//This is the message we are parsing, the part that is just plain text. It comes after the colon and is the last part of the message.
		std::string 				_msg;				
	
	public:
		Message();
		Message(const Message &src);
		~Message();
		Message &operator=(const Message &src);

		std::string getInput(void) const;
		std::string getMsg(void) const;
		std::string getCmd(void) const;
		std::vector<std::string> getArgs(void) const;

		void	setInput(std::string input);

		void clear();
		bool checkCmdEnd(void);
		void parseInput(void);
};
std::ostream& operator<<(std::ostream& os, const Message& parser);

#endif