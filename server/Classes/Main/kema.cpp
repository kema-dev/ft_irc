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
	User*	usr = nullptr;
	string username = "username";
	string fullname = "fullname";
	try {
		usr = new User(username, fullname, hostname, servername, server);
	}
	catch (exception& e) {
		logError(string("User creation"), username, e.what());
	}
	try {
		server->userDB->add(*usr);
	}
	catch (exception& e) {
		logError(string("Adding user to DB"), username, e.what());
	}
	Channel*	chan = nullptr;
	string channame = "channame";
	string chanpasswd = "chanpasswd";
	string motd = "motd";
	string operpassword = "operpassword";
	try {
		chan = new Channel(channame, chanpasswd, motd, operpassword);
		server->userDB->add(*usr);
	}
	catch (exception& e) {
		logError(string("Creating channel"), username, e.what());
	}
	try {
		server->chanDB->add(*chan);
	}
	catch (exception& e) {
		logError(string("Adding channel to DB"), username, e.what());
	}
	try {
		usr->joinChannel(*chan, chanpasswd);
	}
	catch (exception& e) {
		logError(string("Joining channel " + channame), username, e.what());
	}
	try {
		usr->becomeOper(*chan, chanpasswd);
	}
	catch (exception& e) {
		logError(string("Joining channel " + channame), username, e.what());
	}
}
