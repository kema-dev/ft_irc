#include "Channel.hpp"

Channel::Channel(string name, string pass, string motd) {
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
		cerr << e.info() << std::endl;
		return ;
	}
	try {
		_hash = md5(pass);
	}
	catch (PopopenFail& e) {
		cerr << e.info() << endl;
		return ;
	}
	_name = name;
	_hist.push_back(Message(motd, "MOTD", -1));
	_next_uid = 0;
	
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

bool	Channel::userJoin(User usr, string pass) {
	try {
		if (md5(pass) != _hash) {
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
	_log.push_back(pair<ssize_t, timeval>(usr.getUid(), time));
	return true;
}

bool	Channel::userLeave(User usr) {
	vector<pair<ssize_t, timeval> >::iterator	it, end;
	it = _log.begin();
	end = _log.end();
	ssize_t	id = usr.getUid();
	while (it != end) {
		if (it->first == id) {
			_log.erase(it);
			return true;
		}
		it++;
	}
	return false;
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
	vector<pair<ssize_t, timeval> >::iterator	it, end;
	it = _log.begin();
	end = _log.end();
	ssize_t	id = usr.getUid();
	while (it != end) {
		if (it->first == id) {
			return true;
		}
		it++;
	}
	return false;
}

string	Channel::getMsgHist(User usr) {
	if (isLog(usr) == true) {
		vector<pair<ssize_t, timeval> >::iterator	it, end;
		it = _log.begin();
		end = _log.end();
		ssize_t	id = usr.getUid();
		while (it != end) {
			if (it->first == id) {
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
