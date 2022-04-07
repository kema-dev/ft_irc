#include "../Command/Command.hpp"

CommandExec::CommandExec(void) {
	send_op = new Send();
	return;
}

CommandExec::~CommandExec() {
	delete send_op;
	return;
}

void CommandExec::welcome(User* user) {
	send_op->reply(user, user, RPL_WELCOME, HEADER_SERVER, ":Welcome to the Internet Relay Network %s\r\n", user->getHostName().c_str());
	send_op->reply(user, user, RPL_YOURHOST, HEADER_SERVER, ":Your host is %s, running version %s\r\n", user->getHostName().c_str(), "0.0.ratio");
	send_op->reply(user, user, RPL_CREATED, HEADER_SERVER, ":This server was created %s\r\n", getTime().c_str());
	send_op->reply(user, user, RPL_MYINFO, HEADER_SERVER, ":%s version[%s]. Available user MODE: %s Avalaible channel MODE : %s\r\n", user->getHostName().c_str(), "0.0.ratio", "none", "none");
}

void CommandExec::join(User* user, vector<string> args) {
	if (args.size() == 0) {
		// TODO send ERR_NEEDMOREPARAMS
		logError("Join command", "Empty argument", "No channel specified");
		return;
	}
	// FIXME : Check args size : need to match chan : password
	while (args.empty() == false) {
		string chan = *args.begin();
		string uid = itos(user->getUid());
		// FIXME : Check channel password (split args in two lists (i + args.size() / 2))
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
		// reply(user->getServer(), user->getUid(), JOIN_REPLY, chan);
		send_op->reply(user, user, RPL_CUSTOM, HEADER_CUSTOM, "%s %s", "JOIN", chan.c_str());
		// if (user->getServer()->chanDB->search(chan)->getTopic().empty() == true) {
		// 	reply(user->getServer(), user->getUid(), RPL_NOTOPIC_REPLY, chan);
		// } else {
		// 	reply(user->getServer(), user->getUid(), RPL_TOPIC_REPLY, chan);
		// 	send_op->reply(user, RPL_CUSTOM, HEADER_CUSTOM, "%s %s", " ", chan.c_str());
		// }
		send_op->reply(user, user, RPL_CUSTOM, HEADER_CUSTOM, "%s :%s", chan.c_str(), user->getServer()->chanDB->search(chan)->getTopic().c_str());
		// reply(user->getServer(), user->getUid(), RPL_NAMEREPLY_REPLY, chan);
		string users = "";
		vector<string> users_v = user->getServer()->chanDB->search(chan)->getNickLst();
		for (vector<string>::iterator it = users_v.begin(); it != users_v.end(); ++it) {
			users += *it + " ";
		}
		send_op->reply(user, user, RPL_CUSTOM, HEADER_CLIENT, "%s", users.c_str());
		send_op->reply(user, user, RPL_ENDOFNAMES, HEADER_CUSTOM, RPL_ENDOFNAMES_FORMAT, chan.c_str());
		// reply(user->getServer(), user->getUid(), RPL_ENDOFNAMES_REPLY, chan);
		args.erase(args.begin());
		// log("Join channel", itos(user->getUid()), "Join channel " + args[0]);
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
		send_op->reply(user, user, ERR_NORECIPIENT, HEADER_CLIENT, ERR_NORECIPIENT_FORMAT, "PRIVMSG");
		logError("Join command", "Not enough arguments", itos(args.size()));
		return;
	} else if (args.size() < 2) {
		// TODO send ERR_NOTEXTTOSEND
		send_op->reply(user, user, ERR_NOTEXTTOSEND, HEADER_CLIENT, ERR_NOTEXTTOSEND_FORMAT);
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
					send_op->reply(user, user, ERR_NOSUCHNICK, HEADER_CLIENT, ERR_NOSUCHNICK_FORMAT, receiver.c_str());
					logError("Privmsg to " + receiver, uid, e.what());
				}
				// TODO send PRIVMSG <receiver> and mind if <reciever> is away
				send_op->reply(user, user->getServer()->userDB->search(receiver), RPL_CUSTOM, HEADER_CLIENT, "PRIVMSG %s :%s\r\n", user->getNickName().c_str(), msg.c_str());
				// reply_2(user->getServer(), user->getUid(), STR_PRVMSG_U_REPLY, user->getNickName(), msg);
				args.erase(args.begin());
				continue;
			}
			// ? Is a channel
			// reply_2(user->getServer(), user->getUid(), STR_PRVMSG_C_REPLY, receiver, msg);
			vector<string> users_v = user->getServer()->chanDB->search(receiver)->getNickLst();
			for (vector<string>::iterator it = users_v.begin(); it != users_v.end(); ++it) {
				try {
					send_op->reply(user, user->getServer()->userDB->search(*it), RPL_CUSTOM, HEADER_CLIENT, "PRIVMSG %s %s\r\n", receiver.c_str(), msg.c_str());
				}
				catch (NoSuchUser& e) {
					send_op->reply(user, user, ERR_NOSUCHNICK, HEADER_CLIENT, ERR_NOSUCHNICK_FORMAT, receiver.c_str());
					logError("Privmsg to " + receiver, uid, e.what());
				}
			}
			args.erase(args.begin());
		}
	}
	// FIXME msg skips the first args
}

