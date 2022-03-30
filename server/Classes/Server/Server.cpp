#include "../Server/Server.hpp"
// ? Start a server

void Server::start( void )
{
    _running = true;
    std::cout.setf(std::ios::unitbuf);

    int kq = kqueue();
    if (kq == -1)
    {
        log("Failed to create kqueue !");
        return ; 
    }

    struct timespec tmout = { 5, 0};

    while (_running)
    {
        int eventSize = userDB->getDB().size() + 1;
        struct kevent mEvents[eventSize];
        struct kevent tEvents[eventSize];

        int k = 0;
        t_KDescriptor* desc = new t_KDescriptor();
        desc->server = this;
        desc->user = new User();
        desc->connected = false;
        _descriptors.push_back(desc);
        EV_SET(&mEvents[k], _socket, EVFILT_READ, EV_ADD | EV_ERROR, 0, 0, _descriptors.back());
        k++;
        // cout << userDB->getDB().size() << endl;
        // cout << _socket << endl;
        for (unsigned long j = 0; j < userDB->getDB().size(); j++)
        {
            t_KDescriptor* desc = new t_KDescriptor();
            desc->server = this;
            desc->user = new User();
            desc->connected = false;
            _descriptors.push_back(desc);
            EV_SET(&mEvents[k], userDB->getDB()[j].first.getSocket(), EVFILT_READ, EV_ADD | EV_ERROR, 0, 0, _descriptors.back());
            k++;
        }

        int nev = kevent(kq, mEvents, eventSize, tEvents, eventSize, &tmout);

        if (nev == -1)
        {
            log("Error on kevent !");
            return ;
        }
        else if (nev > 0)
        {
            // cout << nev << endl;
            for (int i = 0; i < nev; i++)
            {
                t_KDescriptor* desc = reinterpret_cast<t_KDescriptor*>(tEvents[i].udata);
                if (tEvents[i].flags & EV_EOF)
                {
                    if (desc->connected == true)
                    {
                        close(desc->user->getSocket()); 
                        desc->connected = false;
                    }
                }
                else if (tEvents[i].flags & EV_ERROR)
                {
                    if (desc->connected == true)
                    {
                        close(desc->user->getSocket()); 
                        desc->connected = false;
                    }
                }
                else
                {
                    if (desc->connected == false)
                    {
                        cout << static_cast<int>(tEvents[i].ident) << endl;
                        desc->server->acceptConnection(desc, static_cast<int>(tEvents[i].ident));
                        desc->server->handleConnection(desc, static_cast<int>(tEvents[i].ident));
                    }
                    else
                    {
                        cout << " PASS " << endl;
                        desc->server->handleConnection(desc, tEvents[i].data);
                    }
                }
                if (!_running)
                    break;
            }
        }
        for(unsigned int i = 0; i < _descriptors.size(); i++)
        {
            delete _descriptors[i];
        }
        _descriptors.clear();
    }
}

