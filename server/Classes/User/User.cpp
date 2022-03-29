#include "User.hpp"

// ? Create a user
User::User(string username, string fullname, string nickname, string hostname, string servername, Server* server, int socket) {
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
    _socket = socket;
	_uid = id;
	_nb_msg = 0;
	_active_status = false;
	char s[20]; // ? length of ssize_max
	sprintf(s, "%ld", _uid);
	string str = string(s);
	log(string(LIGHT_MAGENTA) + string("User" DEFAULT) + string(" ") + string(GREEN) + string(_nickname) + string(" (uid: ") + string(s) + string(")") + string(LIGHT_BLUE) + string(" has been created") + string(DEFAULT));
}

// ? Get <this> nick name
string	User::getNickName(void) {
	return _nickname;
}

// ? Get <this> user name
string	User::getUserName(void) {
	return _username;
}

// ? Get <this> ful name
string	User::getFullName(void) {
	return _fullname;
}

// ? Get <this> host name
string	User::getHostName(void) {
	return _hostname;
}

// ? Get <this> number of sent messages
ssize_t	User::getNbMsg(void) {
	return _nb_msg;
}

// ? Get <this> active status
bool	User::getActiveStatus(void) {
	return _active_status;	
}

// ? Get <this> UID
ssize_t	User::getUid(void) {
	return _uid;
}

int     User::getSocket(void) {
    return (_socket);
}

// ostream &operator<<(ostream &stream, User &rhs)
// {
// 	stream << "User infos:" << endl << "name: " << rhs.getFullName() << endl << "nb_msg: " << rhs.getNbMsg() << endl << "ban_status: " << rhs.getBanStatus() << endl << "active_status: " << rhs.getActiveStatus() << endl << "uid: " << rhs.getUid() << endl << "hash: " << rhs.getHash();
//     return stream;
// }

// ? Set <this> user name
void	User::setUserName(string new_username) {
	_username = new_username;
}

// ? Set <this> nick name
void	User::setNickName(string new_nickname) {
	_nickname = new_nickname;
}

// ? Set <this> full name
void	User::setFullName(string new_fullname) {
	_fullname = new_fullname;
}

// ? Set <this> host name
void	User::setHostName(string new_hostname) {
	_hostname = new_hostname;
}

// ? Set <this> number of sent messages
void	User::setNbMsg(ssize_t new_nb_msg) {
	_nb_msg = new_nb_msg;
}

// ? Set <this> active status
void	User::setActiveStatus(bool new_active_status) {
	_active_status = new_active_status;
}

// ? Set <this> UID
void	User::setUid(ssize_t new_uid) {
	_uid = new_uid;
}

// ? Log in to <server>
void	User::logIn(Server& server) {
	if ((server.userDB->search(*this) == nullptr) || (server.userDB->search(*this)->getActiveStatus() != false)) {
		throw AlreadyLoggedGlobal();
	}
	this->setActiveStatus(true);
	log(string(LIGHT_MAGENTA) + string("User ") + string(GREEN) + string(this->getNickName()) + string(LIGHT_BLUE) + string(" logged in to ") + string(LIGHT_MAGENTA) + string("server ") + string(GREEN) + server.name + string(DEFAULT));
}

// ? Log out from <server>
void	User::logOut(Server& server) {
	if ((server.userDB->search(*this) == nullptr) || (server.userDB->search(*this)->getActiveStatus() != true)) {
		throw NotLoggedGlobal();
	}
	this->setActiveStatus(false);
	log(string(LIGHT_MAGENTA) +  string("User ") +  string(RED) +  string(this->getNickName()) +  string(LIGHT_BLUE) +  string(" logged out from ") +  string(RED) + string(LIGHT_MAGENTA) + string("server ") +  string(DEFAULT));
}

