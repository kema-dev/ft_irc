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

class ChanSearchFail : public exception
{
	public:
		virtual const char*	what() const throw()
		{
			return ("Channel search failed");
		}
};

class ChanRemoveFail : public exception
{
	public:
		virtual const char*	what() const throw()
		{
			return ("Channel remove failed");
		}
};

class DuplicateName : public exception
{
	public:
		virtual const char*	what() const throw()
		{
			return ("An existing channel already uses this name");
		}
};

class ChannelDB {
	private:
	vector<Channel>	_db;
	string			_name;

	public:
	ChannelDB(string name) {_name = name;};
	~ChannelDB() {};

	void		remove(string name);
	void		add(Channel& chan);
	Channel*	search(Channel& chan);
	Channel*	search(string name);
	void		chkDuplicate(string name);
	void		logOut(User& usr);
};

#endif