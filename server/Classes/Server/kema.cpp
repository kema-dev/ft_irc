#include "../Channel/Channel.hpp"
#include "../Log/Log.hpp"
#include "../ChannelDB/ChannelDB.hpp"

int main(void) {
	UidPool	pool = UidPool();
	User	usr = User("kema", "fullname", "operator", pool);
	ChannelDB* chandb = new ChannelDB();
	Channel*	chan = new Channel("chan", "passwordchannel", "motd");
	chandb->add(chan);
	usr.joinChannel(chan, "passwordchannel");
	usr.sendMessage("this is before", chan);
	User	after = User("after", "fullname", "operator", pool);
	after.joinChannel(chan, "passwordchannel");
	usr.sendMessage("this is after", chan);
	log("test");
	cout << "server logs:" << endl << getLog() << endl;
}