void CommandExec::topic(User* user, vector<string> args) {
	(void)user;
	(void)args;
	// string topic;

	// if (args.size() < 1) {
	// 	// TODO send ERR_NEEDMOREPARAMS
	// }
	// else if(args.size() == 1) {
	// 	// ? Only return the topic of the cannel
	// 	try {
	// 		user->getServer()->chanDB->search(*args.begin());
	// 	}
	// 	catch (NoSuchChan& e) {
	// 		logError("Changing topic of " + *args.begin(), "" ,e.what());
	// 	}
	// 	if (user->getServer()->chanDB->search(*args.begin())->getTopic().empty())
	// 		reply(user->getServer(), user->getUid(), RPL_NOTOPIC_REPLY, *args.begin());
	// 	else
	// 		reply(user->getServer(), user->getUid(), RPL_TOPIC_REPLY, *args.begin());
	// }
	// else if(args.size() == 2) {
	// 	// ? Set the topic
	// 	if (user->getServer()->userDB->isOper(user->getNickName()) == true)
	// 	{
			
	// 		if (args.back().find(':') != string::npos)
	// 		{
	// 			args.back().erase(0, 1);
	// 			while (args.empty() == false) {
	// 				topic = *args.begin() + topic;
	// 				args.erase(args.begin());
	// 			}
	// 		}
	// 		user->getServer()->chanDB->search(*args.begin())->setTopic(topic);
	// 	}
	// 	else
	// 	{
	// 		// TODO send ERR_CHANOPRIVSNEEDED
	// 	}
	// 	try {
	// 		user->getServer()->chanDB->search(*args.begin());
	// 	}
	// 	catch (NoSuchChan& e) {
	// 		logError("Changing topic of " + *args.begin(), "" ,e.what());
	// 	}
	// 	reply(user->getServer(), user->getUid(), RPL_TOPIC_REPLY, *args.begin());
	// }
}

// ? "3 This server was created <date>"
#define SEND_RPL_CREATED "3 This server was created %s\r\n"

void CommandExec::oper(User* user, vector<string> args) {
	(void)user;
	(void)args;
	// if (args.size() != 2) {
	// 	// TODO send ERR_NEEDMOREPARAMS
	// 	reply(user->getServer(), user->getUid(), ERR_NEEDMOREPARAMS_REPLY, "OPER");
	// 	logError("Oper command", "Wrong number of arguments (need 2)", itos(args.size()));
	// 	return;
	// }
	// string nick = *args.begin();
	// string pass = *(++args.begin());
	// string uid = itos(user->getUid());
	// try {
	// 	user->getServer()->userDB->search(nick)->becomeOper(*(user->getServer()), pass);
	// } catch (BadPasswd& e) {
	// 	// TODO send ERR_PASSWDMISMATCH
	// 	logError("Oper command " + nick, uid, e.what());
	// 	return;
	// } catch (exception& e) {
	// 	// * Maybe we should not pass here
	// 	logError("Oper command " + nick, "Unknown error", e.what());
	// 	return;
	// }
	// // TODO RPL_YOUREOPER to <user>
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
