#include "../UserDB/UserDB.hpp"

using namespace std;

// ? Add a <usr> to database
ssize_t	UserDB::add(User& usr) {
	_db.push_back(pair<User&, bool>(usr, USER));
	log(string(LIGHT_MAGENTA) + string("User ") + string(GREEN) + string(usr.getNickName()) + string(LIGHT_BLUE) + string(" has been added to ") + string(LIGHT_MAGENTA) + string("userDB ") + string(GREEN) + string(this->_name) + string(DEFAULT));
	return usr.getUid();
}

// ? Search <usr> in database (from reference)
User*	UserDB::search(User& usr) {
    vector<pair<User&, bool> >::iterator it = _db.begin(), end = _db.end();
    ssize_t	id = usr.getUid();
    while (it != end) {
        if (it->first.getUid() == id) {
            return &(it->first);
        }
        it++;
    }
    throw NoSuchUser();
	return nullptr;
}

// ? Search user in database from ID <id>
User*	UserDB::search(ssize_t id) {
    vector<pair<User&, bool> >::iterator it = _db.begin(), end = _db.end();
    while (it != end) {
        if (it->first.getUid() == id) {
            return &(it->first);
        }
        it++;
    }
    throw NoSuchUser();
	return nullptr;
}

// ? Search user in database from NickName <nickname>
User*	UserDB::search(string nickname) {
    vector<pair<User&, bool> >::iterator it = _db.begin(), end = _db.end();
    while (it != end) {
        if (it->first.getNickName() == nickname) {
            return &(it->first);
        }
        it++;
    }
    throw NoSuchUser();
	return nullptr;
}

// ? Return User database
vector<pair<User&, bool> > UserDB::getDB() {
    return _db;
}

// ? Check if <username>, <fullname> and <nickname> are uniques
void	UserDB::chkDuplicate(string username, string fullname, string nickname) {
	try {
		vector<pair<User&, bool> >::iterator it = _db.begin(), end = _db.end();
		while (it != end) {
			if (it->first.getUserName() == username) {
				throw DuplicateUsername();
			}
			if (it->first.getFullName() == fullname) {
				throw DuplicateFullname();
			}
			if (it->first.getNickName() == nickname) {
				throw DuplicateNickname();
			}
			it++;
		}
	}
	catch (exception& e) {
		logError(string("Checking user duplicate"), username, e.what());
		throw UserDuplicate();
	}
}

// void	UserDB::remove(string name) {
// 	try {
// 		vector<User>::iterator it = _db.begin(), end = _db.end();
// 		while (it != end) {
// 			if (it->getFullName() == name) {
// 				_db.erase(it);
// 				log(string(LIGHT_MAGENTA) + string("User ") + string(RED) + name + string(LIGHT_BLUE) + string(" has been removed from ") + string(LIGHT_MAGENTA) + string("userDB ") + string(RED) + string(this->_name) + string(DEFAULT));
// 				return ;
// 			}
// 			it++;
// 		}
// 		throw NoSuchChan();
// 	}
// 	catch (exception& e) {
// 		logError(string("Removing channel"), name, e.what());
// 		throw ChanRemoveFail();
// 	}
// }

// ? Check if <usr> has operator permissions for server <this>
bool	UserDB::isOper(string nickname) {
	vector<pair<User&, bool> >::iterator	it, end;
	it = _db.begin();
	end = _db.end();
	// ssize_t	id = usr.getUid();
	while (it != end) {
		if (it->first.getNickName() == nickname) {
			if (it->second == OPERATOR) {
				return true;
			}
			return false;
		}
		it++;
	}
	return false;
}
// ? Check if <pass> matches server <this> operator password
bool	UserDB::checkOperPasswd(string pass) {
	if (sha256(pass) == _oper) {
		return true;
	}
	return false;
}

// ? Set server <this> operator password
bool	UserDB::setOperPasswd(string oper_pass) {
	_oper = sha256(oper_pass);
	return true;
}

// ? Set <usr> role as operator for server <this>
bool	UserDB::addOper(User& usr) {
	if (usr.getActiveStatus() != true) {
		throw NotLoggedGlobal();
		return false;
	}
	vector<pair<User&, bool> >::iterator	it, end;
	it = _db.begin();
	end = _db.end();
	ssize_t	id = usr.getUid();
	while (it != end) {
		if (it->first.getUid() == id) {
			it->second = OPERATOR;
			return true;
		}
		it++;
	}
	throw NotLogged();
	return false;
}

// ? Set <usr> role as user for server <this>
bool	UserDB::removeOper(User& usr) {
	if (usr.getActiveStatus() != true) {
		throw NotLoggedGlobal();
		return false;
	}
	vector<pair<User&, bool> >::iterator	it, end;
	it = _db.begin();
	end = _db.end();
	ssize_t	id = usr.getUid();
	while (it != end) {
		if (it->first.getUid() == id) {
			it->second = USER;
			return true;
		}
		it++;
	}
	throw NotLogged();
	return false;
}
