#include "ServerManip.hpp"

using namespace std;

void init_kqueue(int socket, int &kq)
{
    (void)socket;
    struct kevent event_list[1];
    try {
        if ((kq = kqueue()) == -1)
            throw(ErrKQueue());
    }
    catch(const ErrKQueue e) {
        std::cerr << e.what() << '\n';
        exit(KQUEUE_ERR);
    }
    EV_SET(&event_list[0], socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
    try {
        if (kevent(kq, event_list, 1, NULL, 0, NULL) == -1)
            throw(ErrKEvent());
    }
    catch (const ErrKEvent e){
        std::cerr << e.what() << std::endl;
        exit(KEVENT_ERR);
    }
}

size_t ft_find(string input)
{
    size_t pos_r = input.find("\r");
    size_t pos_n = input.find("\n");
    if (pos_r == string::npos && pos_n != string::npos)
        return (pos_n);
    else if (pos_r != string::npos && pos_n == string::npos)
        return (pos_r);
    else if (pos_r == string::npos && pos_n == string::npos)
        return (0);
    else {
        if (pos_r < pos_n)
            return(pos_r);
        else
            return(pos_n);
    }
}

int check_password(string input, Server *irc_serv, int socket)
{
    string hash;
    if (input.find("PASS") != std::string::npos)
    {
        std::string password = input.substr(strlen("PASS") + 1 , input.length() - strlen("PASS ") - (input.length() - (ft_find(input))));
        try {
            hash = sha256(password);
        }
        catch (exception& e) {
		    logError(string("Password does not match."), NULL, e.what());
		    throw ServerFail();
		    return (-1);
	    }
        if (hash != irc_serv->getHash())
        {
            send(socket, "Unable to log into the server: password incorrect\r\n", strlen("Unable to connect to the server: password incorrect\r\n"), 0);
            std::cerr << "PasswordIncorrect" << std::endl;
            return (-1);
        }
        input.erase(0, strlen("PASS ") + password.length() + 2); // OPTIONAL
    }
    else{
        return (-1);
    }
    cerr << "Password valid" << endl;
    return (0);
}

string parseNickname(string input)
{
    string nickname;

    if (input.empty())
        return (NULL);
    if (input.find("NICK") == std::string::npos)
        return (NULL);
    else
    {
        cout << ft_find(input) << endl;
        nickname = input.substr(strlen("NICK ") , input.length() - strlen("NICK ") - (input.length() - ft_find(input)));
    }    
    return (nickname);
}

ssize_t createUser(string input, Server *server, int socket, string nickname)
{
    ssize_t id;
    string fullname;
    char username[50];
    char hostname[50];
    char servername[50];
    size_t pos;
    string cmd;
    (void)socket;
    (void)pos;

    if (input.empty() || nickname.empty())
        return (-1);
    cmd = input;
    cmd.erase(0, strlen("USER "));
    sscanf(cmd.c_str(), "%s %s %s", username, hostname, servername);
    cmd.erase(0, cmd.length() - (cmd.length() - cmd.find(":")) + 1);
    fullname = cmd.substr(0, cmd.length() - 2);
    try {
		id = server->addUser(username, fullname, nickname, hostname, servername, server);
        cout << "User created!" << endl;
	}
	catch (exception& e) {
		logError(string("Adding user on server " + string(servername)), username, e.what());
        return (-1);
	}
    return (id);
}
