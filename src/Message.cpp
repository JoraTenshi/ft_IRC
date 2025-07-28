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
	
    if (!_cmd.empty())
	    _cmd.clear();
    if (!_args.empty())
		_args.clear();
    if (!_msg.empty())
	    _msg.clear();
}

/* void Message::parseInput(void)
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
} */


//con este parse del imput funciona con el hexchat pero da segmentation fault para el nc en el paso de USER
void Message::parseInput(void) {
    if (!checkCmdEnd())
        return ;
    
    int end = _input.find("\r\n");
    std::string tmp = _input.substr(0, end);
    
    // Extraer comando
    size_t spacePos = tmp.find(" ");
    if (spacePos != std::string::npos) {
        _cmd = tmp.substr(0, spacePos);
        tmp = tmp.substr(spacePos + 1);
    } else {
        _cmd = tmp;
        _msg = "";
        return;
    }
    
    // Buscar mensaje (parte después de ':')
    size_t colonPos = tmp.find(":");
    std::string argsPart;
    
    if (colonPos != std::string::npos) {
        argsPart = tmp.substr(0, colonPos);
        // Quitar espacios al final de argsPart
        while (!argsPart.empty() && argsPart[argsPart.length() - 1] == ' ')
            argsPart = argsPart.substr(0, argsPart.length() - 1);
        _msg = tmp.substr(colonPos + 1);
    } else {
        argsPart = tmp;
        _msg = "";
    }
    
    // Parsear argumentos correctamente
    while (!argsPart.empty()) {
        // Buscar primer espacio
        size_t space = argsPart.find(" ");
        if (space != std::string::npos) {
            std::string arg = argsPart.substr(0, space);
            if (!arg.empty()) {
                _args.push_back(arg);
            }
            // Eliminar el argumento procesado y espacios adicionales
            argsPart = argsPart.substr(space + 1);
            while (!argsPart.empty() && argsPart[0] == ' ')
                argsPart = argsPart.substr(1);
        } else {
            // No hay más espacios, añadir el resto como último argumento
            if (!argsPart.empty())
                _args.push_back(argsPart);
            break;
        }
    }
}

std::ostream &operator<<(std::ostream &out, const Message &message)
{
	(void)message;
	out << "Message [" << "" << "]";
	return (out);
}