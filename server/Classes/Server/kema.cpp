#include "../Channel/Channel.hpp"
#include "../Log/Log.hpp"
#include "../ChannelDB/ChannelDB.hpp"

int main(void) {
	clearLog();
	UidPool	pool = UidPool();
	User	usr = User("firstname", "firstfullname", "operator", pool);
	User	usr2 = User("secondname", "secondfullname", "secondnickname", "operator", pool);
	ChannelDB*	chandb = new ChannelDB();
	Channel*	chan = new Channel("chan", "passwordchannel", "motd");
	chandb->add(chan);
	usr.joinChannel(chan, "passwordchannel");
	usr2.joinChannel(chan, "passwordchannel");
	usr.ban(usr2, *chan);
}
