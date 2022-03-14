#include "../Channel/Channel.hpp"
#include "../Log/Log.hpp"
#include "../Server/Server.hpp"

int main(void) {
	clearLog();
	log("-------------------------------------------------------------------------------------");
	Server*	server = nullptr;
	string hostname = "hostname";
	string servername = "servername";
	string serverpassword = "serverpassword";
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
		logError(string("Adding user on server"), servername, e.what());
	}
	string channame = "channame";
	string chanpasswd = "chanpasswd";
	string motd = "motd";
	string operpassword = "operpassword";
	try {
		server->addChan(channame, chanpasswd, motd, operpassword);
	}
	catch (exception& e) {
		logError(string("Adding channel on server"), servername, e.what());
	}
	try {
		server->userDB->search(id)->joinChannel(*(server->chanDB->search(channame)), chanpasswd);
	}
	catch (exception& e) {
		logError(string("Joining channel on server"), servername, e.what());
	}
	// try {
	// 	usr->joinChannel(*chan, chanpasswd);
	// }
	// catch (exception& e) {
	// 	logError(string("Joining channel " + channame), username, e.what());
	// }
	// try {
	// 	usr->becomeOper(*chan, chanpasswd);
	// }
	// catch (exception& e) {
	// 	logError(string("Joining channel " + channame), username, e.what());
	// }
}
