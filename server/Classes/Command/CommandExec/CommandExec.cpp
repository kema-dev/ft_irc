#include "../Command/Command.hpp"

CommandExec::CommandExec(void) {
	return;
}

CommandExec::~CommandExec() {
	return;
}

void CommandExec::welcome(User* user) {
	reply(user->getServer(), user->getUid(), RPL_WELCOME, "");
	reply(user->getServer(), user->getUid(), RPL_YOURHOST, "");
	reply(user->getServer(), user->getUid(), RPL_CREATED, "");
	reply(user->getServer(), user->getUid(), RPL_MYINFO, "");
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
			user->tryJoinChannel(chan, "", "", user->getServer());
		} catch (NotLoggedGlobal& e) {
			// * Maybe we should not pass here
			logError("Join channel " + chan, uid, e.what());
			return;
		} catch (AlreadyLogged& e) {
			logError("Join channel " + chan, uid, e.what());
			return;
		} catch (BadPasswd& e) {
			// TODO send ERR_BADCHANNELKEY
			logError("Join channel " + chan, uid, e.what());
			return;
		} catch (ChanAddFail& e) {
			// * Maybe we should not pass here
			logError("Join channel " + chan, uid, e.what());
			return;
		} catch (exception& e) {
			// * Maybe we should not pass here
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

void CommandExec::part(User* user, vector<string> args) {
	if (args.size() == 0) {
		// TODO send ERR_NEEDMOREPARAMS
		logError("Join command", "Empty argument", "No channel specified");
		return;
	}
	while (args.empty() == false) {
		string chan = *args.begin();
		string uid = itos(user->getUid());
		try {
			user->getServer()->chanDB->search(chan)->userLeave(*user);
		} catch (NoSuchChan& e) {
			// TODO send ERR_NOSUCHCHANNEL
			logError("Part from channel " + chan, "No such channel", e.what());
			continue;
		} catch (NotLoggedGlobal& e) {
			// * Maybe we should not pass here
			logError("Part from channel " + chan, uid, e.what());
			return;
		} catch (NotInChan& e) {
			// TODO send ERR_NOTONCHANNEL
			logError("Part from channel " + chan, uid, e.what());
			return;
		} catch (exception& e) {
			logError("Part from channel " + chan, "Unknown error", e.what());
			return;
		}
		args.erase(args.begin());
	}
}

void CommandExec::quit(User* user, vector<string> args) {
	if (args.size() == 0) {
		args.push_back(user->getNickName());
	}
	string msg = "";
	while (args.empty() == false) {
		msg += *args.begin();
		args.erase(args.begin());
	}
	user->getServer()->userDB->search(user->getUid())->logOut(*(user->getServer()), msg);
}

void CommandExec::nick(User* user, vector<string> args) {
	if (args.size() == 0) {
		// TODO send ERR_NONICKNAMEGIVEN
		logError("Nick command", "Empty argument", "No nickname specified");
		return;
	} else if (args.size() > 1) {
		// TODO send ERR_ERRONEUSNICKNAME
		logError("Nick command", "Too many arguments", itos(args.size()));
	} else {
		string nick = *args.begin();
		string uid = itos(user->getUid());
		try {
			user->getServer()->userDB->chkNickDuplicate(nick);
		} catch (UserDuplicate& e) {
			// TODO send ERR_NICKNAMEINUSE
			logError("Nick command " + nick, uid, e.what());
			return;
		} catch (exception& e) {
			// * Maybe we should not pass here
			logError("Nick command " + nick, "Unknown error", e.what());
			return;
		}
	}
}
