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
	send_op->reply(user, user, RPL_WELCOME, HEADER_SERVER, RPL_WELCOME_FORMAT, user->getHostName().c_str());
	send_op->reply(user, user, RPL_YOURHOST, HEADER_SERVER, RPL_YOURHOST_FORMAT, user->getHostName().c_str(), "42.42");
	send_op->reply(user, user, RPL_CREATED, HEADER_SERVER, RPL_CREATED_FORMAT, getTime().c_str());
	send_op->reply(user, user, RPL_MYINFO, HEADER_SERVER, RPL_MYINFO_FORMAT, user->getHostName().c_str(), "42.42", "none", "none");
}

void CommandExec::join(User* user, vector<string> args) {
	if (args.size() < 1) {
		send_op->reply(user, user, ERR_NEEDMOREPARAMS, HEADER_CLIENT, ERR_NEEDMOREPARAMS_FORMAT, "JOIN");
		logError("Join command", "Empty argument", "No channel specified");
		return;
	}
	if (args.size() > 2) {
		send_op->reply(user, user, ERR_NEEDMOREPARAMS, HEADER_CLIENT, ERR_NEEDMOREPARAMS_FORMAT, "JOIN");
		logError("Join command", "Too much argument", itos(args.size()));
		return;
	}
	string chan = *args.begin();
	if (chan.length() < 2 || chan[0] != '#') {
		send_op->reply(user, user, ERR_NOSUCHCHANNEL, HEADER_CLIENT, ERR_NOSUCHCHANNEL_FORMAT, chan.c_str());
		logError("Join command", "Wrong channel name", chan);
		return;
	}
	string uid = itos(user->getUid());
	args.erase(args.begin());
	string pass = "";
	if (args.empty() == false) {
		pass = *args.begin();
		args.erase(args.begin());
	}
	try {
		user->tryJoinChannel(chan, pass, "", user->getServer());
	} catch (NotLoggedGlobal& e) {
		// * Maybe we should not pass here
		logError("Join channel " + chan, uid, e.what());
		return;
	} catch (AlreadyLogged& e) {
		logError("Join channel " + chan, uid, e.what());
		return;
	} catch (BadPasswd& e) {
		send_op->reply(user, user, ERR_PASSWDMISMATCH, HEADER_CLIENT, ERR_PASSWDMISMATCH_FORMAT);
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
	vector<string> users_v = user->getServer()->chanDB->search(chan)->getNickLst();
	for (vector<string>::iterator it = users_v.begin(); it != users_v.end(); ++it) {
		send_op->reply(user, user->getServer()->userDB->search(*it), RPL_CUSTOM, HEADER_CLIENT, "%s %s\r\n", "JOIN", chan.c_str());
	}
	if (user->getServer()->chanDB->search(chan)->getTopic().empty() == true) {
		send_op->reply(user, user, RPL_TOPIC, HEADER_SERVER, RPL_NOTOPIC_FORMAT, chan.c_str());
	}
	else {
		send_op->reply(user, user, RPL_TOPIC, HEADER_SERVER, RPL_TOPIC_FORMAT, chan.c_str(), user->getServer()->chanDB->search(chan)->getTopic().c_str());
	}
	string users = "";
	for (vector<string>::iterator it = users_v.begin(); it != users_v.end(); ++it) {
		if (user->getServer()->userDB->isOper(user->getServer()->userDB->search(*it)->getNickName()) == true) {
			users += "@";
		}
		users += *it + " ";
	}
	send_op->reply(user, user, RPL_NAMREPLY, HEADER_SERVER, RPL_NAMREPLY_FORMAT, chan.c_str(), users.c_str());
	send_op->reply(user, user, RPL_ENDOFNAMES, HEADER_SERVER, RPL_ENDOFNAMES_FORMAT, chan.c_str());
}

void CommandExec::part(User* user, vector<string> args) {
	if (args.size() < 1) {
		send_op->reply(user, user, ERR_NEEDMOREPARAMS, HEADER_CLIENT, ERR_NEEDMOREPARAMS_FORMAT, "PART");
		logError("Part command", "Empty argument", "No channel specified");
		return;
	}
	if (args.size() > 2) {
		send_op->reply(user, user, ERR_NEEDMOREPARAMS, HEADER_CLIENT, ERR_NEEDMOREPARAMS_FORMAT, "PART");
		logError("Part command", "Too much argument", itos(args.size()));
		return;
	}
	string chan = *args.begin();
	if (chan.length() < 2 || chan[0] != '#') {
		send_op->reply(user, user, ERR_NOSUCHCHANNEL, HEADER_CLIENT, ERR_NOSUCHCHANNEL_FORMAT, chan.c_str());
		logError("Part command", "Wrong channel name", chan);
		return;
	}
	string uid = itos(user->getUid());
	try {	
		user->tryPartChannel(user->getNickName(), chan, user->getServer());
	} catch (NotLoggedGlobal& e) {
		// * Maybe we should not pass here
		logError("Part channel " + chan, uid, e.what());
		return;
	} catch (NoSuchChan& e) {
		send_op->reply(user, user, ERR_NOSUCHCHANNEL, HEADER_CLIENT, ERR_NOSUCHCHANNEL_FORMAT);
		logError("Part channel " + chan, uid, e.what());
		return;
	} catch (NotInChan& e) {
		send_op->reply(user, user, ERR_NOTONCHANNEL, HEADER_CLIENT, ERR_NOTONCHANNEL_FORMAT);
		logError("Part channel " + chan, uid, e.what());
		return;
	} catch (exception& e) {
		// * Maybe we should not pass here
		logError("Part command", "Unknown error", e.what());
		return;
	}
	send_op->reply(user, user, RPL_CUSTOM, HEADER_CLIENT, "%s %s\r\n", "PART", chan.c_str());
	vector<string> users_v = user->getServer()->chanDB->search(chan)->getNickLst();
	for (vector<string>::iterator it = users_v.begin(); it != users_v.end(); ++it) {
		if (user->getServer()->userDB->search(*it)->getSocket() != user->getSocket())
			send_op->reply(user, user->getServer()->userDB->search(*it), RPL_CUSTOM, HEADER_CLIENT, "%s %s\r\n", "PART", chan.c_str());
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
	vector<Channel*> chans = user->getServer()->chanDB->getDB();
	for (vector<Channel*>::iterator it = chans.begin(); it != chans.end(); ++it) {
		try{
			if (user->getServer()->chanDB->search(*it)->userLeave(user) == true)
				continue;
		}
		catch(exception &e){
		}
	}
	send_op->reply(user, user, RPL_CUSTOM, HEADER_CLIENT, "%s :%s\r\n", "QUIT", msg.c_str());
	user->setActiveStatus(NOT_CONNECTED);
	user->setConnectStatus(NOT_CONNECTED);
	close(user->getSocket());
	user->getServer()->userDB->removeUser(user);
}

void CommandExec::nick(User* user, vector<string> args) {
	if (args.size() == 0) {
		send_op->reply(user, user, ERR_NONICKNAMEGIVEN, HEADER_SERVER, ERR_NONICKNAMEGIVEN_FORMAT);
		logError("Nick command", "Empty argument", "No nickname specified");
		return;
	} else if (args.size() > 1) {
		send_op->reply(user, user, ERR_NONICKNAMEGIVEN, HEADER_SERVER, ERR_NONICKNAMEGIVEN_FORMAT);
		logError("Nick command", "Too many arguments", itos(args.size()));
	} else {
		string nick = *args.begin();
		string uid = itos(user->getUid());
		try {
			user->getServer()->userDB->chkNickDuplicate(nick);
		} catch (UserDuplicate& e) {
			send_op->reply(user, user, ERR_NICKNAMEINUSE, HEADER_SERVER, ERR_NICKNAMEINUSE_FORMAT, nick.c_str());
			logError("Nick command " + nick, uid, e.what());
			return;
		} catch (exception& e) {
			// * Maybe we should not pass here
			logError("Nick command " + nick, "Unknown error", e.what());
			return;
		}
		user->setNickName(nick);
		send_op->reply(user, user, RPL_CUSTOM, HEADER_CLIENT, "%s %s\r\n", "NICK", nick.c_str());
	}
}

void CommandExec::privmsg(User* user, vector<string> args) {
	if (args.size() < 1) {
		send_op->reply(user, user, ERR_NORECIPIENT, HEADER_CLIENT, ERR_NORECIPIENT_FORMAT, "PRIVMSG");
		logError("Privmsg command", "Not enough arguments", itos(args.size()));
		return;
	} else if (args.size() < 2) {
		send_op->reply(user, user, ERR_NOTEXTTOSEND, HEADER_CLIENT, ERR_NOTEXTTOSEND_FORMAT);
		logError("Privmsg command", "Not enough arguments", itos(args.size()));
		return;
	} else {
		string uid = itos(user->getUid());
		string receiver = *args.begin();
		args.erase(args.begin());
		if (args.begin()->find(":") == 0) {
			args.begin()->erase(0, 1);
		}
		string msg = "";
		while (args.empty() == false) {
			msg += *args.begin() + " ";
			args.erase(args.begin());
		}
		msg.erase(msg.end() - 1);
		try {
			user->getServer()->chanDB->search(receiver);
		} catch (NoSuchChan& e) {
			// ? Is not a channel
			try {
				user->getServer()->userDB->search(receiver);
			} catch (NoSuchUser& e) {
				send_op->reply(user, user, ERR_NOSUCHNICK, HEADER_CLIENT, ERR_NOSUCHNICK_FORMAT, receiver.c_str());
				logError("Privmsg to " + receiver, uid, e.what());
			}
			send_op->reply(user, user->getServer()->userDB->search(receiver), RPL_CUSTOM, HEADER_CLIENT, "PRIVMSG %s :%s\r\n", user->getServer()->userDB->search(receiver)->getNickName().c_str(), msg.c_str());
			log(string(LIGHT_MAGENTA) + "User " + string(GREEN) + user->getNickName() + string(LIGHT_BLUE) + " sent PRIVMSG to " + string(GREEN) + receiver + string(LIGHT_MAGENTA) + ": " + string(LIGHT_BLUE) + msg + DEFAULT);
			return;
		}
		// ? Is a channel
		if (user->getServer()->chanDB->search(receiver)->isLog(user) == UNKNOWN) {
			send_op->reply(user, user, ERR_NOTONCHANNEL, HEADER_CLIENT, ERR_NOTONCHANNEL_FORMAT, receiver.c_str());
			logError("Privmsg to " + receiver, uid, "User is not in channel");
			return;
		}
		vector<string> users_v = user->getServer()->chanDB->search(receiver)->getNickLst();
		for (vector<string>::iterator it = users_v.begin(); it != users_v.end(); ++it) {
			if (*it == user->getNickName()) {
				continue;
			}
			try {
				send_op->reply(user, user->getServer()->userDB->search(*it), RPL_CUSTOM, HEADER_CLIENT, "PRIVMSG %s :%s\r\n", receiver.c_str(), msg.c_str());
			}
			catch (NoSuchUser& e) {
				send_op->reply(user, user, ERR_NOSUCHNICK, HEADER_CLIENT, ERR_NOSUCHNICK_FORMAT, receiver.c_str());
				logError("Privmsg to " + receiver, uid, e.what());
			}
		}
		log(string(LIGHT_MAGENTA) + "User " + string(GREEN) + user->getNickName() + string(LIGHT_BLUE) + " sent PRIVMSG to " + string(GREEN) + receiver + string(LIGHT_MAGENTA) + ": " + string(LIGHT_BLUE) + msg + DEFAULT);
	}
}

void CommandExec::topic(User* user, vector<string> args) {
	string topic;
	string chan;
	if (args.size() < 1) {
		send_op->reply(user, user, ERR_NEEDMOREPARAMS, HEADER_CLIENT, ERR_NEEDMOREPARAMS_FORMAT, "PART");
		logError("Topic command", "Empty argument", "No channel specified");
	}
	else if(args.size() == 1) {
		// ? Only return the topic of the cannel
		try {
			user->getServer()->chanDB->search(*args.begin());
		}
		catch (NoSuchChan e) {
			logError("Changing topic of " + *args.begin(), "" ,e.what());
		}
		if (user->getServer()->chanDB->search(*args.begin())->getTopic().empty() == false)
			send_op->reply(user, user, RPL_TOPIC, HEADER_SERVER, RPL_TOPIC_FORMAT, (*args.begin()).c_str(), user->getServer()->chanDB->search(*args.begin())->getTopic().c_str());
		else
			send_op->reply(user, user, RPL_TOPIC, HEADER_SERVER, RPL_NOTOPIC_FORMAT, (*args.begin()).c_str());
	}
	else if(args.size() >= 2) {
		// ? Set the topic
		try {
			user->getServer()->chanDB->search(*args.begin());
		}
		catch (NoSuchChan& e) {
			logError("Changing topic of " + *args.begin(), "" ,e.what());
			return;
		}
		try {
			if (user->getServer()->chanDB->search(*args.begin())->isLog(user) != CONNECTED)
				throw NotInChan();
		}
		catch (NotInChan& e) {
			send_op->reply(user, user, ERR_NOTONCHANNEL, HEADER_CLIENT, ERR_NOTONCHANNEL_FORMAT, "PART");
			logError("Topic command", "Is Logged to channel", e.what());
			return;
		}
		if (user->getServer()->userDB->isOper(user->getNickName()) == true) {
			chan = *args.begin();
			args.erase(args.begin());
			size_t i = 0;	
			if (args[0].find(':') != string::npos) {
				args.begin()->erase(0, 1);
				while (i <= (args.size() - 1)) {
					topic += args[i] + " ";
					i++;
				}
			}
			else
				topic = args[0];
			user->getServer()->chanDB->search(chan)->setTopic(topic);
		}
		else {
			send_op->reply(user, user, ERR_CHANOPRIVSNEEDED, HEADER_CLIENT, ERR_CHANOPRIVSNEEDED_FORMAT, chan.c_str());
			logError("Topic command", "Op required", "User don't have op rights");
			return;
		}
		vector<string> users_v = user->getServer()->chanDB->search(chan)->getNickLst();
		for (vector<string>::iterator it = users_v.begin(); it != users_v.end(); ++it) {
			send_op->reply(user, user->getServer()->userDB->search(*it), RPL_CUSTOM, HEADER_CLIENT, "TOPIC %s :%s\r\n", chan.c_str(), topic.c_str());
		}

	}
}

void CommandExec::oper(User* user, vector<string> args) {
	if (args.size() != 2) {
		send_op->reply(user, user, ERR_NEEDMOREPARAMS, HEADER_SERVER, ERR_NEEDMOREPARAMS_FORMAT);
		logError("Oper command", "Wrong number of arguments (need 2)", itos(args.size()));
		return;
	}
	string nick = *args.begin();
	string pass = *(++args.begin());
	string uid = itos(user->getUid());
	try {
		user->getServer()->userDB->search(nick)->becomeOper(*(user->getServer()), pass);
	} catch (BadPasswd& e) {
		send_op->reply(user, user, ERR_PASSWDMISMATCH, HEADER_SERVER, ERR_PASSWDMISMATCH_FORMAT);
		logError("Oper command " + nick, uid, e.what());
		return;
	} catch (exception& e) {
		// * Maybe we should not pass here
		logError("Oper command " + nick, "Unknown error", e.what());
		return;
	}
	send_op->reply(user, user, RPL_YOUREOPER, HEADER_SERVER, RPL_YOUREOPER_FORMAT);
}

void CommandExec::kick(User* user, vector<string> args) {
	if (args.size() < 2) {
		send_op->reply(user, user, ERR_NEEDMOREPARAMS, HEADER_SERVER, ERR_NEEDMOREPARAMS_FORMAT, "KICK");
		logError("Oper command", "Not enough arguments", itos(args.size()));
		return;
	}
	string chan = *args.begin();
	args.erase(args.begin());
	string target = *args.begin();
	args.erase(args.begin());
	string msg = "";
	while (args.empty() == false) {
		msg += *args.begin() + " ";
		args.erase(args.begin());
	}
	string uid = itos(user->getUid());
	try {
		user->getServer()->userDB->search(user->getNickName())->kick((user->getServer()->userDB->search(target)), (user->getServer()->chanDB->search(chan)), msg);
		// user->getServer()->chanDB->search(chan)->userLeave(*user->getServer()->userDB->search(target));
	} catch (NoSuchChan& e) {
		send_op->reply(user, user, ERR_NOSUCHCHANNEL, HEADER_SERVER, ERR_NOSUCHCHANNEL_FORMAT, chan.c_str());
		logError("Kick command " + chan, uid, e.what());
		return;
	} catch (NoSuchUser& e) {
		send_op->reply(user, user, ERR_NOSUCHNICK, HEADER_SERVER, ERR_NOSUCHNICK_FORMAT, user->getNickName().c_str());
		logError("Kick command " + target, uid, e.what());
		return;
	} catch (BadRole& e) {
		send_op->reply(user, user, ERR_CHANOPRIVSNEEDED, HEADER_SERVER, ERR_CHANOPRIVSNEEDED_FORMAT, chan.c_str());
		logError("Kick command " + target, uid, e.what());
		return;
	} catch (NotLogged& e) {
		send_op->reply(user, user, ERR_NOTONCHANNEL, HEADER_SERVER, ERR_NOTONCHANNEL_FORMAT, chan.c_str());
		logError("Kick command " + target, uid, e.what());
		return;
	} catch (exception& e) {
		// * Maybe we should not pass here
		logError("Kick command " + target, "Unknown error", e.what());
		return;
	}
	send_op->reply(user, user, RPL_CUSTOM, HEADER_CLIENT, "%s %s %s %s\r\n", "KICK", chan.c_str(), target.c_str(), msg.c_str());
	vector<string> users_v = user->getServer()->chanDB->search(chan)->getNickLst();
	for (vector<string>::iterator it = users_v.begin(); it != users_v.end(); ++it) {
		send_op->reply(user, user->getServer()->userDB->search(*it), RPL_CUSTOM, HEADER_CLIENT, "%s %s %s %s\r\n", "KICK", chan.c_str(), target.c_str(), msg.c_str());
	}
}

void CommandExec::notice(User *user, vector<string> args){
	if (args.size() >= 2) {
		string uid = itos(user->getUid());
		string receiver = *args.begin();
		args.erase(args.begin());
		if (args.begin()->find(":") == 0) {
			args.begin()->erase(0, 1);
		}
		string msg = "";
		while (args.empty() == false) {
			msg += *args.begin() + " ";
			args.erase(args.begin());
		}
		msg.erase(msg.end() - 1);
		try {
			user->getServer()->chanDB->search(receiver);
		} catch (NoSuchChan& e) {
			// ? Is not a channel
			try {
				user->getServer()->userDB->search(receiver);
			} catch (NoSuchUser& e) {}
			send_op->reply(user, user->getServer()->userDB->search(receiver), RPL_CUSTOM, HEADER_CLIENT, "NOTICE %s :%s\r\n", user->getServer()->userDB->search(receiver)->getNickName().c_str(), msg.c_str());
			log(string(LIGHT_MAGENTA) + "User " + string(GREEN) + user->getNickName() + string(LIGHT_BLUE) + " sent NOTICE to " + string(GREEN) + receiver + string(LIGHT_MAGENTA) + ": " + string(LIGHT_BLUE) + msg + DEFAULT);
			return;
		}
		// ? Is a channel
		if (user->getServer()->chanDB->search(receiver)->isLog(user) == UNKNOWN) {
			return;
		}
		vector<string> users_v = user->getServer()->chanDB->search(receiver)->getNickLst();
		for (vector<string>::iterator it = users_v.begin(); it != users_v.end(); ++it) {
			if (*it == user->getNickName()) {
				continue;
			}
			try {
				send_op->reply(user, user->getServer()->userDB->search(*it), RPL_CUSTOM, HEADER_CLIENT, "NOTICE %s :%s\r\n", receiver.c_str(), msg.c_str());
			}
			catch (NoSuchUser& e) {}
		}
		log(string(LIGHT_MAGENTA) + "User " + string(GREEN) + user->getNickName() + string(LIGHT_BLUE) + " sent NOTICE to " + string(GREEN) + receiver + string(LIGHT_MAGENTA) + ": " + string(LIGHT_BLUE) + msg + DEFAULT);
	}
}

void CommandExec::shutdown(User *user){
	if (user->getServer()->userDB->isOper(user->getNickName()) == true) {
		log(string(LIGHT_MAGENTA) + "Server is shutting down" + DEFAULT);
		exit(0);
	} else {
		send_op->reply(user, user, ERR_NOPRIVILEGES, HEADER_SERVER, ERR_NOPRIVILEGES_FORMAT);
		logError("Shutdown command", user->getNickName(), "User has no privileges");
	}
}

// void CommandExec::away(User* user, vector<string> args) {
// 	if (args.empty() == true) {
// 		if (user->getActiveStatus() == CONNECTED) {
// 			// ? Already active
// 			return;
// 		} else {
// 			user->setActiveStatus(true);
// 			// TO\DO send RPL_NOWAWAY
// 		}
// 		user->setActiveStatus(CONNECTED);
// 		user->setAwayMessage("");
// 		// TO\DO RPL_UNAWAY to <user>
// 		log(string(LIGHT_MAGENTA) + string("User ") + string(GREEN) + string(user->getNickName()) + string(LIGHT_BLUE) + string(" came back on ") + string(LIGHT_MAGENTA) + string("server ") + string(GREEN) + string(DEFAULT));
// 		return;
// 	}
// 	else {
// 		string msg = "";
// 		while (args.empty() == false) {
// 			msg += *args.begin();
// 			msg += " ";
// 			args.erase(args.begin());
// 		}
// 		if ((user->getActiveStatus() == AWAY) && (user->getAwayMessage() == msg)) {
// 			// ? Already away with same msg
// 			return;
// 		}
// 		user->setActiveStatus(AWAY);
// 		user->setAwayMessage(msg);
// 		log(string(LIGHT_MAGENTA) + string("User ") + string(RED) + string(user->getNickName()) + string(LIGHT_BLUE) + string(" is now away from ") + string(LIGHT_MAGENTA) + string("server ") + string(RED) + user->getServer()->name + string(LIGHT_BLUE) + " with message " + string(RED) + msg + string(DEFAULT));
// 		// TO\DO send RPL_NOWAWAY
// 	}
// }

// void CommandExec::names(User* user, vector<string> args) {
// 	if (args.size() == 0) {
// 		send_op->reply(user, user, ERR_NEEDMOREPARAMS, HEADER_CLIENT, ERR_NEEDMOREPARAMS_FORMAT, "NAMES");
// 		logError("Names command", "Empty argument", "No channel specified");
// 		return;
// 	}
// 	else if (args.size() > 1) {
// 		send_op->reply(user, user, ERR_NEEDMOREPARAMS, HEADER_CLIENT, ERR_NEEDMOREPARAMS_FORMAT, "NAMES");
// 		logError("Names command", "Too much arguments", itos(args.size()));
// 		return;
// 	}
// 	string chan = *args.begin();
// 	string uid = itos(user->getUid());
// 	vector<string> lst;
// 	try {
// 		lst = user->getServer()->chanDB->search(chan)->getNickLst();
// 	} catch (NoSuchChan& e) {
// 		send_op->reply(user, user, ERR_NOSUCHCHANNEL, HEADER_CLIENT, ERR_NOSUCHCHANNEL_FORMAT, chan.c_str());
// 		logError("Names from channel " + chan, "No such channel", e.what());
// 		return;
// 	}
// 	string users = "";
// 	vector<string> users_v = user->getServer()->chanDB->search(chan)->getNickLst();
// 	for (vector<string>::iterator it = users_v.begin(); it != users_v.end(); ++it) {
// 		if (user->getServer()->userDB->isOper(user->getServer()->userDB->search(*it)->getNickName()) == true) {
// 			users += "@";
// 		}
// 		users += *it + " ";
// 	}
// 	send_op->reply(user, user, RPL_NAMREPLY, HEADER_SERVER, RPL_NAMREPLY_FORMAT, chan.c_str(), users.c_str());
// 	send_op->reply(user, user, RPL_ENDOFNAMES, HEADER_SERVER, RPL_ENDOFNAMES_FORMAT, chan.c_str());
// }

// void	CommandExec::list(User* user, vector<string> args)
// {
// 	(void)user;
// 	(void)args;
// }
