#include "Channel.hpp"

// // ? Create a new channel
// Channel::Channel(string name, string pass, string topic, string oper_pass) {
// 	if (name == "") {
// 		throw (WrongChannelName());
// 	}
//     if (name[0] != '#') {
// 		throw (WrongChannelName());
//     }
// 	for (size_t i = 1; name[i]; i++) {
// 		if (!(isalnum(name[i]))) {
// 			throw (WrongChannelName());
// 		}
// 	}
// 	_topic = topic;
// 	_hash = sha256(pass);
// 	_name = name;
// 	_next_uid = 0;
// 	_hash = sha256(pass);
// 	_oper = sha256(oper_pass);
// 	log(string(LIGHT_MAGENTA) + string("Channel ") + string(GREEN) + string(_name) + string(" (hash: ") + string(GREEN) + string(_hash) + string(") ") + string(LIGHT_BLUE) + string("has been created") + string(DEFAULT));
// }

// ? Create a new channel
Channel::Channel(string name, string pass, string topic) {
	if (name == "") {
		throw (WrongChannelName());
	}
    if (name[0] != '#') {
		throw (WrongChannelName());
    }
	for (size_t i = 1; name[i]; i++) {
		if (!(isalnum(name[i]))) {
			throw (WrongChannelName());
		}
	}
	_topic = topic;
	_hash = sha256(pass);
	_name = name;
	_next_uid = 0;
	log(string(LIGHT_MAGENTA) + string("Channel ") + string(GREEN) + string(_name) + string(" (hash: ") + string(GREEN) + string(_hash) + string(") ") + string(LIGHT_BLUE) + string("has been created") + string(DEFAULT));
}

// ? Get <this> name
string	Channel::getName(void) {
	return _name;
}

// ? Get <this> topic
string Channel::getTopic(void) {
	return _topic;
}

// ? Get the UID of the first message in <this> after timestamp <time>
ssize_t	Channel::getUidAfter(timeval time) {
	vector<Message>::iterator it, end;
	it = _hist.begin();
	end = _hist.end();
	while (it != end) {
		if (it->compareTime(time) == true) {
			return (it->getUid());
		}
		it++;
	}
	return -1;
}

// ? Set <this> topic as <topic>
void	Channel::setTopic(string topic) {
	_topic = topic;
}

// ? Make <usr> log to channel <this> using password <pass>
bool	Channel::userJoin(User& usr, string pass) {
	if (usr.getActiveStatus() != true) {
		throw NotLoggedGlobal();
		return false;
	}
	if (sha256(pass) != _hash) {
		return false;
	}
	_log.push_back(pair<User&, int>(usr, CONNECTED));
	log(string(LIGHT_MAGENTA) + string("User ") + string(GREEN) + string(usr.getNickName()) + string(LIGHT_BLUE) + string(" joined ") + string(LIGHT_MAGENTA) + string("channel ") + string(GREEN) + string(_name) + string(DEFAULT));
	return true;
}

// ? Make <usr> leave channel channel <this>
bool	Channel::userLeave(User& usr) {
	if (usr.getActiveStatus() != true) {
		throw NotLoggedGlobal();
		return false;
	}
	vector<pair<User&, int> >::iterator	it, end;
	it = _log.begin();
	end = _log.end();
	while (it != end) {
		if (it->first.getNickName() == usr.getNickName()) {
			log(string(LIGHT_MAGENTA) + string("User ") + string(RED) + string(usr.getNickName()) + string(LIGHT_BLUE) + string(" left ") + string(LIGHT_MAGENTA) + string("channel ") + string(RED) + string(_name) + string(DEFAULT));
			_log.erase(it);
			return true;
		}
		it++;
	}
	throw NotInChan();
	return false;
}

// ? Receive message <msg>
void	Channel::receiveMsg(Message& msg) {
	_hist.push_back(msg);
}

// ? Get next message UID of channel <this>
ssize_t	Channel::getNextUid(void) {
	if (_next_uid >= SSIZE_MAX) {
		throw (PoolFull());
		return (-1);
	}
	ssize_t	id = _next_uid;
	_next_uid++;
	return id;
}

// ? Print all messages of channel <this>
void	Channel::printAllMsg(void) {
	vector<Message>::iterator	it, end;
	it = _hist.begin();
	end = _hist.end();
	while (it != end) {
		cout << "msg:" << it->getUid() << " - Content:" << it->getContent() << endl;
		it++;
	}
}

// ? Check if <usr> is logged to channel <this>
int	Channel::isLog(User& usr) {
	vector<pair<User&, int> >::iterator	it, end;
	it = _log.begin();
	end = _log.end();
	ssize_t	id = usr.getUid();
	while (it != end) {
		if (it->first.getUid() == id) {
			return it->second;
		}
		it++;
	}
	return UNKNOWN;
}

