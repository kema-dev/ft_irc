#include "../Command/Command.hpp"

CommandExec::CommandExec(void) {
	return;
}

CommandExec::~CommandExec() {
	return;
}

void CommandExec::join(User* user, vector<string> args) {
	if (args.size() == 0) {
		// TODO send ERR_NEEDMOREPARAMS
		logError("Join command", "Empty argument", "No channel specified");
		return;
	}
	while (args.empty() == false) {
		string chan = *args.begin();
		string uid = itos(user->getUid());
		try {
			user->getServer()->userDB->search(user->getUid())->tryJoinChannel(chan, "", "", user->getServer());
		} catch (NoSuchUser& e) {
			logError("Join command", "No such user", e.what());
			return;
		} catch (NotLoggedGlobal& e) {
			logError("Join channel " + chan, uid, e.what());
			return;
		} catch (AlreadyLogged& e) {
			logError("Join channel " + chan, uid, e.what());
			return;
		} catch (BadPasswd& e) {
			logError("Join channel " + chan, uid, e.what());
			return;
		} catch (ChanAddFail& e) {
			logError("Join channel " + chan, uid, e.what());
			return;
		} catch (exception& e) {
			logError("Join command", "Unknown error", e.what());
			return;
		}
		reply(user->getServer(), user->getUid(), JOIN, chan);
		if (user->getServer()->chanDB->search(chan)->getTopic().empty() == true) {
			reply(user->getServer(), user->getUid(), RPL_NOTOPIC, chan);
		} else {
			reply(user->getServer(), user->getUid(), RPL_TOPIC, chan);
		}
		reply(user->getServer(), user->getUid(), RPL_NAMEREPLY, chan);
		reply(user->getServer(), user->getUid(), RPL_ENDOFNAMES, chan);
		args.erase(args.begin());
	}
}

void CommandExec::welcome(User* user) {
	reply(user->getServer(), user->getUid(), RPL_WELCOME, "");
	reply(user->getServer(), user->getUid(), RPL_YOURHOST, "");
	reply(user->getServer(), user->getUid(), RPL_CREATED, "");
	reply(user->getServer(), user->getUid(), RPL_MYINFO, "");
}

// void CommandExec::parse_Part(string message, string *channel_s, string *msg) {
// 	string cmd;
// 	size_t pos;

// 	cmd = message;
// 	pos = cmd.find(' ');
// 	cmd.erase(0, pos + 1);
// 	pos = cmd.find(' ');
// 	if (pos == string::npos)
// 		pos = cmd.length();
// 	*channel_s = cmd.substr(0, pos);
// 	cout << cmd << "'" << endl;
// 	cmd.erase(0, pos + 1);
// 	cout << cmd << "'" << endl;
// 	if (cmd.empty())
// 		return;
// 	if (cmd.find(':') != string::npos)
// 		*msg = cmd.substr(1, cmd.length());
// 	else
// 		*msg = cmd.substr(0, cmd.length());
// }

// void CommandExec::parse_Topic(string message, string *channel_s, string *topic) {
// 	string cmd;
// 	size_t pos;

// 	cmd = message;
// 	pos = cmd.find(' ');
// 	cmd.erase(0, pos + 1);
// 	pos = cmd.find(' ');
// 	if (pos == string::npos)
// 		pos = cmd.length();
// 	*channel_s = cmd.substr(0, pos);
// 	cout << cmd << "'" << endl;
// 	cmd.erase(0, pos + 1);
// 	cout << cmd << "'" << endl;
// 	if (cmd.empty())
// 		return;
// 	if (cmd.find(':') != string::npos)
// 		*topic = cmd.substr(1, cmd.length());
// 	else
// 		*topic = cmd.substr(0, cmd.length());
// }

// void CommandExec::parse_Privmsg(string message, string *channel_s, string *msg) {
// 	string cmd;
// 	size_t pos;

// 	cmd = message;
// 	pos = cmd.find(' ');
// 	cmd.erase(0, pos + 1);
// 	pos = cmd.find(' ');
// 	*channel_s = cmd.substr(0, pos);
// 	cmd.erase(0, pos + 1);
// 	*msg = cmd.substr(0, cmd.length());
// }

// void CommandExec::parse_Oper(string message, string* password) {
// 	string cmd;
// 	size_t pos;

// 	cmd = message;
// 	pos = cmd.find(' ');
// 	cmd.erase(0, pos + 1);
// 	*password = string(cmd);
// }

// void CommandExec::parse_Kick(string message, string* chan, string* user, string* kickMsg, Server* serv) {
// 	istringstream iss(message);
// 	string s;
// 	getline(iss, s, ' '); // ? Skip "KICK"
// 	getline(iss, s, ' ');
// 	*chan = s;
// 	if (chan->empty()) {
// 		throw NeedMoreParams();
// 	}
// 	getline(iss, s, ' ');
// 	*user = s;
// 	if (user->empty()) {
// 		throw NeedMoreParams();
// 	}
// 	getline(iss, s, '\n');
// 	if (s[0] == ':')
// 		s = &s[1];
// 	try {
// 		serv->userDB->search(s);
// 	}
// 	catch (exception& e) {
// 		if (!s.empty()) {
// 			*kickMsg = s;
// 		}
// 	}
// }

// void CommandExec::parse_Mode(string message, string* user, int* req_op, int* req_away) {
// 	istringstream iss(message);
// 	string s;
// 	int i;
// 	getline(iss, s, ' '); // ? Skip "MODE"
// 	getline(iss, s, ' ');
// 	*user = s;
// 	if (user->empty()) {
// 		throw NeedMoreParams();
// 	}
// 	*req_op = 0;
// 	*req_away = 0;
// 	getline(iss, s, '\n');
// 	if ((s[0] != '+') && (s[0] != '-')) {
// 		throw NeedMoreParams();
// 	}
// 	if (s[0] == '+') {
// 		i = ACTION_ADD;
// 		s.erase(0, 1);
// 	}
// 	else if (s[0] == '-') {
// 		i = ACTION_REMOVE;
// 		s.erase(0, 1);
// 	}
// 	if (!s.empty() && s.find('o') != string::npos) {
// 		*req_op = 1;
// 		s.erase(s.find('o'), 1);
// 	}
// 	if (!s.empty() && s.find('a') != string::npos) {
// 		*req_away = 1;
// 		s.erase(s.find('a'), 1);
// 	}
// 	if (!s.empty()) {
// 		throw UnknownParam();
// 	}
// 	if (*req_op == 0 && *req_away == 0) {
// 		throw NeedMoreParams();
// 	}
// 	*req_op *= i;
// 	*req_away *= i;
// }
