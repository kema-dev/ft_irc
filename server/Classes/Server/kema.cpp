#include "../Channel/Channel.hpp"
#include "../Log/Log.hpp"
#include "../ChannelDB/ChannelDB.hpp"
#include "../UserDB/UserDB.hpp"

int main(void) {
	clearLog();
	log("-------------------------------------------------------------------------------------");
	UidPool	pool = UidPool();
	UserDB usrdb = UserDB("user_database");
	User*	usr = new User("firstname", "firstfullname", "operator", pool);
	usrdb.add(usr);
	User*	usr2 = new User("secondname", "secondfullname", "secondnickname", "operator", pool);
	usrdb.add(usr2);
	ChannelDB*	chandb = new ChannelDB("chan_database");
	Channel*	chan = new Channel("chan", "", "motd");
	chandb->add(chan);
	usr->joinChannel(chan, "");
	usr2->joinChannel(chan, "");
	usr->ban(*usr2, *chan);
}
