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
	size_t		_nb_msg;
	bool		_ban_status;
	UidPool		_pool;
	size_t		_uid;

	public:
	User(string name, string role);
	~User();
	void sendMessage(std::string msg, Channel& chan);
};

#endif