#include "ServerManip.hpp"

using namespace std;

ServerManip::ServerManip() {
}

ServerManip::~ServerManip() {
}

void ServerManip::setInput(string input) {
	_input = input;
}
void ServerManip::setBuf(string buf) {
	_buf = buf;
}

string ServerManip::getInput(void) {
	return _input;
}
string ServerManip::getBuf(void) {
	return _buf;
}

size_t ServerManip::ft_find(string input) {
	size_t pos_r = input.find("\r");
	size_t pos_n = input.find("\n");
	if (pos_r == string::npos && pos_n != string::npos)
		return (pos_n);
	else if (pos_r != string::npos && pos_n == string::npos)
		return (pos_r);
	else if (pos_r == string::npos && pos_n == string::npos)
		return (0);
	else {
		if (pos_r < pos_n)
			return (pos_r);
		else
			return (pos_n);
	}
}

int ServerManip::check_password(string input, t_KDescriptor *desc, int socket) {
	string hash;
	if (input.length() - strlen("PASS ") < 1)
	{
		logError("Checking password", "Failure", "Bad number of arguments");
		send(socket, ":0 461 0 PASS :Not enough parameters\r\n", strlen(":0 461 0 PASS :Not enough parameters\r\n"),0);
		return(1);
	}
	if (input.find("PASS") != std::string::npos) {
		std::string password = input.substr(strlen("PASS "), input.length() - strlen("PASS "));
		try {
			hash = sha256(password);
		} catch (exception& e) {
			logError(string("Invalid password"), "", e.what());
			throw BadPasswd();
		}
		if (hash != desc->server->getHash()) {
			logError(string("Invalid password"), "", "Bad password");
			throw BadPasswd();
		}
	} else {
		throw BadPasswd();
	}
	desc->user->setActiveStatus(PASSED);
	return (0);
}

string ServerManip::parseNickname(string input) {
	string nickname;

	if (input.empty())
		return (NULL);
	if (input.find("NICK ") == std::string::npos)
		return (NULL);
	else
		nickname = input.substr(strlen("NICK "), input.length() - strlen("NICK ") - (input.length() - ft_find(input)));
	return (nickname);
}

ssize_t ServerManip::createUser(string input, t_KDescriptor* desc) {
	ssize_t id;
	string fullname;
	char username[26];
	char hostname[26];
	char servername[26];
	string cmd;

	if (input.empty() || desc->user->getNickName().empty())
		return (-1);
	cmd = input;
	cmd.erase(0, strlen("USER "));
	sscanf(cmd.c_str(), "%25s %25s %25s", username, hostname, servername);
	cmd.erase(0, cmd.length() - (cmd.length() - cmd.find(":")) + 1);
	if (cmd.find(' ') == string::npos)
		return (-1);
	fullname = cmd.substr(0, cmd.length());
	try {
		id = desc->server->addUser(desc, username, fullname, hostname, servername);
		// cout << "User created!" << endl;
	} catch (exception& e) {
		logError(string("Adding user on server " + string(servername)), username, e.what());
		return (-1);
	}
	return (id);
}

vector<string> ServerManip::parseInput(string input) {
	istringstream iss(input);
	string s;
	vector<string> ret;

	while (getline(iss, s, '\n')) {
		string buf;
		try {
			buf = s.replace(s.find("\r"), 1, "");
		} catch (out_of_range& e) {
			buf = s;
		}
		ret.push_back(buf);
	}
	return (ret);
}
