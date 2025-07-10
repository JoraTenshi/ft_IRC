#include "../inc/Message.hpp"

Message::Message() {}

Message::Message(const Message &src) { *this = src; }

Message &Message::operator=(const Message &src) 
{
	if (this != &src) 
	{
		_input = src._input;
		_cmd = src._cmd;
		_args = src._args;
		_msg = src._msg;
	}
	return *this;
}

Message::~Message() {}

std::string Message::getInput(void) const {return _input;}

std::string Message::getMsg(void) const {return _msg;}

std::string Message::getCmd(void) const {return _cmd;}

std::vector<std::string> Message::getArgs(void) const {return _args;}

void	Message::setInput(std::string input) { _input = input; }

bool Message::checkCmdEnd(void) 
{
    if (_input.find("\r\n") != std::string::npos)
		return (true);
	return (false);
}

void	Message::clear()
{
	size_t pos = _input.find("\r\n");
	if (pos != std::string::npos)
		_input = _input.substr(pos + 2);
	else
		_input.clear();
		
	_cmd.clear();
	_args.clear();
	_msg.clear();
}

void Message::parseInput(void)
{
    if (!checkCmdEnd())
        return ;
    
	//If using NetCat, please ensure to use -C flag to enable \r\n at the end of the input.
    int end = _input.find("\r\n"); 				//Find end of line
    std::string tmp = _input.substr(0, end);	//Get the input line up to the end
	
	_cmd = tmp.substr(0, tmp.find(" "));				//Get the command, which is the first word in the input line
    tmp = tmp.substr(tmp.find(" ") + 1, tmp.length());	//Remove the command from the input line, leaving only the args and message
    
	//Parse the input up until the colon, which separates the args from the message
    while (tmp.find(" ") != std::string::npos && tmp.find(" ") < tmp.find(":"))
    {
        _args.push_back(tmp.substr(0, tmp.find(" ")));
        tmp.erase(0, tmp.find(" ") + 1);
    }

	//If there is no space but a colon, we still need to add the command to the args
	if (tmp.find(" ") == std::string::npos)
	{
		_args.push_back(tmp.substr(0, tmp.length()));
		tmp.erase(0, tmp.length());
	}
	//If there is no colon, we add the remaining part as the last argument
	if (tmp.find(":") != std::string::npos)
		_input = tmp.erase(0, tmp.find(":") + 1);
	else
		_input = "";
}

std::ostream &operator<<(std::ostream &out, const Message &message)
{
	(void)message;
	out << "Message [" << "" << "]";
	return (out);
}