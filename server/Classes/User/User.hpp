#ifndef _USER_HPP
 #define _USER_HPP

class User;
#include "../UidPool/UidPool.hpp"
#include "../Channel/Channel.hpp"
#include "../Crypto/Crypto.hpp"
#include <sys/types.h>
#include <iostream>

using namespace std;

class WrongUserName : public exception
{
	public:
		virtual const string	info() const throw()
		{
			return ("Username should only contain alphanumeric characters");
		}
};

class WrongRoleNameUser : public exception
{
	public:
		virtual const string	info() const throw()
		{
			return ("Role should either be 'user' or 'operator'");
		}
};

class User {
	private:
	string 		_username;
    string      _nickname;
    string      _fullname;
	string		_role;
	ssize_t		_nb_msg;
	bool		_ban_status;
	bool		_active_status;
	ssize_t		_uid;
	string		_hash;

	public:
    User(string username, string fullname, string role, UidPool& pool);
	User(string username, string fullname, string nickname ,string role, UidPool& pool);
	~User() {};
	
	string	getName(void);
	string	getRole(void);
	ssize_t	getNbMsg(void);
	bool	getBanStatus(void);
	bool	getActiveStatus(void);
	ssize_t	getUid(void);
	string	getHash(void);

	bool	setNickName(string new_name);
	bool	setRole(string new_role);
	bool	setNbMsg(ssize_t new_nb_msg);
	bool	setBanStatus(bool new_ban_status);
	bool	setActiveStatus(bool new_active_status);
	bool	setUid(ssize_t new_uid);
	bool	setHash(string new_hash);
	bool	setPass(string new_pass);

	bool	logIn(string pass);
	bool	sendMessage(string content, Channel* chan);
	bool	joinChannel(Channel* chan, string pass);
};

std::ostream &	operator<<(std::ostream &stream, User &rhs);

#endif