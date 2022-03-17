#include "../Server/Server.hpp"

// ? Create a server
Server::Server(string dname, string pass) {
	try {
		chanDB = new ChannelDB(dname + "_chanDB");
	}
	catch (exception& e) {
		logError(string("Allocating ChannelDB for server"), dname, e.what());
		throw ServerFail();
		return ;
	}
	try {
		userDB = new UserDB(dname + "_userDB");
	}
	catch (exception& e) {
		delete chanDB;
		logError(string("Allocating UserDB for server"), dname, e.what());
		throw ServerFail();
		return ;
	}
	try {
		pool = new UidPool(dname + "_uidPool");
	}
	catch (exception& e) {
		delete chanDB;
		delete userDB;
		logError(string("Allocating UidPool for server"), dname, e.what());
		throw ServerFail();
		return ;
	}
	try {
		_hash = sha256(pass);
	}
	catch (exception& e) {
		delete chanDB;
		delete userDB;
		delete pool;
		logError(string("Hashing password for server"), dname, e.what());
		throw ServerFail();
		return ;
	}
	name = dname;
}

Server::~Server() {
	delete chanDB;
	delete userDB;
	delete pool;
	_hash = "";
}

// ? Add a channel to <this>
void	Server::addChan(string name, string pass, string topic, string oper_pass) {
	try {
		chanDB->chkDuplicate(name);
	}
	catch (exception& e) {
		logError(string("Creating channel"), name, e.what());
		throw ChanAddFail();
		return ;
	}
	Channel* chan = nullptr;
	try {
		chan = new Channel(name, pass, topic, oper_pass);
	}
	catch (exception& e) {
		logError(string("Creating channel"), name, e.what());
		throw ChanAddFail();
		return ;
	}
	this->chanDB->add(*chan);
}

// ? Get <this> password's hash
string  Server::getHash()
{
    return _hash;
}

// ? Add a user to <this>
ssize_t	Server::addUser(string username, string fullname, string nickname, string hostname, string servername, Server* server) {
	try {
		userDB->chkDuplicate(username, fullname, nickname);
	}
	catch (exception& e) {
		logError(string("Creating user"), username, e.what());
		throw UserAddFail();
		return -1;
	}
	User* usr = nullptr;
	try {
		usr = new User(username, fullname, nickname, hostname, servername, server);
	}
	catch (exception& e) {
		logError(string("Creating user"), username, e.what());
		throw UserAddFail();
		return -1;
	}
	return this->userDB->add(*usr);
}
