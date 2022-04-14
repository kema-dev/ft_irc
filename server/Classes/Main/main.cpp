#include <netdb.h>
#include <stdlib.h>
#include <sys/event.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string>

// #include "../CommandHandler/CommandHandler.hpp"
#include "../Command/Command/Command.hpp"
#include "../Error/Error.hpp"
#include "../Server/Server.hpp"
#include "../ServerManip/ServerManip.hpp"
#include "../Utils/Utils.hpp"

void *task1(void *);

using namespace std;

// static int connFd;
void error(const char *msg) {
	perror(msg);
	exit(1);
}

int check_params(int argc, char **argv) {
	log("-------------------------------------------------------------------------------------");
	try {
		if (argc != 3)
			throw(BadNumberArgs());
	} catch (BadNumberArgs &e) {
		string str = itos(argc);
		logError("Checking argc", str, e.what());
		cerr << RED << "Bad number of arguments: " << str << ": " << e.what() << DEFAULT << endl;
		exit(BAD_NUMBER_ARGS);
	}
	int portNo = atoi(argv[1]);
	try {
		if (portNo < 1 || portNo > 65535)
			throw(InvalidPort());
	} catch (InvalidPort &e) {
		logError("Checking port", argv[1], e.what());
		cerr << RED << "Invalid port: " << portNo << ": " << e.what() << DEFAULT << endl;
		exit(INVALID_PORT);
	}
	return portNo;
}

int create_socket(void) {
	int listenFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	try {
		if (listenFd < 0)
			throw(ErrorInSocketCreation());
	} catch (ErrorInSocketCreation &e) {
		logError("Creating socket", "FAIL", e.what());
		cerr << RED << "Socket creation failed: " << e.what() << DEFAULT << endl;
		exit(ERR_SOCKET_CREATION);
	}
	try {
		int enable = 1;
		if (setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
			throw(ErrorReusingSocket());
	} catch (ErrorReusingSocket &e) {
		logError("setsockopt", "FAIL", e.what());
		cerr << RED << "setsockopt failed: " << e.what() << DEFAULT << endl;
		exit(ERR_SOCKET_CREATION);
	}
	return listenFd;
}

void bind_listen(int listenFd, int portNo) {
	struct sockaddr_in svrAdd;
	bzero((char *)&svrAdd, sizeof(svrAdd));
	svrAdd.sin_family = AF_INET;
	svrAdd.sin_addr.s_addr = INADDR_ANY;
	svrAdd.sin_port = htons(portNo);
	try {
		if (bind(listenFd, (struct sockaddr *)&svrAdd, sizeof(svrAdd)) < 0)
			throw(ErrorInBinding());
	} catch (const ErrorInBinding e) {
		logError("binding socket", "FAIL", e.what());
		cerr << RED << "binding socket failed: " << e.what() << DEFAULT << endl;
		exit(ERR_BIND);
	}
	listen(listenFd, 5);
}

int main(int argc, char **argv) {
	int portNo = check_params(argc, argv);
	int listenFd = create_socket();
	bind_listen(listenFd, portNo);

	// NOTE Create only one server
	Server *irc_serv = NULL;
	string servername = "IRC_SERV";
	clearLog();
	try {
		irc_serv = new Server(servername, argv[2], listenFd);
	} catch (exception &e) {
		logError(string("Server creation"), servername, e.what());
	}
	cout << GREEN + string("Server initialized, now waiting for connections on port ") + argv[1] + DEFAULT << endl;
	log(GREEN + string("Server initialized, now waiting for connections on port ") + argv[1] + DEFAULT);
	irc_serv->start();
	delete irc_serv;
}
