#ifndef _CHANNELDB_HPP
 #define _CHANNELDB_HPP
#include "../Channel/Channel.hpp"
#include "../Crypto/Crypto.hpp"
#include <sys/types.h>
#include <iostream>

using namespace std;

class ChannelDB {
	private:
	vector<Channel*>	_db;

	public:
	ChannelDB() {};
	~ChannelDB() {};

	void		add(Channel* chan);
	Channel*	search(Channel* chan);
	Channel*	search(string name);
};

#endif