#include "../Channel/Channel.hpp"
#include "../Log/Log.hpp"
#include "../Server/Server.hpp"

int main(void) {
	clearLog();
	log("-------------------------------------------------------------------------------------");
	Server*	server = NULL;
	string hostname = "hostname";
	string servername = "servername";
	string serverpassword = "serverpassword";
	// NOTE Create only one server
	try {
		server = new Server(servername, serverpassword);
	}
	catch (exception& e) {
		logError(string("Server creation"), servername, e.what());
	}
	string username = "username";
	string fullname = "fullname";
	string nickname = "nickname";
	ssize_t id;
	try {
		id = server->addUser(username, fullname, nickname, hostname, servername, server);
	}
	catch (exception& e) {
		logError(string("Adding user on server " + servername), username, e.what());
	}
	try {
		server->userDB->search(id)->logIn(*server);
	}
	catch (exception& e) {
		logError(string("Logging in server " + servername), username, e.what());
	}
	string channame = "#channame";
	string chanpasswd = "chanpasswd";
	string topic = "topic";
	string operpassword = "operpassword";
	try {
		server->addChan(channame, chanpasswd, topic, operpassword);
	}
	catch (exception& e) {
		logError(string("Adding channel on server " + servername), channame, e.what());
	}
	try {
		server->userDB->search(id)->joinChannel(*(server->chanDB->search(channame)), chanpasswd);
	}
	catch (exception& e) {
		logError(string("Joining channel " + channame + " on server " + servername), username, e.what());
	}
	try {
		server->userDB->search(id)->becomeOper(*(server->chanDB->search(channame)), operpassword);
	}
	catch (exception& e) {
		logError(string("Becoming operator of channel " + channame), username, e.what());
	}
	// TODO send message
	// TODO send private message
	// TODO print chan messages
	// TODO count nb of messages per user
}
