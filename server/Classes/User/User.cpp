#include "User.hpp"

using namespace std;

User::User(string username, string fullname, string role, UidPool& pool) {
	try {
		if (username == "") {
			throw (WrongUserName());
		}
		for (size_t i = 0; username[i]; i++) {
			if (!(isalnum(username[i]))) {
		    		throw (WrongUserName());
			}
		}
        if (fullname == "") {
			throw (WrongUserName());
		}
		for (size_t i = 0; fullname[i]; i++) {
			if (!(isalnum(fullname[i])) && fullname[i] != ' ') {
		    	throw (WrongUserName());
			}
		}
	}
	catch (WrongUserName& e) {
		cerr << e.info() << endl;
		return ;
	}
	try {
		if (role == "") {
			throw (WrongRoleNameUser());
		}
		if (role != "user" && role != "operator") {
			throw (WrongRoleNameUser());
		}
	}
	catch (WrongRoleNameUser& e) {
		cerr << e.info() << endl;
		return ;
	}
	size_t id;
	try {
		id = pool.generate();
	}
	catch (PoolFull& e) {
		cerr << e.info() << endl;
		return ;
	}
	_username = username;
    _fullname = fullname;
	_nickname = "";
	_role = role;
	_uid = id;
	_nb_msg = 0;
	_ban_status = false;
	_active_status = false;
	char s[15];
	sprintf(s, "%ld", _uid);
	string str = string(s);
	log(LIGHT_MAGENTA, "User" DEFAULT, " username: ", GREEN, _username, DEFAULT, " - uid: ", GREEN, s, DEFAULT, " - role: ", GREEN, _role, DEFAULT, LIGHT_BLUE," has been created",DEFAULT);
}

User::User(string username, string fullname, string nickname, string role, UidPool& pool) {
	try {
		if (username == "") {
			throw (WrongUserName());
		}
		for (size_t i = 0; username[i]; i++) {
			if (!(isalnum(username[i]))) {
		    		throw (WrongUserName());
			}
		}
        if (fullname == "") {
			throw (WrongUserName());
		}
		for (size_t i = 0; fullname[i]; i++) {
			if (!(isalnum(fullname[i])) && fullname[i] != ' ') {
		    	throw (WrongUserName());
			}
		}
        if (nickname == "") {
			throw (WrongUserName());
		}
		for (size_t i = 0; nickname[i]; i++) {
			if (!(isalnum(nickname[i]))) {
		    	throw (WrongUserName());
			}
		}
	}
	catch (WrongUserName& e) {
		cerr << e.info() << endl;
		return ;
	}
	try {
		if (role == "") {
			throw (WrongUserName());
		}
		if (role != "user" && role != "operator") {
			throw (WrongUserName());
		}
	}
	catch (WrongRoleNameUser& e) {
		cerr << e.info() << endl;
		return ;
	}
	size_t id;
	try {
		id = pool.generate();
	}
	catch (PoolFull& e) {
		cerr << e.info() << endl;
		return ;
	}
	_username = username;
	_fullname = fullname;
	_nickname = nickname;
	_role = role;
	_uid = id;
	_nb_msg = 0;
	_ban_status = false;
	_active_status = false;
	char s[15];
	sprintf(s, "%ld", _uid);
	string str = string(s);
	log(LIGHT_MAGENTA, "User" DEFAULT, " username: ", GREEN, _username, DEFAULT, " - uid: ", GREEN, s, DEFAULT, " - role: ", GREEN, _role, DEFAULT, LIGHT_BLUE," has been created",DEFAULT);
}

string	User::getNickName(void) {
	return _nickname;
}

string	User::getUserName(void) {
	return _username;
}

string	User::getFullName(void) {
	return _fullname;
}

string	User::getRole(void) {
	return _role;
}

ssize_t	User::getNbMsg(void) {
	return _nb_msg;
}

bool	User::getBanStatus(void) {
	return _ban_status;	
}

bool	User::getActiveStatus(void) {
	return _active_status;	
}

ssize_t	User::getUid(void) {
	return _uid;	
}

string	User::getHash(void) {
	return _hash;	
}

