#include "../ChannelDB/ChannelDB.hpp"

using namespace std;

void	ChannelDB::add(Channel* chan) {
	_db.push_back(chan);
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
