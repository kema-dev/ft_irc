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

#define ERR_SOCKET_CREATION -3
class ErrorInSocketCreation : public std::exception
{
	public:
		virtual const std::string	info() const throw()
		{
			return ("Cannot open socket.");
		}
};

#define ERR_BIND -4
class ErrorInBinding : public std::exception
{
	public:
		virtual const std::string	info() const throw()
		{
			return ("Cannot bind the socket.");
		}
};

#define ERR_CONNECTION -5
class CannotAcceptConnection : public std::exception
{
	public:
		virtual const std::string	info() const throw()
		{
			return ("Connection cannot be established.");
		}
};

#endif