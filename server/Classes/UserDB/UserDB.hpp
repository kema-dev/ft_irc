#ifndef _USERDB_HPP
 #define _USERDB_HPP

class UserDB;

#include "../User/User.hpp"

#include <sys/types.h>
#include <iostream>

using namespace std;

class NoSuchUser : public exception
{
	public:
		virtual const char*	what() const throw()
		{
			return ("User doesn't exist");
		}
};

class DuplicateUsername : public exception
{
	public:
		virtual const char*	what() const throw()
		{
			return ("An existing user already uses this username");
		}
};

class DuplicateFullname : public exception
{
	public:
		virtual const char*	what() const throw()
		{
			return ("An existing user already uses this fullname");
		}
};

class DuplicateNickname : public exception
{
	public:
		virtual const char*	what() const throw()
		{
			return ("An existing user already uses this nickname");
		}
};

// ? operator password
#define OPER_PASS "password"

class   UserDB {
	private:
	vector<pair<User&, bool> >	_db;
	string			_oper;
	string			_name;

	public:
	UserDB(string name) {_name = name; _oper = sha256(OPER_PASS);};
	~UserDB() {};

	ssize_t	add(User& usr);
	User*	search(User& usr);
	User*	search(ssize_t id);
    User*	search(string nickname);
	void	chkDuplicate(string username, string fullname, string nickname);
	// void	remove(string name);
	bool	isOper(string nickname);
	bool	checkOperPasswd(string pass);
	bool	setOperPasswd(string oper_pass);
	bool	addOper(User& usr);
	bool	removeOper(User& usr);
    vector<pair<User&, bool> > getDB();

};

#endif
