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

void parse_kick(string message, string* chan, string* user, string* kickMsg, Server* serv) {
	istringstream iss(message);
	string s;
	getline(iss, s, ' '); // ? Skip "KICK"
	getline(iss, s, ' ');
	*chan = s;
	if (chan->empty()) {
		throw NeedMoreParams();
	}
	getline(iss, s, ' ');
	*user = s;
	if (user->empty()) {
		throw NeedMoreParams();
	}
	getline(iss, s, '\n');
	if (s[0] == ':')
		s = &s[1];
	try {
		serv->userDB->search(s);
	}
	catch (exception& e) {
		if (!s.empty()) {
			*kickMsg = s;
		}
	}
}

void parse_mode(string message, string* user, int* req_op, int* req_away) {
	istringstream iss(message);
	string s;
	int i;
	getline(iss, s, ' '); // ? Skip "MODE"
	getline(iss, s, ' ');
	*user = s;
	if (user->empty()) {
		throw NeedMoreParams();
	}
	*req_op = 0;
	*req_away = 0;
	getline(iss, s, '\n');
	if ((s[0] != '+') && (s[0] != '-')) {
		throw NeedMoreParams();
	}
	if (s[0] == '+') {
		i = ACTION_ADD;
		s.erase(0, 1);
	}
	else if (s[0] == '-') {
		i = ACTION_REMOVE;
		s.erase(0, 1);
	}
	if (!s.empty() && s.find('o') != string::npos) {
		*req_op = 1;
		s.erase(s.find('o'), 1);
	}
	if (!s.empty() && s.find('a') != string::npos) {
		*req_away = 1;
		s.erase(s.find('a'), 1);
	}
	if (!s.empty()) {
		throw UnknownParam();
	}
	if (*req_op == 0 && *req_away == 0) {
		throw NeedMoreParams();
	}
	*req_op *= i;
	*req_away *= i;
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
	commands.reserve(20);
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
	commands.push_back("MODE");
	try {
		size_t pos;
		string cmd;
		string channel_s;
		int index = -1;

		cmd = message;
		pos = cmd.find(' ');
		if (pos == string::npos) {
			cmd = message;
		}
		else {
			cmd.erase(pos, cmd.length() - pos);
		}
		vector<string>::iterator it = find(commands.begin(), commands.end(), cmd);
		if (it != commands.end())
			index = it - commands.begin();
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
			case 5: {
				// TODO handle it + send PING every minute
				break;
			}
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
				break;
			}
			// ? MODE
			case 10: {
				cerr << "MODE" << endl;
				Mode(message, params);
				break;
			}
			default: {
				throw(InvalidCommand());
			}
		}
	} catch (const InvalidCommand e) {
		logError("Received command", message, e.what());
		return (UNKNOWN_COMMAND);
	}
	return (0);
}
