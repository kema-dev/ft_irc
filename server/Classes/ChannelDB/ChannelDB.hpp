#ifndef _CHANNELDB_HPP
 #define _CHANNELDB_HPP

class Channel;
class User;
#include "../Channel/Channel.hpp"

#include <sys/types.h>
#include <iostream>

using namespace std;

class NoSuchChan : public exception
{
	public:
		virtual const char*	what() const throw()
		{
			return ("Channel doesn't exist");
		}
};

class ChannelDB {
	private:
	vector<Channel>	_db;
	string			_name;

	public:
	ChannelDB(string name) {_name = name;};
	~ChannelDB() {};

	void		add(Channel& chan);
	Channel*	search(Channel& chan);
	Channel*	search(string name);
};

#endif