ostream &operator<<(ostream &stream, User &rhs)
{
	stream << "User infos:" << endl << "name: " << rhs.getFullName() << endl << "role: " << rhs.getRole() << endl << "nb_msg: " << rhs.getNbMsg() << endl << "ban_status: " << rhs.getBanStatus() << endl << "active_status: " << rhs.getActiveStatus() << endl << "uid: " << rhs.getUid() << endl << "hash: " << rhs.getHash();
    return stream;
}

bool	User::setUserName(string new_username) {
	_username = new_username;
	return true;
}

bool	User::setNickName(string new_nickname) {
	_nickname = new_nickname;
	return true;
}

bool	User::setFullName(string new_fullname) {
	_fullname = new_fullname;
	return true;
}

bool	User::setRole(string new_role) {
	_role = new_role;
	return true;
}

bool	User::setNbMsg(ssize_t new_nb_msg) {
	_nb_msg = new_nb_msg;
	return true;
}

bool	User::setBanStatus(bool new_ban_status) {
	_ban_status = new_ban_status;
	return true;
}

bool	User::setActiveStatus(bool new_active_status) {
	_active_status = new_active_status;
	return true;
}

bool	User::setUid(ssize_t new_uid) {
	_uid = new_uid;
	return true;
}

bool	User::setHash(string new_hash) {
	_hash = new_hash;
	return true;
}

bool	User::setPass(string new_pass) {
	try {
		_hash = md5(new_pass);
	}
	catch (PopopenFail& e) {
		cerr << e.info() << endl;
		return false;
	}
	return true;
}

bool	User::logIn(UserDB* db) {
	try {
		if ((db->search(this) == nullptr) || (db->search(this)->getActiveStatus() != false)) {
			throw AlreadyLogged();
		}
	}
	catch (SameInfo& e) {
		cerr << e.info() << endl;
		return false;
	}
	try {
		if ((db->search(this) == nullptr) || (db->search(this)->getActiveStatus() != false)) {
			throw AlreadyLogged();
		}
	}
	catch (AlreadyLogged& e) {
		cerr << e.info() << endl;
		return false;
	}
	this->setActiveStatus(true);
	log(LIGHT_MAGENTA, "User ", GREEN, this->getFullName(), LIGHT_BLUE, " logged in to ", GREEN, "the server", DEFAULT);
	return true;
}

bool	User::logOut(UserDB* db) {
	try {
		if ((db->search(this) == nullptr) || (db->search(this)->getActiveStatus() != true)) {
			throw NotLoggedGlobal();
		}
	}
	catch (NotLoggedGlobal& e) {
		cerr << e.info() << endl;
		return false;
	}
	this->setActiveStatus(false);
	log(LIGHT_MAGENTA, "User ", RED, this->getFullName(), LIGHT_BLUE, " logged out from ", RED, "the server", DEFAULT);
	return true;
}

bool	User::sendMessage(string content, Channel* chan) {
	if (chan->isLog(*this) == true) {
		Message msg;
		try {
			msg = Message(content, this->getFullName(), chan->getNextUid());
		}
		catch (PoolFull& e) {
			cerr << e.info() << endl;
			return false;
		}
		chan->receiveMsg(msg);
		this->setNbMsg(getNbMsg() + 1);
		log(GREEN, this->getFullName(), LIGHT_BLUE, " sent message to ", GREEN, chan->getName(), DEFAULT);
		return true;
	}
	return false;
}

bool	User::joinChannel(Channel* chan, string pass) {
	return chan->userJoin(*this, pass);
}

bool	User::ban(User& usr, Channel& chan) {
	try {
		if (this->getRole() != "operator") {
			throw BadRole();
		}
	}
	catch (BadRole& e) {
		cerr << e.info() << endl;
		return false;
	}
	try {
		if (chan.isLog(usr) != true) {
			throw NotLogged();
		}
	}
	catch (NotLogged& e) {
		cerr << e.info() << endl;
	}
	usr.getBanned(chan, *this);
	return true;
}

void	User::getBanned(Channel& chan, User& banner) {
	chan.userBan(*this, banner);
	// TODO Send ban info to *this
}