// ? Send <content> to <chan>
void	User::sendMessage(string content, Channel& chan) {
	if (this->getActiveStatus() != true) {
		throw NotLoggedGlobal();
		return ;
	}
	if (chan.isLog(*this) != true) {
		throw NotLogged();
		return;
	}
	Message msg = Message(content, this->getNickName(), chan.getNextUid());
	chan.receiveMsg(msg);
	this->setNbMsg(getNbMsg() + 1);
	log(string(LIGHT_MAGENTA) + string("User ") + string(GREEN) + string(this->getNickName()) + string(LIGHT_BLUE) + string(" sent message to ") + string(LIGHT_MAGENTA) + string("channel ") + string(GREEN) + string(chan.getName()) + string(DEFAULT));
}

// ? Join <chan> with password <pass>
void	User::joinChannel(Channel& chan, string pass) {
	if (this->getActiveStatus() != true) {
		throw NotLoggedGlobal();
		return ;
	}
	if (chan.isLog(*this) == true) {
		throw AlreadyLogged();
		return ;
	}
	bool auth = chan.userJoin(*this, pass);
	if (auth == true) {
		return;
	}
	throw BadPasswd();
	return;
}

// ? Try to join <chan> with password <pass> and create it if non-existent
void	User::tryJoinChannel(string name, string pass, string topic, Server* server) {
	if (this->getActiveStatus() != true) {
		throw NotLoggedGlobal();
		return ;
	}
	try {
		server->chanDB->search(name);
	}
	catch (exception& e) {
		server->addChan(name, pass, topic);
	}
	if (server->chanDB->search(name)->isLog(*this) == true) {
		throw AlreadyLogged();
		return ;
	}
	bool auth = server->chanDB->search(name)->userJoin(*this, pass);
	if (auth == true) {
		return ;
	}
	throw BadPasswd();
	return ;
}

// ? Ban <usr> form <chan>
void	User::ban(User& usr, Channel& chan) {
	if (this->getActiveStatus() != true) {
		throw NotLoggedGlobal();
		return ;
	}
	if (chan.isLog(usr) != true) {
		throw NotLogged();
	}
	usr.getBanned(chan, *this);
	return;
}

// ? Get banned from <chan> by <banner>
void	User::getBanned(Channel& chan, User& banner) {
	if (chan.isLog(banner) != true) {
		throw NotLogged();
		return;
	}
	chan.userBan(*this, banner);
	// TODO Send ban info to *this
}

// ? Set <serv> password with password <pass>
void	User::setPasswd(Server& serv, string pass) {
	if (this->getActiveStatus() != true) {
		throw NotLoggedGlobal();
		return ;
	}
	if (serv.userDB->isOper(this->getNickName()) != true) {
		throw BadRole();
	}
	serv.setPasswd(pass);
	return;
}

void    User::setSocket(int socket)
{
    _socket = socket;
}

// ? Set <serv> operator password with password <pass>
void	User::setOperPasswd(Server& serv, string pass) {
	if (this->getActiveStatus() != true) {
		throw NotLoggedGlobal();
		return ;
	}
	if (serv.userDB->isOper(this->getNickName()) != true) {
		throw BadRole();
	}
	serv.userDB->setOperPasswd(pass);
}

// ? Become operator of <chan> with password <pass>
void	User::becomeOper(Server& serv, string pass) {
	if (this->getActiveStatus() != true) {
		throw NotLoggedGlobal();
		return ;
	}
	if (serv.userDB->checkOperPasswd(pass) == true) {
		if (serv.userDB->isOper(this->getNickName()) == true) {
			log(string(LIGHT_MAGENTA) + string("User ") + string(GREEN) + string(this->getNickName()) + string(LIGHT_BLUE) + string(" is already operator and successfuly requested operator permissions again on ") + string(LIGHT_MAGENTA) + string("server ") + string(GREEN) + serv.name + string(DEFAULT));
		}
		else {
			log(string(LIGHT_MAGENTA) + string("User ") + string(GREEN) + string(this->getNickName()) + string(LIGHT_BLUE) + string(" became operator of ") + string(LIGHT_MAGENTA) + string("server ") + string(GREEN) + serv.name + string(DEFAULT));
		}
		serv.userDB->addOper(*this);
		return;
	}
	else {
		throw BadPasswd();
		return;
	}
}
