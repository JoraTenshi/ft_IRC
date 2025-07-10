# include "../inc/Channel.hpp"

Channel::Channel() : _name(""), _pass(""), _topic(""), _mode(""), _maxUsers(0) {}
Channel::Channel(const std::string &name, std::vector<User> ops) : _name(name), _pass(""), _topic(""), _mode(""), _users(), _ops(ops), _maxUsers(0) {}
Channel::Channel(const Channel &src) {*this = src;}
Channel::~Channel() {}

Channel &Channel::operator=(const Channel &src) {
	if (this != &src) {
		_name = src._name;
		_pass = src._pass;
		_topic = src._topic;
		_mode = src._mode;
		_users = src._users;
		_ops = src._ops;
		_maxUsers = src._maxUsers;
	}
	return *this;
}

const std::string &Channel::getName() const { return _name; }
const std::string &Channel::getPass() const { return _pass; }
std::vector<User> &Channel::getUsers() { return _users; }
std::vector<User> &Channel::getOps() { return _ops; }
int Channel::getMaxUsers() const { return _maxUsers; }
const std::string &Channel::getTopic() const { return _topic; }
const std::string &Channel::getMode() const { return _mode; }
const std::vector<User> &Channel::getInvited() const { return _invited; }

void Channel::setName(const std::string &name) { _name = name; }
void Channel::setPass(const std::string &pass) { _pass = pass; }
void Channel::setUsers(const std::vector<User> &users) { _users = users; }
void Channel::setOps(const std::vector<User> &ops) { _ops = ops; }
void Channel::setMaxUsers(int maxUsers) { _maxUsers = maxUsers; }
void Channel::setTopic(const std::string &topic) { _topic = topic; }
void Channel::setMode(const std::string &mode) { _mode = mode; }
void Channel::setInvited(const std::vector<User> &invited) { _invited = invited; }

bool	Channel::isUser(User user) const { return (std::find(_users.begin(), _users.end(), user) != _users.end()); }

void	Channel::rmInvited(User user) { _invited.erase(std::remove(_invited.begin(), _invited.end(), user), _invited.end()); }

void	Channel::rmUser(User &user) {
	rmOps(user);
	std::vector<User> &users = getUsers();
	users.erase(std::remove(users.begin(), users.end(), user), users.end());
}

void	Channel::rmOps(User &user) {
	std::vector<User> &ops = getOps();
	ops.erase(std::remove(ops.begin(), ops.end(), user), ops.end());
}