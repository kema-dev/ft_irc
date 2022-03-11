#include "../ChannelDB/ChannelDB.hpp"

using namespace std;

void	ChannelDB::add(Channel* chan) {
	_db.push_back(chan);
	log(string(LIGHT_MAGENTA) + string("Channel ") + string(GREEN) + string(chan->getName()) + string(LIGHT_BLUE) + string(" has been added to channel database " GREEN) + string(this->_name) + string(DEFAULT));
}

Channel*	ChannelDB::search(Channel* chan) {
	vector<Channel*>::iterator it = _db.begin(), end = _db.end();
    while (it != end) {
		if ((*it)->getName() == chan->getName()) {
			return *it;
		}
	}
	return nullptr;
}

Channel*	ChannelDB::search(string name) {
	vector<Channel*>::iterator it = _db.begin(), end = _db.end();
    while (it != end) {
		if ((*it)->getName() == name) {
			return *it;
		}
	}
	return nullptr;
}
