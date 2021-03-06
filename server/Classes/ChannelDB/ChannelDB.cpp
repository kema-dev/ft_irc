#include "../ChannelDB/ChannelDB.hpp"

using namespace std;

// ? Add a channel to database
void	ChannelDB::add(Channel* chan) {
	_db.push_back(chan);
	log(string(LIGHT_MAGENTA) + string("Channel ") + string(GREEN) + string(chan->getName()) + string(LIGHT_BLUE) + string(" has been added to ") + string(LIGHT_MAGENTA) + string("channelDB ") + string(GREEN) + string(this->_name) + string(DEFAULT));
}

// ? Remove chan with name <name> from database
void	ChannelDB::remove(string name) {
	try {
		vector<Channel*>::iterator it = _db.begin(), end = _db.end();
		while (it != end) {
			if ((*it)->getName() == name) {
				_db.erase(it);
				log(string(LIGHT_MAGENTA) + string("Channel ") + string(RED) + name + string(LIGHT_BLUE) + string(" has been removed from ") + string(LIGHT_MAGENTA) + string("channelDB ") + string(RED) + string(this->_name) + string(DEFAULT));
				return ;
			}
			it++;
		}
		throw NoSuchChan();
	}
	catch (exception& e) {
		logError(string("Removing channel"), name, e.what());
		throw ChanRemoveFail();
	}
}

// ? Search <chan>
Channel*	ChannelDB::search(Channel* chan) {
    vector<Channel*>::iterator it = _db.begin(), end = _db.end();
    while (it != end) {
        if ((*it)->getName() == chan->getName()) {
            return *it;
        }
        it++;
    }
    throw NoSuchChan();
	return NULL;
}

// ? Search chan with name <name> in database
Channel*	ChannelDB::search(string name) {
    vector<Channel*>::iterator it = _db.begin(), end = _db.end();
    while (it != end) {
        if ((*it)->getName() == name) {
            return *it;
        }
        it++;
    }
    throw NoSuchChan();
	return NULL;
}

// ? Check if <name> already exists as channel name
void	ChannelDB::chkDuplicate(string name) {
	try {
		vector<Channel*>::iterator it = _db.begin(), end = _db.end();
		while (it != end) {
			if ((*it)->getName() == name) {
				throw DuplicateName();
			}
			it++;
		}
	}
	catch (exception& e) {
		logError(string("Checking chan duplicate"), name, e.what());
		throw ChanDuplicate();
	}
}

vector<Channel*> ChannelDB::getDB(void)
{
	return (_db);
}
