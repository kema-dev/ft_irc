#include "../Channel/Channel.hpp"
#include "../Log/Log.hpp"
#include "../ChannelDB/ChannelDB.hpp"
#include "../UserDB/UserDB.hpp"

int main(void) {
	clearLog();
	log("-------------------------------------------------------------------------------------");
	UidPool	pool = UidPool();
	UserDB usrdb = UserDB("user_database");
	User*	usr = new User("firstname", "firstfullname", "hostname", "servername", pool);
	usrdb.add(usr);
	User*	usr2 = new User("secondname", "secondfullname", "secondnickname", "hostname", "servername", pool);
	usrdb.add(usr2);
	ChannelDB*	chandb = new ChannelDB("chan_database");
	Channel*	chan = new Channel("chan", "chan_password", "motd", "oper_password");
	chandb->add(chan);
	usr->joinChannel(chan, "chan_password");
	usr2->joinChannel(chan, "chan_password");
	usr->ban(*usr2, *chan);
}
