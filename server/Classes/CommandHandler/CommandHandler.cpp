#include "CommandHandler.hpp"

int command_check(std::string message, int fd)
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
                channel_s = message.substr(pos + 1, message.length() - pos - 3);
                // TODO Implement check_channel(std::string channel) check_channel(channel) < 0
                if (channel_s.compare("#ratio") != 0 /* ratio cette condition */)
                {
                    std::string msg;
                    msg = "Unable to join the channel: " + channel_s + " is unknown.\n";
                    send(fd, msg.c_str(), strlen(msg.c_str()), 0);
                    std::cout << msg << std::endl;
                }
                else
                {
                    std::string msg;
                    msg = std::string(channel_s + " joined.\n");

                    send(fd, ":dOD!dginisty@localhost JOIN ratio\r\n", strlen(":dOD!dginisty@localhost JOIN ratio\r\n"), MSG_DONTWAIT);
                    send(fd, ":dOD!dginisty@localhost 332 : ratio Bonjour et gros ratio a toi:)\r\n", strlen(":dOD!dginisty@localhost 332 : ratio Bonjour et gros ratio a toi:)\r\n"), MSG_DONTWAIT);
                    send(fd, ":ratio 353 dOD = ratio : dOD\r\n", strlen(":ratio 353 dOD = ratio : dOD\r\n"), MSG_DONTWAIT);
                    send(fd, ":ratio 366 dOD ratio : End of NAMES list\r\n", strlen(":ratio 366 dOD ratio : End of NAMES list\r\n"), MSG_DONTWAIT);
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
                    send(fd, msg.c_str(), strlen(msg.c_str()), 0);
                }
                else
                {
                    std::string msg;
                    msg = std::string(channel_s + " joined.\n");
                    send(fd, msg.c_str(), strlen(msg.c_str()), 0);
                }
                break;
            case 2:
                send(fd, "QUIT", strlen("QUIT"), MSG_DONTWAIT);
                break;
            case 3:
                std::cout << "SETNICKNAME" << std::endl;
                break;
            case 4:
                std::cout << "SETUSERNAME" << std::endl;
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
