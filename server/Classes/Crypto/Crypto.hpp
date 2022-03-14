#ifndef _CRYPTO_HPP
 #define _CRYPTO_HPP

#include <string>
#include <iostream>

using namespace std;

class NotAlnum : public exception
{
	public:
		virtual const char*	what() const throw()
		{
			return ("Can't hash non alphanumeric characters");
		}
};

class PopopenFail : public exception
{
	public:
		virtual const char*	what() const throw()
		{
			return ("Function 'popopen' failed");
		}
};

string	sha256(string pass);

#endif
