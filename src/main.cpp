#include "../inc/Server.hpp"
#include <csignal>

Server  *g_server = NULL;

void signalHandler(int signal) {
    if (signal == SIGINT && g_server) {
        std::cout << "\nShutting down server..." << std::endl;
        g_server->stop();
        delete g_server;
        exit(0);
    }
}

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
    
    try 
	{
        g_server = new Server(port, password);
        std::cout << "Starting IRC server on port " << port << std::endl;
        g_server->start();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        if (g_server)
            delete g_server;
        return 1;
    }

    return 0;

}
