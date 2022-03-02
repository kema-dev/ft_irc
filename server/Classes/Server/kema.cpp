#include "../Channel/Channel.hpp"

int main(void) {
	UidPool	pool = UidPool();
	User	usr = User("kema", "operator", "passworduser", pool);
	Channel	chan = Channel("chan", "passwordchannel", "motd");
	usr.joinChannel(chan, "passwordchannel");
	usr.sendMessage("this is before", chan);
	User	after = User("after", "operator", "passworduser", pool);
	after.joinChannel(chan, "passwordchannel");
	usr.sendMessage("this is after", chan);
	cout << chan.getMsgHist(after);
}
