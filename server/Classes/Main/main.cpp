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

// static int connFd;
void error(const char *msg) {
	perror(msg);
	exit(1);
}

// // ! SEND ONLY "<< overloaded" type !
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
	Server *irc_serv = NULL;
	string servername  = "IRC_SERV";
	try {
		irc_serv = new Server(servername, argv[2], listenFd);
	} catch (exception &e) {
		logError(string("Server creation"), servername, e.what());
	}
    cout << GREEN + string("Server initialized, now waiting for connections on port ") + argv[1] + DEFAULT << endl;
	log(GREEN + string("Server initialized, now waiting for connections on port ") + argv[1] + DEFAULT);
    irc_serv->start();
}

// string read_socket(int socket) {
// 	char input[256];
//     string buf;
// 	string ret;
// 	string input_s;

// 	int n = recv(socket, input, 255, MSG_DONTWAIT);
// 	try {
// 		if (n < 0)
// 			throw(ReadImpossible());
// 	} catch (const ReadImpossible e) {
// 		// cerr << e.what() << endl;
// 	}
// 	// cout << "Result from read:'" << input << "'" << endl;
// 	input_s = input;
// 	if (buf.empty() == 0)
// 		input_s = buf.append(input_s);
// 	ret = input_s.substr(0, input_s.find("\r"));
// 	buf = input_s.substr(input_s.find("\n") + 1, input_s.length() - (ret.length() + 1));
// 	return (ret);
// }

// void ratio(t_KDescriptor desc, int socket) {
// 	t_params *params = reinterpret_cast<t_params *>(dummyPt);
// 	string nickname;
// 	string input;
// 	ssize_t id;
// 	size_t nbPass = 0;
// 	int security = 0;
// 	int nbError = 0;
// 	bool loop = false;

// 	// ANCHOR xchat check
// 	// if (input_s.find("CAP") != string::npos)
// 	//     input_s.erase(0, strlen("CAP LS\n\r"));

// 	cout << "-----------------" << endl;
// 	while (!loop) {
// 		while ((input = read_socket(params->client_socket)).empty() == true) {
// 			;
// 		}
// 		cout << DARK_GRAY <<  "Input = '" << input << "'" << DEFAULT << endl;
// 		// TODO add try/catch
//         cout << "nbPass:" << nbPass << endl;
// 		if (nbPass == 0) {
// 			if (check_password(input, params->irc_serv, params->client_socket) == 0)
// 				nbPass++;
// 			continue;
// 		}
// 		if (nbPass == 1 && input.find("NICK") != string::npos) {
//             nickname = parseNickname(input);
//             nbPass++;
// 		}
//         if (nbPass == 2 && input.find("USER") != string::npos)
//         {
//             id = createUser(input, params, nickname);
//             if (id < 0)
//             {
//                 nbPass--;
//                 continue;
//             }
//             params->user_id = id;
//             try {
//                 params->irc_serv->userDB->search(id)->logIn(*params->irc_serv);
//             } catch (exception &e) {
//                 string str = static_cast<ostringstream *>(&(ostringstream() << params->user_id))->str();
//                 logError(string("Logging in server"), str, e.what());
//             }
//             nbPass++;
//         }
// 		if (nbPass == 3) {
// 			welcome_client(params, "");
// 			nbPass++;
// 			continue;
// 		}
// 		if (nbPass > 3) {
// 			try {
// 				security = command_check(input, params);
// 				if (security == CLIENT_DISCONNECTED)
// 					nbError++;
// 				if (nbError >= 5)
// 					throw(ClientDisconnected());
// 			} catch (const ClientDisconnected e) {
// 				cerr << e.what() << endl;
// 				close(params->client_socket);
// 				exit(CLIENT_DISCONNECTED);
// 			}
// 		}
// 	}
// 	cout << "\nClosing thread and connection." << endl;
// 	close(params->client_socket);
// 	return (NULL);
// }
