#include "../Server/Server.hpp"

Server::Server(string dname, string pass) {
	name = dname;
	chanDB = new ChannelDB(name + "_chanDB");
	userDB = new UserDB(name + "_userDB");
	pool = new UidPool();
	_hash = sha256(pass);
}

Server::~Server() {
	delete chanDB;
	delete userDB;
	delete pool;
	_hash = "";
}