string Server::readSocket(int socket)
{
    char input[256];
	static string buf;
	string ret;
	string input_s;

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

// ? Handle connection
void Server::handleConnection(t_KDescriptor *desc, int socket) {
	t_params *params = new t_params();
	string nickname;
	string input;
	ssize_t id;
	size_t nbPass = 0;
	int security = 0;
	int nbError = 0;

    params->client_socket = socket;
    params->irc_serv = desc->server;

	cout << "-----------------" << endl;
    while ((input = desc->server->readSocket(params->client_socket)).empty() == true) {
        ;
    }
    
    
    
    
    
    cout << DARK_GRAY <<  "Input = '" << input << "'" << DEFAULT << endl;
    // TODO add try/catch
    if (nbPass == 0) {
        if (check_password(input, params->irc_serv, params->client_socket) == 0)
            nbPass++;
    }
    if (nbPass == 1 && input.find("NICK") != string::npos) {
        nickname = parseNickname(input);
        nbPass++;
    }
    if (nbPass == 2 && input.find("USER") != string::npos)
    {
        id = createUser(input, params, nickname);
        params->user_id = id;
        try {
            params->irc_serv->userDB->search(id)->logIn(*params->irc_serv);
        } catch (exception &e) {
            string str = static_cast<ostringstream *>(&(ostringstream() << params->user_id))->str();
            logError(string("Logging in server"), str, e.what());
        }
        nbPass++;
    }
    if (nbPass == 3) {
        welcome_client(params, "");
        nbPass++;
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
	return ;
}


// ? Accept connection on event
void Server::acceptConnection(t_KDescriptor *desc, int socket)
{
    struct sockaddr_in clntAdd;
	socklen_t len = sizeof(clntAdd);
    (void)socket;
    int connFd;

    if ((connFd = accept(socket, (struct sockaddr *)&clntAdd, (socklen_t *)&len)) == -1)
    {
		log("Cannot accept connection");
        return;
    }
    desc->user->setSocket(connFd);
    desc->connected = true;
    addVoidUser();

    cout << "Connection established." << endl;
}

// ? Create a server
Server::Server(string dname, string pass, int socket) {
	try {
		chanDB = new ChannelDB(dname + "_chanDB");
	}
	catch (exception& e) {
		logError(string("Allocating ChannelDB for server"), dname, e.what());
		throw ServerFail();
		return ;
	}
	try {
		userDB = new UserDB(dname + "_userDB");
	}
	catch (exception& e) {
		delete chanDB;
		logError(string("Allocating UserDB for server"), dname, e.what());
		throw ServerFail();
		return ;
	}
	try {
		pool = new UidPool(dname + "_uidPool");
	}
	catch (exception& e) {
		delete chanDB;
		delete userDB;
		logError(string("Allocating UidPool for server"), dname, e.what());
		throw ServerFail();
		return ;
	}
	try {
		_hash = sha256(pass);
	}
	catch (exception& e) {
		delete chanDB;
		delete userDB;
		delete pool;
		logError(string("Hashing password for server"), dname, e.what());
		throw ServerFail();
		return ;
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
void	Server::addChan(string name, string pass, string topic) {
	try {
		chanDB->chkDuplicate(name);
	}
	catch (exception& e) {
		logError(string("Creating channel"), name, e.what());
		throw ChanAddFail();
		return ;
	}
	Channel* chan = nullptr;
	try {
		chan = new Channel(name, pass, topic);
	}
	catch (exception& e) {
		logError(string("Creating channel"), name, e.what());
		throw ChanAddFail();
		return ;
	}
	this->chanDB->add(*chan);
}

// ? Get <this> socket
int Server::getSocket( void ) {
    return _socket;
}

void Server::setSocket(int socket) {
    _socket = socket;
}

// ? Get <this> password's hash
string  Server::getHash()
{
    return _hash;
}

void Server::addVoidUser( void )
{
    User* usr = NULL;
    try {
		usr = new User();
	}
	catch (exception& e) {
		logError("Creating User", "", e.what());
		throw UserAddFail();
		return ;
	}
    this->userDB->add(*usr);
	return ;
}

// ? Add a user to <this>
ssize_t	Server::addUser(string username, string fullname, string nickname, string hostname, string servername, Server* server, int socket) {
	try {
		userDB->chkDuplicate(username, fullname, nickname);
	}
	catch (exception& e) {
		logError(string("Creating user"), username, e.what());
		throw UserAddFail();
		return -1;
	}
	User* usr = NULL;
	try {
		usr = new User(username, fullname, nickname, hostname, servername, server, socket);
	}
	catch (exception& e) {
		logError(string("Creating user"), username, e.what());
		throw UserAddFail();
		return -1;
	}
	return this->userDB->add(*usr);
}

void	Server::setPasswd(string pass) {
	_hash = sha256(pass);
}