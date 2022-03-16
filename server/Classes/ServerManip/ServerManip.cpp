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

int check_password(string input, Server *irc_serv, int socket)
{
    string hash;
    if (input.find("PASS") != std::string::npos)
    {
        std::string password = input.substr(strlen("PASS") + 1 , input.length() - strlen("PASS") - (input.length() - (input.find("\n") - 2)));
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
            std::cout << "PasswordIncorrect" << std::endl;
            return (-1);
        }
        input.erase(0, strlen("PASS ") + password.length() + 2); // OPTIONAL
    }
    return (0);
}

string parseNickname(string input)
{
    string nickname;

    if (input.find("NICK") == std::string::npos)
        return (NULL);
    else
    {
        nickname = input.substr(strlen("NICK") + 1 , input.length() - strlen("NICK") - (input.length() - (input.find("\n") - 2)));
        input.erase(0, strlen("NICK ") + nickname.length() + 2); // OPTIONAL
    }
    return (nickname);
}

ssize_t createUser(std::string input, Server *server, int socket, string nickname)
{
    ssize_t id;
    string fullname;
    char username[50];
    char hostname[50];
    char servername[50];
    size_t pos;
    string cmd;

    cmd = input;
    if (nickname.empty() == true)
    {
        pos = cmd.find("NICK");
        if (pos == std::string::npos)
        {
            send(socket, "Registration incomplete : Execute these command(s) first: /NICK <nickname>\r\n", strlen("Registration incomplete : Execute these command(s) first: /NICK <nickname>\r\n"), 0);
            std::cout << "Ratio le exit(-1)" << std::endl;
            exit(-1);
        }
        else
        {
            nickname = cmd.substr(strlen("NICK") + 1 , cmd.length() - strlen("NICK") - (cmd.length() - (cmd.find("\n") - 2)));
            cmd.erase(0, strlen("NICK ") + nickname.length() + 2);
            
        }
    }
    else
    {
        cmd.erase(0, strlen("USER "));
        sscanf(cmd.c_str(), "%s %s %s", username, hostname, servername);
        cmd.erase(0, cmd.length() - (cmd.length() - cmd.find(":")) + 1);
        fullname = cmd.substr(0, cmd.length() - 2);
        // cout << username << endl;
        // cout << hostname << endl;
        // cout << servername << endl;
        // cout << fullname << endl;
    }
    try {
		id = server->addUser(username, fullname, nickname, hostname, servername, server);
        cout << "User created!" << endl;
	}
	catch (exception& e) {
		logError(string("Adding user on server " + string(servername)), username, e.what());
	}
    return (id);
}
