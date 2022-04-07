#include "../Send/Send.hpp"

using namespace std;

char*	Send::gen_reply(char* format, ...) {
	char* msg = NULL;
	va_list args;
	va_start(args, format);
	if (asprintf(&msg, format, args) < 0) {
		logError("asprintf", "asprintf failed", "asprintf");
		return NULL;
	}
	va_end(args);
	return msg;
}

void	Send::send_to_client(string header, int socket, char *msg) {
	string	packet = string(msg);
	packet.insert(0, header + " ");
	send(socket, packet.c_str(), strlen(packet.c_str()), MSG_DONTWAIT);
}

void	Send::reply(User* usr, int code, int header, string format, ...) {
	int		socket = usr->getSocket();
	va_list	args;
	va_start(args, format);
	string	header_str = "";
	string rpl_nb_str = " ";
	switch (header)
	{
	case HEADER_SERVER:
		header_str = ":" + usr->getHostName() + " " + itos(code) + " " + usr->getNickName() + " ";
		break;
	case HEADER_CLIENT:
		if (code > 0) {
			rpl_nb_str = " " + itos(code) + " ";
		}
		header_str = ":" + usr->getNickName() + "!" + usr->getUserName() + "@" + usr->getHostName() + rpl_nb_str;
		break;
	default:
		break;
	}
	printf("%p\n", format.c_str());
	printf("%s\n", va_arg(args, char*));
	printf("%s\n", va_arg(args, char*));
	printf("\n");
	char*	msg = gen_reply(const_cast<char*>(format.c_str()), args);
	this->send_to_client(header_str, socket, msg);
	va_end(args);
}
