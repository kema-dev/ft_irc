#ifndef _USER_HPP
 #define _USER_HPP
#include "../Classes/uid_pool.hpp"
#include "../Classes/channel.hpp"
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
	string 		_name;
	string		_role;
	ssize_t		_nb_msg;
	bool		_ban_status;
	bool		_active_status;
	ssize_t		_uid;
	string		_hash;

	public:
	User(string name, string role, string pass, UidPool pool);
	~User() {};
	
	string	getName(void);
	string	getRole(void);
	ssize_t	getNbMsg(void);
	bool	getBanStatus(void);
	bool	getActiveStatus(void);
	ssize_t	getUid(void);
	string	getHash(void);

	bool	setName(string new_name);
	bool	setRole(string new_role);
	bool	setNbMsg(ssize_t new_nb_msg);
	bool	setBanStatus(bool new_ban_status);
	bool	setActiveStatus(bool new_active_status);
	bool	setUid(ssize_t new_uid);
	bool	setHash(string new_hash);
	bool	setPass(string new_pass);

	string	md5(string pass);
	bool	log_in(string pass);
	void	sendMessage(std::string msg, Channel& chan);
};

std::ostream &	operator<<(std::ostream &stream, User &rhs);

#endif