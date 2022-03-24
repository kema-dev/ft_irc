#include "CommandHandler.hpp"

using namespace std;

void parse_part(string message, string *channel_s, string *msg) {
	string cmd;
	size_t pos;

	cmd = message;
	pos = cmd.find(' ');
	cmd.erase(0, pos + 1);
	pos = cmd.find(' ');
	if (pos == string::npos)
		pos = cmd.length();
	*channel_s = cmd.substr(0, pos);
	cout << cmd << "'" << endl;
	cmd.erase(0, pos + 1);
	cout << cmd << "'" << endl;
	if (cmd.empty())
		return;
	if (cmd.find(':') != string::npos)
		*msg = cmd.substr(1, cmd.length());
	else
		*msg = cmd.substr(0, cmd.length());
}

void parse_topic(string message, string *channel_s, string *topic) {
	string cmd;
	size_t pos;

	cmd = message;
	pos = cmd.find(' ');
	cmd.erase(0, pos + 1);
	pos = cmd.find(' ');
	if (pos == string::npos)
		pos = cmd.length();
	*channel_s = cmd.substr(0, pos);
	cout << cmd << "'" << endl;
	cmd.erase(0, pos + 1);
	cout << cmd << "'" << endl;
	if (cmd.empty())
		return;
	if (cmd.find(':') != string::npos)
		*topic = cmd.substr(1, cmd.length());
	else
		*topic = cmd.substr(0, cmd.length());
}

void parse_privmsg(string message, string *channel_s, string *msg) {
	string cmd;
	size_t pos;

	cmd = message;
	pos = cmd.find(' ');
	cmd.erase(0, pos + 1);
	pos = cmd.find(' ');
	*channel_s = cmd.substr(0, pos);
	cmd.erase(0, pos + 1);
	*msg = cmd.substr(0, cmd.length());
}

void parse_oper(string message, string* password) {
	string cmd;
	size_t pos;

	cmd = message;
	pos = cmd.find(' ');
	cmd.erase(0, pos + 1);
	*password = string(cmd);
}

void parse_kick(string message, string* chan, vector<string>* user, string* kickMsg, Server* serv) {
	// FIXME parse on "," between users / channels
	istringstream iss(message);
	string s;
	while (getline(iss, s, ' ')) {
		user->push_back(s);
	}
	user->erase(user->begin());
	*chan = *(user->begin());
	if (chan->empty()) {
		throw NeedMoreParams();
	}
	user->erase(user->begin());
	if (user->empty()) {
		throw NeedMoreParams();
	}
	try {
		serv->userDB->search(user->back());
	}
	catch (exception& e) {
		*kickMsg = user->back();
	}
}

int command_check(string message, t_params *params) {
	try {
		if (message.empty() == true)
			throw(EmptyCommand());
	} catch (const EmptyCommand e) {
		std::cerr << e.what() << std::endl;
		return (CLIENT_DISCONNECTED);
	}
	vector<string> commands;
	commands.push_back("JOIN");
	commands.push_back("PART");
	commands.push_back("QUIT");
	commands.push_back("NICK");
	commands.push_back("USER");
	commands.push_back("PONG");
	commands.push_back("PRIVMSG");
	commands.push_back("TOPIC");
	commands.push_back("OPER");
	commands.push_back("KICK");
	try {
		size_t pos;
		string cmd;
		string channel_s;
		int index;

		cmd = message;
		pos = cmd.find(' ');
		if (pos == string::npos)
			throw(InvalidCommand());
		cmd.erase(pos, cmd.length() - pos);
		vector<string>::iterator it = find(commands.begin(), commands.end(), cmd);
		if (it != commands.end())
			index = it - commands.begin();
		else {
			throw(InvalidCommand());
		}
		switch (index) {
			// ? JOIN
			case 0:
				channel_s = message.substr(pos + 1, message.length() - pos - 1);
				Join(params, channel_s);
				break;
			// ? PART
			case 1: {
				string msg;
				parse_part(message, &channel_s, &msg);
				Part(params, channel_s, msg);
				break;
			}
			// ? QUIT
			case 2:
				send(params->client_socket, "QUIT", strlen("QUIT"), MSG_DONTWAIT);
				exit(EXIT_SUCCESS);
			// ? NICK
			case 3:
				Nick(params, channel_s);
				break;
			// ? USER
			case 4:
				User(params, channel_s);
				break;
			// ? PONG
			case 5:
				break;
			// ? PRIVMSG
			case 6: {
				string msg;
				parse_privmsg(message, &channel_s, &msg);
				PrivateMessage(params, channel_s, msg);
				break;
			}
			// ? TOPIC
			case 7: {
				string topic;
				parse_topic(message, &channel_s, &topic);
				Topic(params, channel_s, topic);
				break;
			}
			// ? OPER
			case 8: {
				Oper(message, params);
				break;
			}
			// ? KICK
			case 9: {
				Kick(message, params);
			}
			default:
				throw(InvalidCommand());
		}
	} catch (const InvalidCommand e) {
		// std::cerr << e.what() << std::endl;
		return (UNKNOWN_COMMAND);
	}
	return (0);
}
