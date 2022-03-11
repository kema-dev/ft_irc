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
        std::cerr << e.info() << '\n';
        exit(KQUEUE_ERR);
    }
    EV_SET(&event_list[0], socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
    try {
        if (kevent(kq, event_list, 1, NULL, 0, NULL) == -1)
            throw(ErrKEvent());
    }
    catch (const ErrKEvent e){
        std::cerr << e.info() << std::endl;
        exit(KEVENT_ERR);
    }
}

User& createUser(std::string input, UidPool pool, int socket)
{
    string nickname;
    string fullname;
    char username[50];
    char hostname[50];
    char servername[50];
    size_t pos;
    string cmd;
    
    bool nonick = false;

    cmd = input;
    pos = cmd.find("NICK");
    if (pos == std::string::npos)
        nonick = true;
    if (nonick == true)
    {
        send(socket, "Registration incomplete : Execute these command(s) first: /NICK <nickname>\r\n", strlen("Registration incomplete : Execute these command(s) first: /NICK <nickname>\r\n"), 0);
        std::cout << "Ratio le exit(-1)" << std::endl;
        exit(-1);
    }
    if (nonick == false)
    {
        nickname = cmd.substr(strlen("NICK") + 1 , cmd.length() - strlen("NICK") - (cmd.length() - (cmd.find("USER") - 3)));
        cmd.erase(0, strlen("NICK ") + nickname.length() + 2);
    }
    cmd.erase(0, strlen("USER "));
    sscanf(cmd.c_str(), "%s %s %s", username, hostname, servername);
    cmd.erase(0, cmd.length() - (cmd.length() - cmd.find(":")) + 1);
    fullname = cmd.substr(0, cmd.length() - 2);
    // cout << username << endl;
    // cout << hostname << endl;
    // cout << servername << endl;
    // cout << fullname << endl;
    // User* user = new User(username, fullname, nickname, hostname, servername, pool);
    // cout << "User created!" << endl;
    // return *user;
	Server*	server = nullptr;
	User* usr = new User("first_name", "firstfullname", "hostname", "servername", server);
	(void)pool;
	return *usr;
}