// ? Get channel <this> message history
// string	Channel::getMsgHist(User& usr) {
// 	if (isLog(usr) == CONNECTED) {
// 		vector<pair<User&, int> >::iterator	it, end;
// 		it = _log.begin();
// 		end = _log.end();
// 		ssize_t	id = usr.getUid();
// 		while (it != end) {
// 			if (it->first.getUid() == id) {
// 				ssize_t pad = getUidAfter(it->second);
// 				vector<Message>::iterator	itm, endm;
// 				itm = _hist.begin() + pad + 1;
// 				endm = _hist.end();
// 				string hist;
// 				while (itm != endm) {
// 					hist += itm->getContent();
// 					hist += "\n";
// 					itm++;
// 				}
// 				return hist;
// 			}
// 			it++;
// 		}
// 	}
// 	return "";
// }

// ? Get the list of users' connected to channel <this> nicknames (as a vector)
vector<string>	Channel::getNickLst(void) {
	vector<string>vec;
	vector<pair<User&, int> >::iterator	it, end;
	it = _log.begin();
	end = _log.end();
	while (it != end) {
		if (it->second != CONNECTED)
			continue;
		vec.push_back(it->first.getNickName());
		it++;
	}
	return vec;
}

// ? Ban <usr> from channel <this> by <banner>
bool	Channel::userKick(User& usr, User& banner, string msg) {
	if (usr.getActiveStatus() != true) {
		throw NotLoggedGlobal();
		return false;
	}
	vector<pair<User&, int> >::iterator	it, end;
	it = _log.begin();
	end = _log.end();
	ssize_t	id = usr.getUid();
	while (it != end) {
		if (it->first.getUid() == id) {
			it->second = BANNED;
			log(string(LIGHT_MAGENTA) + string("User ") + string(RED) + string(usr.getNickName()) + string(LIGHT_BLUE) + string(" has been kicked from ") + string(LIGHT_MAGENTA) + string("channel ") + string(RED) + string(_name) + string(LIGHT_BLUE) + string(" by ") + string(RED) + string(banner.getNickName()) + string(LIGHT_BLUE) + string(" with message ") + string(RED) + msg + string(DEFAULT));
			return true;
		}
		it++;
	}
	return false;
}

// // ? Check if <usr> has operator permissions for channel <this>
// bool	Channel::isOper(string nickname) {
// 	vector<pair<User&, bool> >::iterator	it, end;
// 	it = _roles.begin();
// 	end = _roles.end();
// 	// ssize_t	id = usr.getUid();
// 	while (it != end) {
// 		if (it->first.getNickName() == nickname) {
// 			if (it->second == OPERATOR) {
// 				return true;
// 			}
// 			return false;
// 		}
// 		it++;
// 	}
// 	return false;
// }

// ? Set channel <this> password
bool	Channel::setPasswd(string pass) {
	_hash = sha256(pass);
	return true;
}

// // ? Check if <pass> matches channel <this> operator password
// bool	Channel::checkOperPasswd(string pass) {
// 	if (sha256(pass) == _oper) {
// 		return true;
// 	}
// 	return false;
// }

// // ? Set channel <this> operator password
// bool	Channel::setOperPasswd(string oper_pass) {
// 	_oper = sha256(oper_pass);
// 	return true;
// }

// // ? Set <usr> role as operator for channel <this>
// bool	Channel::addOper(User& usr) {
// 	if (usr.getActiveStatus() != true) {
// 		throw NotLoggedGlobal();
// 		return false;
// 	}
// 	vector<pair<User&, bool> >::iterator	it, end;
// 	it = _roles.begin();
// 	end = _roles.end();
// 	ssize_t	id = usr.getUid();
// 	while (it != end) {
// 		if (it->first.getUid() == id) {
// 			it->second = OPERATOR;
// 			return true;
// 		}
// 		it++;
// 	}
// 	throw NotLogged();
// 	return false;
// }

// // ? Set <usr> role as user for channel <this>
// bool	Channel::removeOper(User& usr) {
// 	if (usr.getActiveStatus() != true) {
// 		throw NotLoggedGlobal();
// 		return false;
// 	}
// 	vector<pair<User&, bool> >::iterator	it, end;
// 	it = _roles.begin();
// 	end = _roles.end();
// 	ssize_t	id = usr.getUid();
// 	while (it != end) {
// 		if (it->first.getUid() == id) {
// 			it->second = USER;
// 			return true;
// 		}
// 		it++;
// 	}
// 	throw NotLogged();
// 	return false;
// }
