#include "../Server/Server.hpp"

Server::Server(string dname, string pass) {
	name = dname;
	// try {
	// 	chanDB = new ChannelDB(name + "_chanDB");
	// }
	// catch (exception& e) {
	// 	throw NoMem();
	// }
	// try {
	// 	userDB = new UserDB(name + "_userDB");
	// }
	// catch (exception& e) {
	// 	delete chanDB;
	// 	throw NoMem();
	// }
	// try {
	// 	pool = new UidPool();
	// }
	// catch (exception& e) {
	// 	delete chanDB;
	// 	delete userDB;
	// 	throw NoMem();
	// }
	chanDB = ChannelDB(name + "_chanDB");
	userDB = UserDB(name + "_userDB");
	pool = UidPool(name + "_uidPool");
	_hash = sha256(pass);
}

Server::~Server() {
	// try {
	// 	delete chanDB;
	// }
	// catch (exception& e) {
	// 	throw DeleteFail();
	// }
	// try {
	// 	delete userDB;
	// }
	// catch (exception& e) {
	// 	throw DeleteFail();
	// }
	// try {
	// 	delete pool;
	// }
	// catch (exception& e) {
	// 	throw DeleteFail();
	// }
	_hash = "";
}
