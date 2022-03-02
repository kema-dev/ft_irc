#include "CommandHandler.hpp"

int command_check(std::string message, int fd)
{
    try{
        if (message.empty() == true)
            throw(EmptyCommand());
    }
    catch (const EmptyCommand e) {
        std::cerr << e.info() << std::endl;
		return (EMPTY_COMMAND);
    }
    std::vector<std::string> commands;
    commands.push_back("JOIN");
    commands.push_back("PART");
    commands.push_back("QUIT");
    commands.push_back("NICK");
    commands.push_back("USER");
    try {
        size_t pos;
        char *channel;
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
        switch (index)
        {
            case 0:
                std::cout << "JOIN" << std::endl;
                channel = new char[message.length() - pos + 1]();
                message.copy(channel, message.length(), pos + 1);
                channel_s = std::string(channel);
                // TODO Implement check_channel(std::string channel) check_channel(channel) < 0
                
                std::cout << channel_s << std::endl;
                if (channel_s.compare("ratio") != 0)
                {
                    std::string msg;
                    msg = "Unable to join the channel: " + channel_s + " is unknown.";
                    send(fd, msg.c_str(), strlen(msg.c_str()), 0);
                }
                else
                {
                    std::string msg;
                    msg = channel_s + " joined.";
                    send(fd, msg.c_str(), strlen(msg.c_str()), 0);
                }
                break;
            case 1:
                std::cout << "LEAVE" << std::endl;
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
