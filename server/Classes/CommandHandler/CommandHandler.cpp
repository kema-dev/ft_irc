#include "CommandHandler.hpp"

using namespace std;

int command_check(std::string message, t_params *params)
{
    try{
        if (message.empty() == true)
            throw(EmptyCommand());
    }
    catch (const EmptyCommand e) {
        std::cerr << e.info() << std::endl;
		return (CLIENT_DISCONNECTED);
    }
    std::vector<std::string> commands;
    commands.push_back("JOIN");
    commands.push_back("PART");
    commands.push_back("QUIT");
    commands.push_back("NICK");
    commands.push_back("USER");
    commands.push_back("PONG");
    try {
        size_t pos;
        std::string cmd;

        std::string channel_s;
        
        cmd = message;
        pos = cmd.find(' ');
        if (pos == std::string::npos)
            throw(InvalidCommand());
        cmd.erase(pos, cmd.length() - pos);
    
        int index;
        std::vector<std::string>::iterator it = find(commands.begin(), commands.end(), cmd);
 
        if (it != commands.end())
            index = it - commands.begin();
        else {
            throw (InvalidCommand());
        }
        std::cout << message << std::endl;
        switch (index)
        {
            case 0:
                channel_s = message.substr(pos + 1, message.length() - pos - 1);
                // TODO Implement check_channel(std::string channel) check_channel(channel) < 0
                cout << channel_s << endl;
                if (channel_s.compare("#ratio") != 0 /* ratio cette condition */)
                {
                    std::string msg;
                    msg = "Unable to join the channel: " + channel_s + " is unknown.\n";
                    // send(fd, msg.c_str(), strlen(msg.c_str()), 0);
                    std::cout << msg << std::endl;
                }
                else
                {
                    string reply;
                    reply = header_2(params->irc_serv, params->user_id, "") + "JOIN " + channel_s + "\r\n";
                    send(params->client_socket, reply.c_str(), strlen(reply.c_str()), 0);
                    
                    send(params->client_socket, ":127.0.0.1 332 dOD #ratio :Bonjour et gros ratio a toi:)\r\n", strlen(":127.0.0.1 332 dOD #ratio :Bonjour et gros ratio a toi:)\r\n"), 0);
                    send(params->client_socket, ":127.0.0.1 353 dOD = #ratio :@dOD\r\n", strlen(":127.0.0.1 353 dOD = #ratio :@dOD\r\n"), 0);
                    send(params->client_socket, ":127.0.0.1 366 dOD #ratio :End of NAMES list\r\n", strlen(":127.0.0.1 366 dOD #ratio :End of NAMES list\r\n"), 0);
                }
                break;
            case 1:
                std::cout << "PART" << std::endl;
                channel_s = message.substr(pos + 1, message.length() - pos - 3);
                // TODO Implement inChannel(User user, std::string channel) check_channel(channel) < 0
                if (channel_s.compare("#ratio") != 0 /* ratio cette condition */)
                {
                    std::string msg;
                    msg = "Unable to join the channel: " + channel_s + " is unknown.\n";
                    // send(fd, msg.c_str(), strlen(msg.c_str()), 0);
                }
                else
                {
                    std::string msg;
                    msg = std::string(channel_s + " joined.\n");
                    // send(fd, msg.c_str(), strlen(msg.c_str()), 0);
                }
                break;
            case 2:
                send(params->client_socket, "QUIT", strlen("QUIT"), MSG_DONTWAIT);
                exit(EXIT_SUCCESS);
            case 3:
                std::cout << "SETNICKNAME" << std::endl;
                break;
            case 4:
                std::cout << "SETUSERNAME" << std::endl;
                break;
            case 5:
                break;
            default:
                throw(InvalidCommand());
        }
    }
    catch (const InvalidCommand e) {
        std::cerr << e.info() << std::endl;
		return (UNKNOWN_COMMAND);
    }
    return (0);
}
