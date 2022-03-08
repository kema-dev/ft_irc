#include "ServerManip.hpp"

using namespace std;

void createUser(std::string input, UidPool pool)
{
    string username;
    string fullname;
    string nickname;
    size_t pos;
    string cmd;
    (void)pool;

    bool nonick = false;

    cmd = input;
    pos = cmd.find("NICK");
    if (pos == std::string::npos)
        nonick = true;
    
    if (nonick == false)
    {
        nickname = cmd.substr(strlen("NICK") + 1 , cmd.length() - (cmd.length() - cmd.find('\n')) - 5);
        cmd.erase(0, strlen("NICK ") + nickname.length() + 1);
        cout << nickname << endl;
        cout << "Input = " << cmd << endl;
    }
    pos = cmd.find("USER");
    // return;
    //TODO PARSE USER 

    // if (nonick == false){
    //     User user = User(username, fullname, nickname, "user", pool);
    //     return user;
    // }
    // else {
    //     User user = User(username, fullname, "user", pool);
    //     return user;
    // }
} 