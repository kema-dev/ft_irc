#ifndef _CHANNEL_HPP
 #define _CHANNEL_HPP

class User;

#include "../Crypto/Crypto.hpp"
#include "../Message/Message.hpp"
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
			return ("Channel should only contain alphanumeric characters and start with a #");
		}
};

class UserIsBanned : public exception
{
	public:
		virtual const char*	what() const throw()
		{
			return ("User is banned from channel");
		}
};

class NotInChan : public exception
{
	public:
		virtual const char*	what() const throw()
		{
			return ("User is not in channel");
		}
};

#define USER		false
#define OPERATOR	true

class Channel {
	private:
	string			_name;
	string			_hash;
	// string			_oper;
    string          _topic;
	ssize_t			_next_uid;
	vector<Message>	_hist;
	vector<pair<User&, int> >	_log;
	// vector<pair<User&, bool> >	_roles;

	public:
	// Channel(string name, string pass, string oper_pass, string topic);
	Channel(string name, string pass, string topic);
	~Channel() {};

	string	getName(void);
    string  getTopic(void);
	ssize_t	getUidAfter(timeval time);
	ssize_t	getNextUid(void);
    void    setTopic(string topic);
	bool	userJoin(User& usr, string pass);
	bool	userLeave(User& usr);
	void	receiveMsg(Message& msg);
	void	printAllMsg(void);
	int		isLog(User& usr);
	// string	getMsgHist(User& usr);
	vector<string>	getNickLst(void);
	bool	userBan(User& usr, User& banner);
	// bool	isOper(string nickname);
	bool	setPasswd(string pass);
	// bool	checkOperPasswd(string pass);
	// bool	setOperPasswd(string oper_pass);
	// bool	addOper(User& usr);
	// bool	removeOper(User& usr);
};

#endif