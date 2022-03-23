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

void parse_oper(string message, string* name, string* password) {
	string cmd;
	size_t pos;

	cmd = message;
	pos = cmd.find(' ');
	cmd.erase(0, pos + 1);
	pos = cmd.find(' ');
	*name = cmd.substr(0, pos);
	if (name->empty()) {
		throw BadNumberArgs();
	}
	pos = cmd.find(' ');
	if (pos == SIZE_T_MAX) {
		*password = "";
	}
	else {
		cmd.erase(0, pos + 1);
		*password = cmd.substr(0, cmd.length());
	}
}

void parse_kick(string message, string* chan, vector<string>* user) {
	istringstream iss(message);
	string s;
	vector<string>	arg;
	while (getline(iss, s, ' ')) {
		arg.push_back(s);
	}
	arg.erase(arg.begin());
	*chan = *(arg.begin());
	if (chan->empty()) {
		throw BadNumberArgs();
	}
	arg.erase(arg.begin());
	if (arg.empty()) {
		throw BadNumberArgs();
	}
	*user = arg;
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
			case 0:
				channel_s = message.substr(pos + 1, message.length() - pos - 1);
				Join(params, channel_s);
				break;
			case 1: {
				string msg;
				parse_part(message, &channel_s, &msg);
				Part(params, channel_s, msg);
				break;
			}
			case 2:
				send(params->client_socket, "QUIT", strlen("QUIT"), MSG_DONTWAIT);
				exit(EXIT_SUCCESS);
			case 3:
				Nick(params, channel_s);
				break;
			case 4:
				User(params, channel_s);
				break;
			case 5:
				break;
			case 6: {
				string msg;
				parse_privmsg(message, &channel_s, &msg);
				PrivateMessage(params, channel_s, msg);
				break;
			}
			case 7: {
				string topic;
				parse_topic(message, &channel_s, &topic);
				Topic(params, channel_s, topic);
				break;
			}
			case 8: {
				string name;
				string password;
				try {
					parse_oper(message, &name, &password);
				}
				catch (BadNumberArgs& e) {
					logError(string("Set new operator on server " + params->irc_serv->name), name, e.what());
					// TODO send ERR_NEEDMOREPARAMS
				}
				try {
					(*params->irc_serv->userDB->search(params->user_id)).becomeOper(*params->irc_serv, password);
				}
				catch (BadPasswd& e) {
					// TODO send ERR_PASSWDMISMATCH
					logError(string("Set new operator on server " + params->irc_serv->name), name, e.what());
				}
				catch (exception& e) {
					// TODO send ERR_PASSWDMISMATCH (as default error)
					logError(string("Set new operator on server " + params->irc_serv->name + "(default error)"), name, e.what());
				}
				// TODO send RPL_YOUREOPER
				// TODO send MODE
			}
			case 9: {
				string	chan;
				vector<string>	user;
				parse_kick(message, &chan, &user);
				// ? Maybe try catch this
				string	kicker = params->irc_serv->userDB->search(params->user_id)->getNickName();
				string	usr = *(user.begin());
				try {
					while (!usr.empty() && usr != ":") {
						params->irc_serv->chanDB->search(chan)->userLeave(*(params->irc_serv->userDB->search(usr)));
						log(string(LIGHT_MAGENTA) +  string("User ") +  string(RED) +  usr +  string(LIGHT_BLUE) +  string(" has been kicked out from ") + string(LIGHT_MAGENTA) + string("channel ") + string(RED) + chan + string(LIGHT_BLUE) + " by " +  string(RED) + kicker + string(DEFAULT));
						user.erase(user.begin());
						usr = *(user.begin());
					}
				}
				// TODO catch (thus throw) kick errors (+ multiple kick)
				catch (exception& e) {
					logError(string("Kick user from channel " + chan), *(user.begin()), e.what());
				}
			}
			default:
				throw(InvalidCommand());
		}
	} catch (const InvalidCommand e) {
		std::cerr << e.what() << std::endl;
		return (UNKNOWN_COMMAND);
	}
	return (0);
}
