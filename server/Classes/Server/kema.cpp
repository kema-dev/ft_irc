#include "../Channel/Channel.hpp"
#include "../ChannelDB/ChannelDB.hpp"

int main(void) {
	UidPool	pool = UidPool();
	User	usr = User("kema", "operator", "passworduser", pool);
	ChannelDB* chandb = new ChannelDB();
	Channel*	chan = new Channel("chan", "passwordchannel", "motd");
	chandb->add(chan);
	usr.joinChannel(chan, "passwordchannel");
	usr.sendMessage("this is before", chan);
	User	after = User("after", "operator", "passworduser", pool);
	after.joinChannel(chan, "passwordchannel");
	usr.sendMessage("this is after", chan);
	cout << chandb->search(chan)->getMsgHist(after);
	cout << endl;
	cout << chan->getMsgHist(after);
}
