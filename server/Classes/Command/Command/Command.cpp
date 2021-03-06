#include "../Command/Command.hpp"

Command::Command(void) {
	return;
}

Command::~Command() {
	return;
}

void Command::parse(string smessage) {
	if (smessage.empty() == true) {
		throw(EmptyCommand());
	}
	string cmd = smessage;
	if (cmd[0] == '/') {
		cmd = cmd.substr(1);
	}
	size_t pos;
	pos = cmd.find(' ');
	if (pos == string::npos)
		pos = cmd.length();
	_command = cmd.substr(0, pos);
	if (pos < cmd.length()) {
		// ? Add args
		cmd.erase(0, pos + 1);
		while (cmd.length() > 0) {
			pos = cmd.find(' ');
			if (pos == string::npos)
				pos = cmd.length();
			_args_v.push_back(cmd.substr(0, pos));
			cmd.erase(0, pos + 1);
		}
	} else {
		// ? No arg
	}
	// ? Fill commands
	_commands_v.reserve(20);
	_commands_v.push_back("JOIN");
	_commands_v.push_back("PART");
	_commands_v.push_back("QUIT");
	_commands_v.push_back("NICK");
	_commands_v.push_back("PONG");
	_commands_v.push_back("PRIVMSG");
	_commands_v.push_back("NOTICE");
	_commands_v.push_back("TOPIC");
	_commands_v.push_back("OPER");
	_commands_v.push_back("KICK");
	_commands_v.push_back("MODE");
	_commands_v.push_back("SHUTDOWN");
	// _commands_v.push_back("AWAY");
	// _commands_v.push_back("NAMES");
	// _commands_v.push_back("LIST");
}

void Command::welcome(User* user) {
	_exec.welcome(user);
}

void Command::select(string smessage, User* user) {
	try {
		parse(smessage);
	} catch (EmptyCommand& e) {
		logError("Received command", smessage, "Empty command");
		return;
	}
	try {
		int index;
		vector<string>::iterator it = find(_commands_v.begin(), _commands_v.end(), _command);
		if (it != _commands_v.end()) {
			index = it - _commands_v.begin();
		} else {
			Send	sender = Send();
			sender.reply(user, user, ERR_UNKNOWNCOMMAND, HEADER_SERVER, ERR_UNKNOWNCOMMAND_FORMAT, smessage.c_str());
			logError("Received command", smessage, "Unknown command");
			throw UnknownCommand();
		}
		switch (index) {
			case CMD_JOIN: {
				_exec.join(user, _args_v);
				break;
			}
			case CMD_PART: {
				_exec.part(user, _args_v);
				break;
			}
			case CMD_QUIT: {
				_exec.quit(user, _args_v);
				break;
			}
			case CMD_NICK: {
				_exec.nick(user, _args_v);
				break;
			}
			case CMD_PONG: {
				break;
			}
			case CMD_PRIVMSG: {
				_exec.privmsg(user, _args_v);
				break;
			}
			case CMD_NOTICE: {
				_exec.notice(user, _args_v);
				break;
			}
			case CMD_TOPIC: {
				_exec.topic(user, _args_v);
				break;
			}
			case CMD_OPER: {
				_exec.oper(user, _args_v);
				break;
			}
			case CMD_KICK: {
				_exec.kick(user, _args_v);
				break;
			}
			case CMD_MODE: {
				break;
			}
			case CMD_SHUTDOWN: {
				_exec.shutdown(user);
				break;
			}
			default: {
				throw(InvalidCommand());
			}
		}
	} catch (exception& e) {
		logError("Received command", smessage, e.what());
	}
}
