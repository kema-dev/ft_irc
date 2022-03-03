#ifndef _ERROR_HPP_
 #define _ERROR_HPP_

#include <stdexcept>
#include <string>

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

#define NOT_COMMAND -6
class NotACommand : public std::exception
{
	public:
		virtual const std::string	info() const throw()
		{
			return ("What you just typed is not a command.");
		}
};

#define UNKNOWN_COMMAND -7
class InvalidCommand : public std::exception
{
	public:
		virtual const std::string	info() const throw()
		{
			return ("The command you entered is unknown.");
		}
};

#define EMPTY_COMMAND -8
class EmptyCommand : public std::exception
{
	public:
		virtual const std::string	info() const throw()
		{
			return ("The command you entered is empty.");
		}
};

class ErrorReusingSocket : public std::exception
{
	public:
		virtual const std::string	info() const throw()
		{
			return ("Impossible to reuse socket.");
		}
};

class ReadImpossible : public std::exception
{
	public:
		virtual const std::string	info() const throw()
		{
			return ("Read impossible.");
		}
};

#endif