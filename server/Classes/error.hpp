#ifndef _ERROR_HPP_
 #define _ERROR_HPP_

#include <stdexcept>

class BadNumberArgs : public std::exception
{
	public:
		virtual const std::string	info() const throw()
		{
			return ("Usage: ./irc_server <port> <password>");
		}
};

#endif