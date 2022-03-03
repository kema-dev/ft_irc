#ifndef _CHANNEL_HPP
 #define _CHANNEL_HPP

class Channel;
#include "../Message/Message.hpp"
#include "../Crypto/Crypto.hpp"
#include "../User/User.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <sys/time.h>

using namespace std;

class WrongChannelName : public exception
{
	public:
		virtual const string	info() const throw()
		{
			return ("Channel should only contain alphanumeric characters");
		}
};

class Channel {
	private:
	string			_name;
	string			_hash;
	ssize_t			_next_uid;
	vector<Message>	_hist;
	vector<pair<ssize_t, timeval> >	_log;

	public:
	Channel(string name, string pass, string motd);
	~Channel() {};

	string	getName(void);
	ssize_t	getUidAfter(timeval time);
	ssize_t	getNextUid(void);
	bool	userJoin(User usr, string pass);
	bool	userLeave(User usr);
	void	receiveMsg(Message msg);
	void	printAllMsg(void);
	bool	isLog(User usr);
	string	getMsgHist(User usr);
};

#endif