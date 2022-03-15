#include "../Server/Server.hpp"

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

void	Server::addChan(string name, string pass, string motd, string oper_pass) {
	Channel* chan = nullptr;
	try {
		chan = new Channel(name, pass, motd, oper_pass);
	}
	catch (exception& e) {
		logError(string("Creating channel"), name, e.what());
		throw ChanAddFail();
		return ;
	}
	this->chanDB->add(*chan);
}

string  Server::getHash()
{
    return _hash;
}

ssize_t	Server::addUser(string username, string fullname, string nickname, string hostname, string servername, Server* server) {
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
