#include "Channel.hpp"

Channel::Channel(string name, string pass, string motd, string oper_pass) {
	try {
		if (name == "") {
			throw (WrongChannelName());
		}
		for (size_t i = 0; name[i]; i++) {
			if (!(isalnum(name[i]))) {
				throw (WrongChannelName());
			}
		}
	}
	catch (WrongChannelName& e) {
		cerr << e.info() << endl;
		return ;
	}
	try {
		_hash = sha256(pass);
	}
	catch (PopopenFail& e) {
		cerr << e.info() << endl;
		return ;
	}
	_name = name;
	_hist.push_back(Message(motd, "MOTD", -1));
	_next_uid = 0;
	_hash = sha256(pass);
	_oper = sha256(oper_pass);
	log(LIGHT_MAGENTA, "Channel ", GREEN, _name, DEFAULT, " with hash ", GREEN, _hash, LIGHT_BLUE, " has been created", DEFAULT);
}

string	Channel::getName(void){
	return _name;
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

bool	Channel::userJoin(User& usr, string pass) {
	try {
		if (sha256(pass) != _hash) {
			return false;
		}
	}
	catch (PopopenFail& e) {
		cerr << e.info() << endl;
		return false;
	}
	timeval time;
	try {
		gettimeofday(&time, nullptr);
	}
	catch (exception& e) {
		cerr << e.what() << endl;
		return false;
	}
	_log.push_back(pair<User&, timeval>(usr, time));
	_roles.push_back(pair<User&, bool>(usr, USER));
	log(GREEN, usr.getFullName(), LIGHT_BLUE, " joined ", LIGHT_MAGENTA, "channel ", GREEN, _name, DEFAULT);
	return true;
}

bool	Channel::userLeave(User& usr) {
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
			return true;
		}
		it++;
		it2++;
	}
	return false;
	log(RED, usr.getFullName(), LIGHT_BLUE, " left ", LIGHT_MAGENTA, "channel ", RED, _name, DEFAULT);
}

void	Channel::receiveMsg(Message msg) {
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

bool	Channel::isLog(User usr) {
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

string	Channel::getMsgHist(User usr) {
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
	vector<pair<User&, timeval> >::iterator	it, end;
	it = _log.begin();
	end = _log.end();
	ssize_t	id = usr.getUid();
	while (it != end) {
		if (it->first.getUid() == id) {
			_log.erase(it);
			log(RED, usr.getFullName(), LIGHT_BLUE, " has been banned from ", LIGHT_MAGENTA, "channel ", RED, _name, DEFAULT, " by ", RED, banner.getFullName(), DEFAULT);
			return true;
		}
		it++;
	}
	return false;
}

bool	Channel::isOper(User usr) {
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

bool	Channel::setOperPasswd(string oper_pass) {
	_oper = sha256(oper_pass);
	return true;
}
