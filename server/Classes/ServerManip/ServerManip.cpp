#include "ServerManip.hpp"

using namespace std;

ServerManip::ServerManip()
{
}
 
ServerManip::~ServerManip()
{
}

void    ServerManip::setInput(string input){
    _input = input;
}
void    ServerManip::setBuf(string buf){
    _buf = buf;
}

string  ServerManip::getInput(void){
    return _input;
}
string  ServerManip::getBuf(void){
    return _buf;
}

size_t ServerManip::ft_find(string input)
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

int ServerManip::check_password(string input, Server *irc_serv, int socket)
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
    // cerr << "Password valid" << endl;
    return (0);
}

string ServerManip::parseNickname(string input)
{
    string nickname;

    if (input.empty())
        return (NULL);
    if (input.find("NICK") == std::string::npos)
        return (NULL);
    else
        nickname = input.substr(strlen("NICK ") , input.length() - strlen("NICK ") - (input.length() - ft_find(input)));  
    return (nickname);
}

ssize_t ServerManip::createUser(string input, t_params *params, string nickname, t_KDescriptor *desc)
{
    ssize_t id;
    string fullname;
    char username[25];
    char hostname[25];
    char servername[25];
    string cmd;


    if (input.empty() || nickname.empty())
        return (-1);
    cmd = input;
    cmd.erase(0, strlen("USER "));
    sscanf(cmd.c_str(), "%25s %25s %25s", username, hostname, servername);
    cmd.erase(0, cmd.length() - (cmd.length() - cmd.find(":")) + 1);
    if (cmd.find(' ') == string::npos)
        return (-1);
    fullname = cmd.substr(0, cmd.length() - 2);
    try {
		id = params->irc_serv->addUser(desc, username, fullname, nickname, hostname, servername);
        // cout << "User created!" << endl;
	}
	catch (exception& e) {
		logError(string("Adding user on server " + string(servername)), username, e.what());
        return (-1);
	}
    return (id);
}

string  ServerManip::parseInput(string input)
{
    string ret;

    //TODO 
    
    return ret; 
}
