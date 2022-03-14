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

class UserDB {
	private:
	vector<User>	_db;
	string			_name;

	public:
	UserDB(string name) {_name = name;};
	~UserDB() {};

	ssize_t	add(User& usr);
	User*	search(User& usr);
	User*	search(ssize_t id);
	bool	check(string username, string fullname, string nickname);
};

#endif
