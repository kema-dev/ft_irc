#include "Channel.hpp"

Channel::Channel(string name, string pass, string motd, string oper_pass) {
	if (name == "") {
		throw (WrongChannelName());
	}
	for (size_t i = 0; name[i]; i++) {
		if (!(isalnum(name[i]))) {
			throw (WrongChannelName());
		}
	}
	_hash = sha256(pass);
	_name = name;
	_hist.push_back(Message(motd, "MOTD", -1));
	_next_uid = 0;
	_hash = sha256(pass);
	_oper = sha256(oper_pass);
	log(string(LIGHT_MAGENTA) + string("Channel ") + string(GREEN) + string(_name) + string(" (hash: ") + string(GREEN) + string(_hash) + string(") ") + string(LIGHT_BLUE) + string("has been created") + string(DEFAULT));
}

string	Channel::getName(void){
	return _name;
}

string Channel::getTopic(void)
{
    if (_topic.empty())
        return "No topic is set.";
    return _topic;
}

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

void    Channel::setTopic(string topic)
{
    _topic = topic;
}

bool	Channel::userJoin(User& usr, string pass) {
	if (usr.getActiveStatus() != true) {
		throw NotLoggedGlobal();
		return false;
	}
	if (sha256(pass) != _hash) {
		return false;
	}
	timeval time;
	gettimeofday(&time, nullptr);
	_log.push_back(pair<User&, timeval>(usr, time));
	_roles.push_back(pair<User&, bool>(usr, USER));
	log(string(LIGHT_MAGENTA) + string("User ") + string(GREEN) + string(usr.getUserName()) + string(LIGHT_BLUE) + string(" joined ") + string(LIGHT_MAGENTA) + string("channel ") + string(GREEN) + string(_name) + string(DEFAULT));
	return true;
}

bool	Channel::userLeave(User& usr) {
	if (usr.getActiveStatus() != true) {
	throw NotLoggedGlobal();
	return false;
	}
	vector<pair<User&, timeval> >::iterator	it, end;
	vector<pair<User&, bool> >::iterator	it2, end2;
	it = _log.begin();
	end = _log.end();
	it2 = _roles.begin();
	end2 = _roles.end();
	ssize_t	id = usr.getUid();
	while (it != end && it2 != end2) {
		if (it->first.getUid() == id) {
			_log.erase(it);
			_roles.erase(it2);
			log(string(LIGHT_MAGENTA) + string("User ") + string(RED) + string(usr.getUserName()) + string(LIGHT_BLUE) + string(" left ") + string(LIGHT_MAGENTA) + string("channel ") + string(RED) + string(_name) + string(DEFAULT));
			return true;
		}
		it++;
		it2++;
	}
	return false;
}

void	Channel::receiveMsg(Message& msg) {
	_hist.push_back(msg);
}

ssize_t	Channel::getNextUid(void) {
	if (_next_uid >= SSIZE_MAX) {
		throw (PoolFull());
		return (-1);
	}
	ssize_t	id = _next_uid;
	_next_uid++;
	return id;
}

void	Channel::printAllMsg(void) {
	vector<Message>::iterator	it, end;
	it = _hist.begin();
	end = _hist.end();
	while (it != end) {
		cout << "msg:" << it->getUid() << " - Content:" << it->getContent() << endl;
		it++;
	}
}

bool	Channel::isLog(User& usr) {
	vector<pair<User&, timeval> >::iterator	it, end;
	it = _log.begin();
	end = _log.end();
	ssize_t	id = usr.getUid();
	while (it != end) {
		if (it->first.getUid() == id) {
			return true;
		}
		it++;
	}
	return false;
}

string	Channel::getMsgHist(User& usr) {
	if (isLog(usr) == true) {
		vector<pair<User&, timeval> >::iterator	it, end;
		it = _log.begin();
		end = _log.end();
		ssize_t	id = usr.getUid();
		while (it != end) {
			if (it->first.getUid() == id) {
				ssize_t pad = getUidAfter(it->second);
				vector<Message>::iterator	itm, endm;
				itm = _hist.begin() + pad + 1;
				endm = _hist.end();
				string hist;
				while (itm != endm) {
					hist += itm->getContent();
					hist += "\n";
					itm++;
				}
				return hist;
			}
			it++;
		}
	}
	return "";
}

vector<string>	Channel::getNickLst(void) {
	vector<string>vec;
	vector<pair<User&, timeval> >::iterator	it, end;
	it = _log.begin();
	end = _log.end();
	while (it != end) {
		vec.push_back(it->first.getNickName());
		it++;
	}
	return vec;
}

bool	Channel::userBan(User& usr, User& banner) {
	if (usr.getActiveStatus() != true) {
		throw NotLoggedGlobal();
		return false;
	}
	vector<pair<User&, timeval> >::iterator	it, end;
	it = _log.begin();
	end = _log.end();
	ssize_t	id = usr.getUid();
	while (it != end) {
		if (it->first.getUid() == id) {
			_log.erase(it);
			log(string(LIGHT_MAGENTA) + string("User ") + string(RED) + string(usr.getUserName()) + string(LIGHT_BLUE) + string(" has been banned from ") + string(LIGHT_MAGENTA) + string("channel ") + string(RED) + string(_name) + string(DEFAULT) + string(" by ") + string(RED) + string(banner.getUserName()) + string(DEFAULT));
			return true;
		}
		it++;
	}
	return false;
}

bool	Channel::isOper(User& usr) {
	vector<pair<User&, bool> >::iterator	it, end;
	it = _roles.begin();
	end = _roles.end();
	ssize_t	id = usr.getUid();
	while (it != end) {
		if (it->first.getUid() == id) {
			if (it->second == OPERATOR) {
				return true;
			}
			return false;
		}
		it++;
	}
	return false;
}

bool	Channel::setPasswd(string pass) {
	_hash = sha256(pass);
	return true;
}

bool	Channel::checkOperPasswd(string pass) {
	if (sha256(pass) == _oper) {
		return true;
	}
	return false;
}

bool	Channel::setOperPasswd(string oper_pass) {
	_oper = sha256(oper_pass);
	return true;
}

bool	Channel::addOper(User& usr) {
	if (usr.getActiveStatus() != true) {
		throw NotLoggedGlobal();
		return false;
	}
	vector<pair<User&, bool> >::iterator	it, end;
	it = _roles.begin();
	end = _roles.end();
	ssize_t	id = usr.getUid();
	while (it != end) {
		if (it->first.getUid() == id) {
			it->second = OPERATOR;
			return true;
		}
		it++;
	}
	throw NotLogged();
	return false;
}
