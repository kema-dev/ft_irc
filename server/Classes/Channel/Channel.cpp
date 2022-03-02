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
	_name = name;
	_hash = md5(pass);
	_hist.push_back(Message(motd, "MOTD", -1));
	_next_uid = 0;
}

ssize_t	Channel::getUidAfter(timeval time) {
	vector<Message>::iterator it, end;
	it = _hist.begin();
	end = _hist.end();
	while (it != end) {
		if (it->compareTime(time) == true) {
			// cerr << "UID:" << it->getUid() << endl;
			return (it->getUid() + 1);
		}
		it++;
	}
	return -1;
}

bool	Channel::userJoin(User usr, string pass) {
	if (md5(pass) != _hash) {
		return false;
	}
	timeval time;
	gettimeofday(&time, nullptr); // TODO ANCHOR secure function
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
		ssize_t	id = usr.getUid(); // ! ANCHOR remove this / how does the last msg get timestamp before any other
		while (it != end) {
			if (it->first == id) {
				ssize_t pad = getUidAfter(it->second);
				vector<Message>::iterator	itm, endm;
				itm = _hist.begin() + pad;
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
