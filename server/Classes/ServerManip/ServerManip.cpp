#include "ServerManip.hpp"

using namespace std;

User& createUser(std::string input, UidPool pool, int socket)
{
    string nickname;
    char username[50];
    char fullname[50];
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
        // TODO Handle new nickname input
        exit(-1);
    }
    if (nonick == false)
    {
        nickname = cmd.substr(strlen("NICK") + 1 , cmd.length() - strlen("NICK") - (cmd.length() - (cmd.find("USER") - 3)));
        cmd.erase(0, strlen("NICK ") + nickname.length() + 2);
        cout << "'" << nickname << "'" << endl;
    }
    cmd.erase(0, strlen("USER "));
    sscanf(cmd.c_str(), "%s %s %s :%s", username, hostname, servername, fullname);

    cout << username << endl;
    cout << hostname << endl;
    cout << servername << endl;
    cout << fullname << endl;

    User* user = new User(username, fullname, nickname, hostname, servername, "user", pool);
    return *user;
}
