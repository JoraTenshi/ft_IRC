#include "../inc/User.hpp"

User::User() :_isAuthenticated(false) {}

User::User(int fd) : _fd(fd), _isAuthenticated(false) {}

User::User(const User &src) {*this = src; }

User::~User() {}

User &User::operator=(const User &src) {
	if (this != &src) {
		_nickname = src._nickname;
		_username = src._username;
		_realname = src._realname;
		_password = src._password;
		_hostname = src._hostname;
		_fd = src._fd;
		_isAuthenticated = src._isAuthenticated;
		_message = src._message;
	}
	return *this;
}

std::string User::getNickname() const { return _nickname; }
std::string User::getUsername() const { return _username; }
std::string User::getRealname() const { return _realname; }
std::string User::getPassword() const { return _password; }
std::string User::getHostname() const { return _hostname; }
const Message &User::getMessage() const { return _message; }
Message &User::getMessage() { return _message; }
int User::getFd() const { return _fd; }
bool User::isOp() const { return _op; }


bool User::isAuthenticated() const { return _isAuthenticated; }


void User::setNickname(const std::string &nickname) { _nickname = nickname; }
void User::setUsername(const std::string &username) { _username = username; }
void User::setRealname(const std::string &realname) { _realname = realname; }
void User::setPassword(const std::string &password) { _password = password; }
void User::setHostname(const std::string &hostname) { _hostname = hostname; }
void User::setFd(int fd) { _fd = fd; }
void User::setAuthenticated(bool authenticated) { _isAuthenticated = authenticated; }
void User::setOp(bool op) { _op = op; }

std::ostream &operator<<(std::ostream &out, const User &user) {
	out << "User Info:" << std::endl;
	out << "Nickname: " << user.getNickname() << std::endl;
	out << "Username: " << user.getUsername() << std::endl;
	out << "Realname: " << user.getRealname() << std::endl;
	out << "Password: " << user.getPassword() << std::endl;
	out << "IP: " << user.getHostname() << std::endl;
	out << "File Descriptor: " << user.getFd() << std::endl;
	out << "Authenticated: " << (user.isAuthenticated() ? "Yes" : "No") << std::endl;
	out << "Message: " << user.getMessage().getInput() << std::endl; // Assuming _message is accessible
	return out;
}