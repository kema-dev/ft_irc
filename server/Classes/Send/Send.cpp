#include "../Send/Send.hpp"

using namespace std;

// char*	Send::gen_reply(char* format, va_list args) {
// 	char* msg = NULL;
// 	if (asprintf(&msg, format, args) < 0) {
// 		logError("asprintf", "asprintf failed", "asprintf");
// 		return NULL;
// 	}
// 	return msg;
// }

void	Send::send_to_client(string header, int socket, const char *msg) {
	string	packet = string(msg);
	packet.insert(0, header);
	send(socket, packet.c_str(), strlen(packet.c_str()), MSG_DONTWAIT);
	cerr << "Sent to " << socket << ": " << packet << endl;
}

void	Send::reply(User* sender, User* receiver, int code, int header, string format, ...) {
	int socket = receiver->getSocket();
	string	header_str = "";
	string rpl_nb_str = " ";
	switch (header)
	{
	case HEADER_SERVER:
		header_str = ":" + sender->getHostName() + " " + itos(code) + " " + sender->getNickName() + " ";
		break;
	case HEADER_CLIENT:
		if (code > 0) {
			rpl_nb_str = " " + itos(code) + " ";
		}
		header_str = ":" + sender->getNickName() + "!" + sender->getUserName() + "@" + sender->getHostName() + rpl_nb_str;
		break;
	default:
		break;
	}
	va_list	args;
	va_start(args, format);
	while (format.find("%s") != string::npos) {
		format.replace(format.find("%s"), 2, va_arg(args, char*));
	}
	this->send_to_client(header_str, socket, format.c_str());
	va_end(args);
}
