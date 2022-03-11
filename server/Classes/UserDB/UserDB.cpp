#include "../UserDB/UserDB.hpp"

using namespace std;

void	UserDB::add(User& usr) {
	_db.push_back(usr);
	log(string(LIGHT_MAGENTA) + string("User ") + string(GREEN) + string(usr.getFullName()) + string(LIGHT_BLUE) + string(" has been added to ") + string(LIGHT_MAGENTA) + string("userDB ") + string(GREEN) + string(this->_name) + string(DEFAULT));
}

User*	UserDB::search(User& usr) {
	vector<User>::iterator it = _db.begin(), end = _db.end();
	ssize_t	id = usr.getUid();
    while (it != end) {
		if (it->getUid() == id) {
			return &(*it);
		}
	}
	return nullptr;
}

User*	UserDB::search(ssize_t id) {
	vector<User>::iterator it = _db.begin(), end = _db.end();
    while (it != end) {
		if (it->getUid() == id) {
			return &(*it);
		}
	}
	return nullptr;
}

bool	UserDB::check(string username, string fullname, string nickname) {
	vector<User>::iterator it = _db.begin(), end = _db.end();
	while (it != end) {
		if ((it->getUserName() == username) && (it->getFullName() == fullname) && (it->getNickName() == nickname)) {
			return false;
		}
	}
	return true;
}
