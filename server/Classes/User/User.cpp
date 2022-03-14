#include "User.hpp"

using namespace std;

// User::User(string username, string fullname, string hostname, string servername, Server* server) {
// 	if (username == "") {
// 		throw (WrongUserName());
// 	}
// 	for (size_t i = 0; username[i]; i++) {
// 		if (!(isalnum(username[i]))) {
// 				throw (WrongUserName());
// 		}
// 	}
// 	if (fullname == "") {
// 		throw (WrongFullName());
// 	}
// 	for (size_t i = 0; fullname[i]; i++) {
// 		if (!(isalnum(fullname[i])) && fullname[i] != ' ') {
// 			throw (WrongFullName());
// 		}
// 	}
// 	size_t id;
// 	_server = server;
// 	id = server->pool->generate();
// 	_username = username;
//     _fullname = fullname;
// 	_nickname = "";
// 	_hostname = hostname;
// 	_servername = servername;
// 	_uid = id;
// 	_nb_msg = 0;
// 	_ban_status = false;
// 	_active_status = false;
// 	char s[20]; // ? length of max_ssize
// 	sprintf(s, "%ld", _uid);
// 	string str = string(s);
// 	log(string(LIGHT_MAGENTA) + string("User" DEFAULT) + string(" ") + string(GREEN) + string(_username) + string(" (uid: ") + string(s) + string(")") + string(LIGHT_BLUE) + string(" has been created") + string(DEFAULT));
// }

User::User(string username, string fullname, string nickname, string hostname, string servername, Server* server) {
	if (username == "") {
		throw (WrongUserName());
	}
	for (size_t i = 0; username[i]; i++) {
		if (!(isalnum(username[i]))) {
				throw (WrongUserName());
		}
	}
	if (fullname == "") {
		throw (WrongFullName());
	}
	for (size_t i = 0; fullname[i]; i++) {
		if (!(isalnum(fullname[i])) && fullname[i] != ' ') {
			throw (WrongFullName());
		}
	}
	if (nickname == "") {
		throw (WrongNickName());
	}
	for (size_t i = 0; nickname[i]; i++) {
		if (!(isalnum(nickname[i]))) {
			throw (WrongNickName());
		}
	}
	size_t id;
	_server = server;
	id = server->pool->generate();
	_username = username;
	_fullname = fullname;
	_nickname = nickname;
	_hostname = hostname;
	_servername = servername;
	_uid = id;
	_nb_msg = 0;
	_ban_status = false;
	_active_status = false;
	char s[20]; // ? length of ssize_max
	sprintf(s, "%ld", _uid);
	string str = string(s);
	log(string(LIGHT_MAGENTA) + string("User" DEFAULT) + string(" ") + string(GREEN) + string(_username) + string(" (uid: ") + string(s) + string(")") + string(LIGHT_BLUE) + string(" has been created") + string(DEFAULT));
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

// ostream &operator<<(ostream &stream, User &rhs)
// {
// 	stream << "User infos:" << endl << "name: " << rhs.getFullName() << endl << "nb_msg: " << rhs.getNbMsg() << endl << "ban_status: " << rhs.getBanStatus() << endl << "active_status: " << rhs.getActiveStatus() << endl << "uid: " << rhs.getUid() << endl << "hash: " << rhs.getHash();
//     return stream;
// }

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
	_hash = sha256(new_pass);
	return true;
}

bool	User::logIn(Server& server) {
	if ((server.userDB->search(*this) == nullptr) || (server.userDB->search(*this)->getActiveStatus() != false)) {
		throw AlreadyLogged();
	}
	this->setActiveStatus(true);
	log(string(LIGHT_MAGENTA) + string("User ") + string(GREEN) + string(this->getUserName()) + string(LIGHT_BLUE) + string(" logged in to ") + string(LIGHT_MAGENTA) + string("server ") + string(GREEN) + server.name + string(DEFAULT));
	return true;
}

bool	User::logOut(Server& server) {
	if ((server.userDB->search(*this) == nullptr) || (server.userDB->search(*this)->getActiveStatus() != false)) {
		throw NotLogged();
	}
	this->setActiveStatus(false);
	log(string(LIGHT_MAGENTA) +  string("User ") +  string(RED) +  string(this->getUserName()) +  string(LIGHT_BLUE) +  string(" logged out from ") +  string(RED) + string(LIGHT_MAGENTA) + string("server ") +  string(DEFAULT));
	return true;
}

void	User::sendMessage(string content, Channel& chan) {
	if (chan.isLog(*this) != true) {
		throw NotLogged();
		return;
	}
	Message msg;
	msg = Message(content, this->getUserName(), chan.getNextUid());
	chan.receiveMsg(msg);
	this->setNbMsg(getNbMsg() + 1);
	log(string(LIGHT_MAGENTA) + string("User ") + string(GREEN) + string(this->getUserName()) + string(LIGHT_BLUE) + string(" sent message to ") + string(LIGHT_MAGENTA) + string("channel ") + string(GREEN) + string(chan.getName()) + string(DEFAULT));
}

void	User::joinChannel(Channel& chan, string pass) {
	if (chan.isLog(*this) != true) {
		throw NotLogged();
		return;
	}
	bool auth = chan.userJoin(*this, pass);
	if (auth == true) {
		return;
	}
	throw BadPasswd();
	return;
}

bool	User::ban(User& usr, Channel& chan) {
	if (chan.isLog(usr) != true) {
		throw NotLogged();
	}
	usr.getBanned(chan, *this);
	return true;
}

void	User::getBanned(Channel& chan, User& banner) {
	chan.userBan(*this, banner);
	// TODO Send ban info to *this
}

bool	User::setPasswd(Channel& chan, string pass) {
	if (chan.isOper(*this) != true) {
		throw BadRole();
	}
	chan.setPasswd(pass);
	return true;
}

bool	User::setOperPasswd(Channel& chan, string pass) {
	if (chan.isOper(*this) != true) {
		throw BadRole();
	}
	chan.setOperPasswd(pass);
	return true;
}

bool	User::becomeOper(Channel& chan, string pass) {
	if (chan.isLog(*this)) {
		if (chan.checkOperPasswd(pass) == true) {
			chan.addOper(*this);
			log(string(LIGHT_MAGENTA) + string("User ") + string(GREEN) + string(this->getUserName()) + string(LIGHT_BLUE) + string(" became operator of ") + string(LIGHT_MAGENTA) + string("channel ") + string(GREEN) + string(chan.getName()) + string(DEFAULT));
			return true;
		}
		else {
			throw BadPasswd();
			return false;
		}
	}
	else {
		throw NotLogged();
		return false;
	}
	return false;
}
