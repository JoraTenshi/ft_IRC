#include "../inc/Server.hpp"

int g_signal = 0;

void signalHandler(int signum) { g_signal = signum; }

bool isValidPort(const std::string& portStr) 
{
    for (size_t i = 0; i < portStr.length(); ++i)
{
        if (!isdigit(portStr[i]))
            return false;
    }

    int port = std::atoi(portStr.c_str());
    return port > 0 && port <= 65535;
}

int main(int argc, char* argv[]) 
{
    if (argc != 3) 
	{
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return 1;
    }

    std::string portStr = argv[1];
    std::string password = argv[2];

    if (!isValidPort(portStr)) 
	{
        std::cerr << "Error: Invalid port number." << std::endl;
        return 1;
    }

    int port = std::atoi(portStr.c_str());
	
    std::signal(SIGINT, signalHandler);
    std::signal(SIGQUIT, signalHandler);

    try 
	{
        Server g_server(port, password);
        std::cout << "Starting IRC server on port " << port << std::endl;
        g_server.start();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;

}
