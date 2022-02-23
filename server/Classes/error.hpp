#ifndef _ERROR_HPP_
 #define _ERROR_HPP_

#include <stdexcept>

#define BAD_NUMBER_ARGS -1
class BadNumberArgs : public std::exception
{
	public:
		virtual const std::string	info() const throw()
		{
			return ("Usage: ./irc_server <port> <password>");
		}
};

#define INVALID_PORT -2
class InvalidPort : public std::exception
{
	public:
		virtual const std::string	info() const throw()
		{
			return ("Please enter a port between 1 and 65535.");
		}
};

#endif