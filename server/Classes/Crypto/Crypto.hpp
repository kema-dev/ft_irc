#ifndef _CRYPTO_HPP
 #define _CRYPTO_HPP

#include <string>
#include <iostream>

using namespace std;

class PopopenFail : public exception
{
	public:
		virtual const string	info() const throw()
		{
			return ("popopen() failed.");
		}
};

string	sha256(string pass);

#endif
