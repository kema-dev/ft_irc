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
		log(e.what());
	}
	User*	usr = nullptr;
	string username1 = "username1";
	string fullname1 = "fullname1";
	try {
		usr = new User(username1, fullname1, hostname, servername, server);
	}
	catch (exception& e) {
		logError(string("User creation"), username1, e.what());
	}
	// server.userDB->add(usr);
	// User*	usr2 = new User("secondname", "secondfullname", "secondnickname", "hostname", "servername", *(server.pool));
	// server.userDB->add(usr2);
	// Channel*	chan = new Channel("chan", "chanpassword", "motd", "operpassword");
	// server.chanDB->add(chan);
	// usr->joinChannel(chan, "chanpassword");
	// usr2->joinChannel(chan, "chanpassword");
	// usr->ban(*usr2, *chan);
}
