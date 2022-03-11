#ifndef _CHANNEL_HPP
 #define _CHANNEL_HPP

class Channel;
#include "../Message/Message.hpp"
#include "../Crypto/Crypto.hpp"
#include "../User/User.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <list>
#include <sys/time.h>

using namespace std;

class WrongChannelName : public exception
{
	public:
		virtual const char*	what() const throw()
		{
			return ("Channel should only contain alphanumeric characters");
		}
};

#define USER		false
#define OPERATOR	true

class Channel {
	private:
	string			_name;
	string			_hash;
	string			_oper;
	ssize_t			_next_uid;
	vector<Message>	_hist;
	vector<pair<User&, timeval> >	_log;
	vector<pair<User&, bool> >	_roles;

	public:
	Channel(string name, string pass, string motd, string oper_pass);
	~Channel() {};

	string	getName(void);
	ssize_t	getUidAfter(timeval time);
	ssize_t	getNextUid(void);
	bool	userJoin(User& usr, string pass);
	bool	userLeave(User& usr);
	void	receiveMsg(Message msg);
	void	printAllMsg(void);
	bool	isLog(User usr);
	string	getMsgHist(User usr);
	vector<string>	getNickLst(void);
	bool	userBan(User& usr, User& banner);
	bool	isOper(User usr);
	bool	setPasswd(string pass);
	bool	setOperPasswd(string oper_pass);
};

#endif