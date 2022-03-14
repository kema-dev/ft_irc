#include "../ChannelDB/ChannelDB.hpp"

using namespace std;

void	ChannelDB::add(Channel& chan) {
	_db.push_back(chan);
	log(string(LIGHT_MAGENTA) + string("Channel ") + string(GREEN) + string(chan.getName()) + string(LIGHT_BLUE) + string(" has been added to ") + string(LIGHT_MAGENTA) + string("channelDB ") + string(GREEN) + string(this->_name) + string(DEFAULT));
}

Channel*	ChannelDB::search(Channel& chan) {
	try {
		vector<Channel>::iterator it = _db.begin(), end = _db.end();
		while (it != end) {
			if (it->getName() == chan.getName()) {
				return &(*it);
			}
			it++;
		}
		throw NoSuchChan();
	}
	catch (exception& e) {
		logError(string("Searching channel"), chan.getName(), e.what());
		throw ChanSearchFail();
	}
	return nullptr;
}

Channel*	ChannelDB::search(string name) {
	try {
		vector<Channel>::iterator it = _db.begin(), end = _db.end();
		while (it != end) {
			if (it->getName() == name) {
				return &(*it);
			}
			it++;
		}
		throw NoSuchChan();
	}
	catch (exception& e) {
		logError(string("Searching channel"), name, e.what());
		throw ChanSearchFail();
	}
	return nullptr;
}
