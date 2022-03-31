#include "../Command/Command.hpp"

Command::Command(void) {
	return ;
}

Command::~Command() {
	return ;
}

void	Command::parse(string smessage) {
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
	}
	else {
		// ? No arg
	}
	// ? Fill commands
	_commands_v.reserve(20);
	_commands_v.push_back("JOIN");
	_commands_v.push_back("PART");
	_commands_v.push_back("QUIT");
	_commands_v.push_back("NICK");
	_commands_v.push_back("USER");
	_commands_v.push_back("PONG");
	_commands_v.push_back("PRIVMSG");
	_commands_v.push_back("TOPIC");
	_commands_v.push_back("OPER");
	_commands_v.push_back("KICK");
	_commands_v.push_back("MODE");
	_commands_v.push_back("AWAY");
	_commands_v.push_back("NAMES");
	_commands_v.push_back("LIST");
	_commands_v.push_back("NOTICE");
	_commands_v.push_back("PASS");
	_commands_v.push_back("WELCOME");
}

void	Command::welcome(User* user) {
	_exec.welcome(user);
}

void	Command::select(string smessage, User* user) {
	try {
		parse(smessage);
	}
	catch (EmptyCommand& e) {
		// TODO send empty command, maybe we should not pass here
		logError("Received command", smessage, "Empty command");
		throw EmptyCommand();
	}
	try {
		int index;
		vector<string>::iterator it = find(_commands_v.begin(), _commands_v.end(), _command);
		if (it != _commands_v.end()) {
			index = it - _commands_v.begin();
		}
		else {
			// TODO send unknown command
			logError("Received command", smessage, "Unknown command");
			throw UnknownCommand();
		}
		_user = user;
		switch (index) {
			case CMD_JOIN:
				_exec.join(user, _args_v);
				break;
			// case PART: {
			// 	string msg;
			// 	parse_Part(message, &channel_s, &msg);
			// 	exec_Part(params, channel_s, msg);
			// 	break;
			// }
			// case QUIT:
			// 	send(params->client_socket, "QUIT", strlen("QUIT"), MSG_DONTWAIT);
			// 	exit(EXIT_SUCCESS);
			// case NICK:
			// 	exec_Nick(params, channel_s);
			// 	break;
			// case USER:
			// 	exec_User(params, channel_s);
			// 	break;
			// case PONG: {
			// 	// TODO handle it + send PING every minute
			// 	break;
			// }
			// case PRIVMSG: {
			// 	string msg;
			// 	parse_Privmsg(message, &channel_s, &msg);
			// 	exec_PrivateMessage(params, channel_s, msg);
			// 	break;
			// }
			// case TOPIC: {
			// 	string topic;
			// 	parse_Topic(message, &channel_s, &topic);
			// 	exec_Topic(params, channel_s, topic);
			// 	break;
			// }
			// case OPER: {
			// 	exec_Oper(message, params);
			// 	break;
			// }
			// case KICK: {
			// 	exec_Kick(message, params);
			// 	break;
			// }
			// case MODE: {
			// 	cerr << "MODE" << endl;
			// 	exec_Mode(message, params);
			// 	break;
			// }
			// default: {
			// 	throw(InvalidCommand());
			// }
		}
	}
	catch (exception& e) {
		// FIXME catch errors and send specific replies
		logError("Received command", smessage, e.what());
		throw InvalidCommand();
	}
}
