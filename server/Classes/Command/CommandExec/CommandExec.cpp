#include "../Command/Command.hpp"

CommandExec::CommandExec(void) {
	return;
}

CommandExec::~CommandExec() {
	return;
}

void CommandExec::welcome(User* user) {
	reply(user->getServer(), user->getUid(), RPL_WELCOME_REPLY, "");
	reply(user->getServer(), user->getUid(), RPL_YOURHOST_REPLY, "");
	reply(user->getServer(), user->getUid(), RPL_CREATED_REPLY, "");
	reply(user->getServer(), user->getUid(), RPL_MYINFO_REPLY, "");
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
		reply(user->getServer(), user->getUid(), JOIN_REPLY, chan);
		if (user->getServer()->chanDB->search(chan)->getTopic().empty() == true) {
			reply(user->getServer(), user->getUid(), RPL_NOTOPIC_REPLY, chan);
		} else {
			reply(user->getServer(), user->getUid(), RPL_TOPIC_REPLY, chan);
		}
		reply(user->getServer(), user->getUid(), RPL_NAMEREPLY_REPLY, chan);
		reply(user->getServer(), user->getUid(), RPL_ENDOFNAMES_REPLY, chan);
		args.erase(args.begin());
	}
	// TODO send JOIN to all channels
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
	// TODO send PART to all channels
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
	// ! TODO remove user from all channels + delete it
	// TODO send QUIT to all channels
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
	// TODO send NICK to all channels
}

void CommandExec::privmsg(User* user, vector<string> args) {
	if (args.size() < 1) {
		// TODO send ERR_NORECIPIENT
		logError("Join command", "Not enough arguments", itos(args.size()));
		return;
	} else if (args.size() < 2) {
		// TODO send ERR_NOTEXTTOSEND
		logError("Join command", "Not enough arguments", itos(args.size()));
		return;
	} else {
		string msg = args.back();
		args.pop_back();
		while (args.empty() == false) {
			string receiver = *args.begin();
			string uid = itos(user->getUid());
			if (receiver.find(':', 0) == 0) {
				// ? Remaining args are message splices
				receiver.erase(0, 1);
				msg = receiver + " " + msg;
				cerr << endl;
				args.erase(args.begin());
				while (args.empty() == false) {
					msg = *args.begin() + msg;
					args.erase(args.begin());
				}
				continue;
			}
			try {
				user->getServer()->chanDB->search(receiver);
			} catch (NoSuchChan& e) {
				// ? Is not a channel
				try {
					user->getServer()->userDB->search(receiver);
				} catch (NoSuchUser& e) {
					// TODO send ERR_NOSUCHNICK
					logError("Privmsg to " + receiver, uid, e.what());
				}
				// TODO send PRIVMSG <receiver> and mind if <reciever> is away
				reply_2(user->getServer(), user->getUid(), PRVMSG_U_REPLY, user->getNickName(), msg);
				args.erase(args.begin());
				continue;
			}
			reply_2(user->getServer(), user->getUid(), PRVMSG_C_REPLY, receiver, msg);

			args.erase(args.begin());
		}
	}
}

