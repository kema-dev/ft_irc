#include <netdb.h>
#include <stdlib.h>
#include <sys/event.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string>

#include "../CommandHandler/CommandHandler.hpp"
#include "../Error/Error.hpp"
#include "../Server/Server.hpp"
#include "../ServerManip/ServerManip.hpp"

void *task1(void *);

using namespace std;

static int connFd;
void error(const char *msg) {
	perror(msg);
	exit(1);
}

// ! SEND ONLY "<< overloaded" type !
// template<typename T>
// string	itos(T nb) {
// 	return static_cast<ostringstream*>(&(ostringstream() << nb))->str();
// }

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
	int listenFd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
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
	Server *irc_serv = nullptr;
	string servername  = "IRC_SERV";
	try {
		irc_serv = new Server(servername, argv[2]);
	} catch (exception &e) {
		logError(string("Server creation"), servername, e.what());
	}
	struct kevent event_list[1];
	int kq;
	init_kqueue(listenFd, kq);
	cout << GREEN + string("Server initialized, now waiting for connections on port ") + argv[1] + DEFAULT << endl;
	log(GREEN + string("Server initialized, now waiting for connections on port ") + argv[1] + DEFAULT);

	int new_event;
	struct sockaddr_in clntAdd;
	socklen_t len = sizeof(clntAdd);
	t_params params;
	vector<pthread_t> threadV;
	while (true) {
		try {
			if ((new_event = kevent(kq, NULL, 0, event_list, 1, NULL)) == -1)
				throw(ErrKEvent());
		} catch (const ErrKEvent e) {
			logError("kevent", "FAIL", e.what());
			cerr << RED << "kevent failed: " << e.what() << DEFAULT << endl;
			exit(KEVENT_ERR);
		}
		int event_fd = event_list[0].ident;
		try {
			if ((connFd = accept(event_fd, (struct sockaddr *)&clntAdd, (socklen_t *)&len)) == -1)
				throw(CannotAcceptConnection());
			else {
				pthread_t a;
				threadV.push_back(a);
				params.client_socket = connFd;
				params.irc_serv = irc_serv;
				pthread_create(&threadV.back(), NULL, task1, &params);
				cout << "Connection established." << endl;
			}
		} catch (const CannotAcceptConnection e) {
			logError("Accepting connection", "FAIL", e.what());
			cerr << RED << "Accepting connection: " << e.what() << DEFAULT << endl;
			return (ERR_CONNECTION);
		}
	}
}

string read_socket(int socket) {
	char input[256];
	static string buf;
	string ret;
	string input_s;

	bzero(input, 256);
	int n = recv(socket, input, 255, MSG_DONTWAIT);
	try {
		if (n < 0)
			throw(ReadImpossible());
	} catch (const ReadImpossible e) {
		// cerr << e.what() << endl;
	}
	// cout << "Result from read:'" << input << "'" << endl;
	input_s = input;
	if (buf.empty() == 0)
		input_s = buf.append(input_s);
	ret = input_s.substr(0, input_s.find("\r"));
	buf = input_s.substr(input_s.find("\n") + 1, input_s.length() - (ret.length() + 1));
	return (ret);
}

void *task1(void *dummyPt) {
	t_params *params = reinterpret_cast<t_params *>(dummyPt);
	string nickname;
	string input;
	ssize_t id;
	size_t nbPass = 0;
	int security = 0;
	int nbError = 0;
	bool loop = false;

	// ANCHOR xchat check
	// if (input_s.find("CAP") != string::npos)
	//     input_s.erase(0, strlen("CAP LS\n\r"));

	cout << "-----------------" << endl;
	while (!loop) {
		while ((input = read_socket(params->client_socket)).empty() == true) {
			;
		}
		cout << "Input = '" << input << "'" << endl;
		// TODO add try/catch
		if (nbPass == 0) {
			if (check_password(input, params->irc_serv, params->client_socket) == 0)
				nbPass++;
			continue;
		}
		if ((nbPass == 1) || (nbPass == 2)) {
			if (nickname.empty() == true) {
				nickname = parseNickname(input);
				nbPass++;
			} else {
				id = createUser(input, params->irc_serv, params->client_socket, nickname);
				params->user_id = id;
				try {
					params->irc_serv->userDB->search(id)->logIn(*params->irc_serv);
				} catch (exception &e) {
					string str = static_cast<ostringstream *>(&(ostringstream() << params->user_id))->str();
					logError(string("Logging in server"), str, e.what());
				}
				nbPass++;
			}
		}
		if (nbPass == 3) {
			send(params->client_socket, ":localhost 001 dOD : Welcome to the Internet Relay Network dOD\r\n", strlen(":localhost 001 dOD : Welcome to the Internet Relay Network dOD\r\n"), 0);
			send(params->client_socket, ":localhost 002 dOD : Your host is localhost, running on version [42.42]\r\n", strlen(":localhost 002 dOD : Your host is localhost, running on version [42.42]\r\n"), 0);
			send(params->client_socket, ":localhost 003 dOD : This server was created Mon Mar 14 13:08:31 2022\r\n", strlen(":localhost 003 dOD : This server was created Mon Mar 14 13:08:31 2022\r\n"), 0);
			send(params->client_socket, ":localhost 004 dOD : localhost version [42.42]. Available user MODE : +Oa . Avalaible channel MODE : none.\r\n", strlen(":localhost 004 dOD : localhost version [42.42]. Available user MODE : +Oa . Avalaible channel MODE : none.\r\n"), 0);
			nbPass++;
			continue;
		}
		if (nbPass > 3) {
			try {
				security = command_check(input, params);
				if (security == CLIENT_DISCONNECTED)
					nbError++;
				if (nbError >= 5)
					throw(ClientDisconnected());
			} catch (const ClientDisconnected e) {
				cerr << e.what() << endl;
				close(params->client_socket);
				exit(CLIENT_DISCONNECTED);
			}
		}
	}
	cout << "\nClosing thread and connection." << endl;
	close(params->client_socket);
	return (NULL);
}
