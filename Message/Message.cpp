#include "../inc/Message.hpp"

Message::Message() {}

Message::~Message() {}

std::string Message::getInput(void) const {return _input;}

std::string Message::getMsg(void) const {return _msg;}

std::string Message::getCmd(void) const {return _cmd;}

std::vector<std::string> Message::getArgs(void) const {return _args;}

bool Message::checkCmdEnd(void) 
{
    if (_input.find("\r\n") != std::string::npos)
		return (true);
	return (false);
}

void Message::parseInput(void)
{
    if (!checkCmdEnd())
        return ;
    
    int end = _input.find("\r\n"); 				//Find end of line
    std::string tmp = _input.substr(0, end);	//Get the input line up to the end
    size_t colonPos = tmp.find(':');			//Find the position of the colon, which separates the command and args from the message	
    size_t spacePos = tmp.find(' ');			//Find the position of the first space, which separates the command from the args
    _cmd = tmp.substr(0, spacePos);				//Get the command, which is the first word in the input line
    tmp.erase(0, spacePos + 1);					//Remove the command from the input line, leaving only the args and message
    
	//Parse the input up until the colon, which separates the args from the message
    while (spacePos != std::string::npos && colonPos != 0)
    {
        _args.push_back(tmp.substr(0, spacePos));
        tmp.erase(0, spacePos + 1);
    }

	//If there is no space but a colon, we still need to add the command to the args
	if (spacePos == std::string::npos && colonPos != std::string::npos)
	{
		_args.push_back(tmp.substr(0, colonPos));
		tmp.erase(0, colonPos + 1);
	}
	//If there is no colon, we add the remaining part as the last argument
	else if (colonPos != std::string::npos)
	{
		_args.push_back(tmp.substr(0, colonPos));
		tmp.erase(0, colonPos + 1);
	}

	_msg = tmp;									//The remaining part is the message
	_input.erase(0, end + 2);					//Remove the parsed line from the input
}

std::ostream &operator<<(std::ostream &out, const Message &message)
{
	(void)message;
	out << "Message [" << "" << "]";
	return (out);
}