void CommandExec::topic(User* user, vector<string> args) {
	string topic;

	if (args.size() < 1) {
		// TODO send ERR_NEEDMOREPARAMS
	}
	else if(args.size() == 1) {
		// ? Only return the topic of the cannel
		try {
			user->getServer()->chanDB->search(*args.begin());
		}
		catch (NoSuchChan& e) {
			logError("Changing topic of " + *args.begin(), "" ,e.what());
		}
		if (user->getServer()->chanDB->search(*args.begin())->getTopic().empty())
			reply(user->getServer(), user->getUid(), RPL_NOTOPIC_REPLY, *args.begin());
		else
			reply(user->getServer(), user->getUid(), RPL_TOPIC_REPLY, *args.begin());
	}
	else if(args.size() == 2) {
		// ? Set the topic
		if (user->getServer()->userDB->isOper(user->getNickName()) == true)
		{
			
			if (args.back().find(':') != string::npos)
			{
				args.back().erase(0, 1);
				while (args.empty() == false) {
					topic = *args.begin() + topic;
					args.erase(args.begin());
				}
			}
			user->getServer()->chanDB->search(*args.begin())->setTopic(topic);
		}
		else
		{
			// TODO send ERR_CHANOPRIVSNEEDED
		}
		try {
			user->getServer()->chanDB->search(*args.begin());
		}
		catch (NoSuchChan& e) {
			logError("Changing topic of " + *args.begin(), "" ,e.what());
		}
		reply(user->getServer(), user->getUid(), RPL_TOPIC_REPLY, *args.begin());
}
void CommandExec::oper(User* user, vector<string> args) {
	if (args.size() != 2) {
		// TODO send ERR_NEEDMOREPARAMS
		logError("Oper command", "Wrong number of arguments (need 2)", itos(args.size()));
		return;
	}
	string nick = *args.begin();
	string pass = *(++args.begin());
	string uid = itos(user->getUid());
	try {
		user->getServer()->userDB->search(nick)->becomeOper(*(user->getServer()), pass);
	} catch (BadPasswd& e) {
		// TODO send ERR_PASSWDMISMATCH
		logError("Oper command " + nick, uid, e.what());
		return;
	} catch (exception& e) {
		// * Maybe we should not pass here
		logError("Oper command " + nick, "Unknown error", e.what());
		return;
	}
	// TODO RPL_YOUREOPER to <user>
}

void CommandExec::kick(User* user, vector<string> args) {
	if (args.size() < 2) {
		// TODO send ERR_NEEDMOREPARAMS
		logError("Oper command", "Not enough arguments", itos(args.size()));
		return;
	}
	string chan = *args.begin();
	args.erase(args.begin());
	string target = *args.begin();
	args.erase(args.begin());
	string msg = "";
	while (args.empty() == false) {
		msg += *args.begin();
		args.erase(args.begin());
	}
	string uid = itos(user->getUid());
	try {
		user->getServer()->userDB->search(user->getNickName())->kick(*(user->getServer()->userDB->search(target)), *(user->getServer()->chanDB->search(chan)), msg);
	} catch (NoSuchChan& e) {
		// TODO send ERR_NOSUCHCHANNEL
		logError("Kick command " + chan, uid, e.what());
		return;
	} catch (NoSuchUser& e) {
		// TODO send ERR_NOSUCHNICK
		logError("Kick command " + target, uid, e.what());
		return;
	} catch (BadRole& e) {
		// TODO send ERR_CHANOPRIVSNEEDED
		logError("Kick command " + target, uid, e.what());
		return;
	} catch (NotLogged& e) {
		// TODO send ERR_NOTONCHANNEL
		logError("Kick command " + target, uid, e.what());
		return;
	} catch (exception& e) {
		// * Maybe we should not pass here
		logError("Kick command " + target, "Unknown error", e.what());
		return;
	}
	// TODO send KICK to <user>
}

void CommandExec::away(User* user, vector<string> args) {
	if (args.empty() == true) {
		if (user->getActiveStatus() == CONNECTED) {
			// ? Already active
			return;
		} else {
			user->setActiveStatus(true);
			// TODO send RPL_NOWAWAY
		}
		user->setActiveStatus(CONNECTED);
		user->setAwayMessage("");
		// TODO RPL_UNAWAY to <user>
		log(string(LIGHT_MAGENTA) + string("User ") + string(GREEN) + string(user->getNickName()) + string(LIGHT_BLUE) + string(" came back on ") + string(LIGHT_MAGENTA) + string("server ") + string(GREEN) + string(DEFAULT));
		return;
	}
	else {
		string msg = "";
		while (args.empty() == false) {
			msg += *args.begin();
			msg += " ";
			args.erase(args.begin());
		}
		if ((user->getActiveStatus() == AWAY) && (user->getAwayMessage() == msg)) {
			// ? Already away with same msg
			return;
		}
		user->setActiveStatus(AWAY);
		user->setAwayMessage(msg);
		log(string(LIGHT_MAGENTA) + string("User ") + string(RED) + string(user->getNickName()) + string(LIGHT_BLUE) + string(" is now away from ") + string(LIGHT_MAGENTA) + string("server ") + string(RED) + user->getServer()->name + string(LIGHT_BLUE) + " with message " + string(RED) + msg + string(DEFAULT));
		// TODO send RPL_NOWAWAY
	}
}

void CommandExec::names(User* user, vector<string> args) {
	if (args.size() == 0) {
		// TODO send ERR_NEEDMOREPARAMS
		string str = "461 NAMES :Not enough parameters\r\n";
		send(user->getSocket(), str.c_str(), str.size(), MSG_DONTWAIT);
		// "<command> :Not enough parameters"
		logError("Names command", "Empty argument", "No channel specified");
		return;
	}
	while (args.empty() == false) {
		string chan = *args.begin();
		string uid = itos(user->getUid());
		vector<string> lst;
		try {
			lst = user->getServer()->chanDB->search(chan)->getNickLst();
		} catch (NoSuchChan& e) {
			// TODO send ERR_NOSUCHCHANNEL
			logError("Part from channel " + chan, "No such channel", e.what());
			args.erase(args.begin());
			continue;
		}
		for (vector<string>::iterator it = lst.begin(); it != lst.end(); ++it) {
			// TODO send RPL_NAMREPLY of <arg> channel
		}
		args.erase(args.begin());
	}
	// TODO send RPL_ENDOFNAMES
}
