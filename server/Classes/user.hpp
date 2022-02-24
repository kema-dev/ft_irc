#ifndef _USER_HPP
 #define _USER_HPP

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

class WrongRoleName : public exception
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
	size_t		_nb_msg;
	bool		_ban_status;
	uid_pool	_pool;
	size_t		_uid;

	public:
	User(string name, string role);
	~User();
	sendMessage(std::string msg, Channel& chan);
}

#endif