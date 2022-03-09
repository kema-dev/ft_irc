#include "../UserDB/UserDB.hpp"

using namespace std;

void	UserDB::add(User* usr) {
	_db.push_back(usr);
	log(LIGHT_MAGENTA, "User ", GREEN, usr->getFullName(), LIGHT_BLUE, " has been added to user database " GREEN, this->_name, DEFAULT);
}

User*	UserDB::search(User* usr) {
	vector<User*>::iterator it = _db.begin(), end = _db.end();
	ssize_t	id = usr->getUid();
    while (it != end) {
		if ((*it)->getUid() == id) {
			return *it;
		}
	}
	return nullptr;
}

User*	UserDB::search(ssize_t id) {
	vector<User*>::iterator it = _db.begin(), end = _db.end();
    while (it != end) {
		if ((*it)->getUid() == id) {
			return *it;
		}
	}
	return nullptr;
}

bool	UserDB::check(string username, string fullname, string nickname) {
	try {
		vector<User*>::iterator it = _db.begin(), end = _db.end();
		while (it != end) {
			if (((*it)->getUserName() == username) && ((*it)->getFullName() == fullname) && ((*it)->getNickName() == nickname)) {
				throw SameInfo();
			}
		}
	}
	catch (SameInfo &e) {
		cerr << e.info() << endl;
		return false;
	}
	return true;
}
