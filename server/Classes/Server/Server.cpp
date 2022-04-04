#include "Server.hpp"
// ? Start a server

void Server::start(void) {
	_running = true;
	std::cout.setf(std::ios::unitbuf);

	int kq = kqueue();
	if (kq == -1) {
		log("Failed to create kqueue !");
		return;
	}

	struct timespec tmout = {5, 0};

	while (_running) {
		int eventSize = userDB->getDB().size() + 1;
		struct kevent mEvents[eventSize];
		struct kevent tEvents[eventSize];

		int k = 0;
		t_KDescriptor* desc = new t_KDescriptor();
		desc->server = this;
		desc->user = new User();
		desc->user->setSocket(_socket);
		desc->user->setConnectStatus(false);
		_descriptors.push_back(desc);
		EV_SET(&mEvents[k], _socket, EVFILT_READ, EV_ADD | EV_ERROR, 0, 0, _descriptors.back());
		k++;
		for (unsigned long j = 0; j < userDB->getDB().size(); j++) {
			t_KDescriptor* desc = new t_KDescriptor();
			desc->server = this;
			desc->user = &userDB->getDB()[j].first;
			_descriptors.push_back(desc);
			EV_SET(&mEvents[k], userDB->getDB()[j].first.getSocket(), EVFILT_READ, EV_ADD | EV_ERROR, 0, 0, _descriptors.back());
			k++;
		}

		int nev = kevent(kq, mEvents, eventSize, tEvents, eventSize, &tmout);

		if (nev == -1) {
			log("Error on kevent !");
			return;
		} else if (nev > 0) {
			for (int i = 0; i < nev; i++) {
				t_KDescriptor* desc = reinterpret_cast<t_KDescriptor*>(tEvents[i].udata);
				if (tEvents[i].flags & EV_EOF) {
					if (desc->user->getConnectStatus() == true) {
						close(desc->user->getSocket());
						desc->user->setConnectStatus(false);
					}
				} else if (tEvents[i].flags & EV_ERROR) {
					if (desc->user->getConnectStatus() == true) {
						close(desc->user->getSocket());
						desc->user->setConnectStatus(false);
					}
				} else {
					if (desc->user->getConnectStatus() == false) {
						desc->server->acceptConnection(desc, static_cast<int>(tEvents[i].ident));
					} else {
						desc->server->handleConnection(desc);
					}
				}
				if (!_running)
					break;
			}
		}
		for (unsigned int i = 0; i < _descriptors.size(); i++) {
			delete _descriptors[i];
		}
		_descriptors.clear();
	}
}

string Server::readSocket(int socket) {
	char input[256];

	bzero(input, 256);
	int n = recv(socket, input, 255, MSG_DONTWAIT);
	try {
		if (n < 0)
			throw(ReadImpossible());
	} catch (const ReadImpossible e) {
		// ERROR RECV
	}
	return (input);
}

// ? Handle connection
void Server::handleConnection(t_KDescriptor* desc) {
	string nickname;
	string input;
	string buf = "";
	ssize_t id;

	ServerManip* manip = new ServerManip();
	Command* command = new Command();
	input = desc->server->readSocket(desc->user->getSocket());

	vector<string> str = manip->parseInput(input);
	vector<string>::iterator it = str.begin(), end = str.end();
	while (it != end) {
		if ((*it).empty() == false) {
			cout << "-----------------" << endl;
			cout << DARK_GRAY << "Input = '" << (*it) << "'" << DEFAULT << endl;
		}
		if ((*it).find("PASS") != string::npos && desc->user->getActiveStatus() == NOT_CONNECTED) {
			if (manip->check_password((*it), desc->server, desc->user->getSocket()) != 0) {
				logError(string("Logging in server"), *it, "Bad password");
				return;
			}
		} else if ((*it).find("NICK") != string::npos && desc->user->getActiveStatus() == NOT_CONNECTED) {
			nickname = manip->parseNickname((*it));
			if (nickname.empty() == true) {
				logError(string("Logging in server"), *it, "Bad nickname");
				return;
			}
			try {
				userDB->chkNickDuplicate(nickname);
			} catch (exception& e) {
				logError(string("Creating user"), nickname, e.what());
				// throw UserAddFail();
				return;
			}
			desc->user->setNickName(nickname);
		} else if ((*it).find("USER") != string::npos && desc->user->getActiveStatus() == NOT_CONNECTED) {
			id = manip->createUser((*it), desc);
			if (id < 0) {
				logError(string("Logging in server"), *it, "Create user fail");
				return;
			}
			desc->user->setUid(id);
			try {
				desc->server->userDB->search(id)->logIn(*desc->server);
			} catch (exception& e) {
				string str = static_cast<ostringstream*>(&(ostringstream() << desc->user->getUid()))->str();
				logError(string("Logging in server"), str, e.what());
				return;
			}
		}
		if (desc->user->getActiveStatus() == WELCOME) {
			command->welcome(desc->user);
			desc->user->setActiveStatus(CONNECTED);
		}
		else if (desc->user->getActiveStatus() == CONNECTED) {
			command->select((*it), desc->user);
		}
		it++;
	}
	return;
}

