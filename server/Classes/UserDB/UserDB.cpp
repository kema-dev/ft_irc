#include "../UserDB/UserDB.hpp"

using namespace std;

void	UserDB::add(User usr) {
	_db.push_back(usr);
}

User*	UserDB::search(User usr) {
	vector<User>::iterator it = _db.begin(), end = _db.end();
	ssize_t	id = usr.getUid();
    while (it != end) {
		if (it->getUid() == id) {
			return &*it;
		}
	}
	return nullptr;
}

User*	UserDB::search(ssize_t id) {
	vector<User>::iterator it = _db.begin(), end = _db.end();
    while (it != end) {
		if (it->getUid() == id) {
			return &*it;
		}
	}
	return nullptr;
}
