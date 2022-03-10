#include "../Channel/Channel.hpp"
#include "../Log/Log.hpp"
#include "../Server/Server.hpp"

int main(void) {
	clearLog();
	log("-------------------------------------------------------------------------------------");
	Server	server = Server("servername", "serverpassword");
	User*	usr = nullptr;
	try {
		usr = new User("first_name", "firstfullname", "hostname", "servername", *(server.pool));
	}
	catch (exception& e) {
		cerr << e.what() << endl;
		logError("user creation", e.what());
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