// ? Accept connection on event
void Server::acceptConnection(t_KDescriptor* desc, int socket) {
	struct sockaddr_in clntAdd;
	socklen_t len = sizeof(clntAdd);
	int connFd;

	if ((connFd = accept(socket, (struct sockaddr*)&clntAdd, (socklen_t*)&len)) == -1) {
		log("Cannot accept connection");
		return;
	}
	// fcntl(connFd, F_SETFL, O_NONBLOCK);
	desc->user->setSocket(connFd);
	desc->user->setConnectStatus(true);
	addVoidUser(desc->user);

	cout << "Connection established." << endl;
}

// ? Create a server
Server::Server(string dname, string pass, int socket) {
	try {
		chanDB = new ChannelDB(dname + "_chanDB");
	} catch (exception& e) {
		logError(string("Allocating ChannelDB for server"), dname, e.what());
		throw ServerFail();
		return;
	}
	try {
		userDB = new UserDB(dname + "_userDB");
	} catch (exception& e) {
		delete chanDB;
		logError(string("Allocating UserDB for server"), dname, e.what());
		throw ServerFail();
		return;
	}
	try {
		pool = new UidPool(dname + "_uidPool");
	} catch (exception& e) {
		delete chanDB;
		delete userDB;
		logError(string("Allocating UidPool for server"), dname, e.what());
		throw ServerFail();
		return;
	}
	try {
		_hash = sha256(pass);
	} catch (exception& e) {
		delete chanDB;
		delete userDB;
		delete pool;
		logError(string("Hashing password for server"), dname, e.what());
		throw ServerFail();
		return;
	}
	_socket = socket;
	name = dname;
}

Server::~Server() {
	delete chanDB;
	delete userDB;
	delete pool;
	_hash = "";
}

// ? Add a channel to <this>
void Server::addChan(string name, string pass, string topic) {
	try {
		chanDB->chkDuplicate(name);
	} catch (exception& e) {
		logError(string("Creating channel"), name, e.what());
		throw ChanAddFail();
		return;
	}
	Channel* chan = NULL;
	try {
		chan = new Channel(name, pass, topic);
	} catch (exception& e) {
		logError(string("Creating channel"), name, e.what());
		throw ChanAddFail();
		return;
	}
	this->chanDB->add(*chan);
}

// ? Get <this> socket
int Server::getSocket(void) {
	return _socket;
}

void Server::setSocket(int socket) {
	_socket = socket;
}

// ? Get <this> password's hash
string Server::getHash() {
	return _hash;
}

void Server::addVoidUser(User* user) {
	this->userDB->add(*user);
	return;
}

// ? Add a user to <this>
ssize_t Server::addUser(t_KDescriptor* desc, string username, string fullname, string hostname, string servername) {
	try {
		userDB->chkDuplicate(username, fullname);
	} catch (exception& e) {
		logError(string("Creating user"), username, e.what());
		throw UserAddFail();
		return -1;
	}
	try {
		desc->user->setServer(desc->server);
		desc->user->setUserName(username);
		desc->user->setFullName(fullname);
		desc->user->setHostName(hostname);
		desc->user->setServerName(servername);
		desc->user->setNbMsg(0);
		desc->user->setActiveStatus(false);
		desc->user->setUid(desc->server->pool->generate());
	} catch (exception& e) {
		logError(string("Creating user"), username, e.what());
		throw UserAddFail();
		return -1;
	}
	return desc->user->getUid();
}

void Server::setPasswd(string pass) {
	_hash = sha256(pass);
}