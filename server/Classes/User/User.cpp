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

void	User::setUserName(string new_username) {
	_username = new_username;
}

void	User::setNickName(string new_nickname) {
	_nickname = new_nickname;
}

void	User::setFullName(string new_fullname) {
	_fullname = new_fullname;
}

void	User::setNbMsg(ssize_t new_nb_msg) {
	_nb_msg = new_nb_msg;
}

void	User::setBanStatus(bool new_ban_status) {
	_ban_status = new_ban_status;
}

void	User::setActiveStatus(bool new_active_status) {
	_active_status = new_active_status;
}

void	User::setUid(ssize_t new_uid) {
	_uid = new_uid;
}

void	User::setHash(string new_hash) {
	_hash = new_hash;
}

void	User::setPass(string new_pass) {
	_hash = sha256(new_pass);
}

void	User::logIn(Server& server) {
	if ((server.userDB->search(*this) == nullptr) || (server.userDB->search(*this)->getActiveStatus() != false)) {
		throw AlreadyLogged();
	}
	this->setActiveStatus(true);
	log(string(LIGHT_MAGENTA) + string("User ") + string(GREEN) + string(this->getUserName()) + string(LIGHT_BLUE) + string(" logged in to ") + string(LIGHT_MAGENTA) + string("server ") + string(GREEN) + server.name + string(DEFAULT));
}

void	User::logOut(Server& server) {
	if ((server.userDB->search(*this) == nullptr) || (server.userDB->search(*this)->getActiveStatus() != false)) {
		throw NotLogged();
	}
	this->setActiveStatus(false);
	log(string(LIGHT_MAGENTA) +  string("User ") +  string(RED) +  string(this->getUserName()) +  string(LIGHT_BLUE) +  string(" logged out from ") +  string(RED) + string(LIGHT_MAGENTA) + string("server ") +  string(DEFAULT));
}

void	User::sendMessage(string content, Channel& chan) {
	if (chan.isLog(*this) != true) {
		throw NotLogged();
		return;
	}
	Message msg = Message(content, this->getUserName(), chan.getNextUid());
	chan.receiveMsg(msg);
	this->setNbMsg(getNbMsg() + 1);
	log(string(LIGHT_MAGENTA) + string("User ") + string(GREEN) + string(this->getUserName()) + string(LIGHT_BLUE) + string(" sent message to ") + string(LIGHT_MAGENTA) + string("channel ") + string(GREEN) + string(chan.getName()) + string(DEFAULT));
}

void	User::joinChannel(Channel& chan, string pass) {
	bool auth = chan.userJoin(*this, pass);
	if (auth == true) {
		return;
	}
	throw BadPasswd();
	return;
}

void	User::ban(User& usr, Channel& chan) {
	if (chan.isLog(usr) != true) {
		throw NotLogged();
	}
	usr.getBanned(chan, *this);
	return;
}

void	User::getBanned(Channel& chan, User& banner) {
	if (chan.isLog(banner) != true) {
		throw NotLogged();
		return;
	}
	chan.userBan(*this, banner);
	// TODO Send ban info to *this
}

void	User::setPasswd(Channel& chan, string pass) {
	if (chan.isLog(*this) != true) {
		throw NotLogged();
		return;
	}
	if (chan.isOper(*this) != true) {
		throw BadRole();
	}
	chan.setPasswd(pass);
	return;
}

void	User::setOperPasswd(Channel& chan, string pass) {
	if (chan.isLog(*this) != true) {
		throw NotLogged();
		return;
	}
	if (chan.isOper(*this) != true) {
		throw BadRole();
	}
	chan.setOperPasswd(pass);
}

void	User::becomeOper(Channel& chan, string pass) {
	if (chan.isLog(*this)) {
		if (chan.checkOperPasswd(pass) == true) {
			chan.addOper(*this);
			log(string(LIGHT_MAGENTA) + string("User ") + string(GREEN) + string(this->getUserName()) + string(LIGHT_BLUE) + string(" became operator of ") + string(LIGHT_MAGENTA) + string("channel ") + string(GREEN) + string(chan.getName()) + string(DEFAULT));
			return;
		}
		else {
			throw BadPasswd();
			return;
		}
	}
	else {
		throw NotLogged();
	}
}
