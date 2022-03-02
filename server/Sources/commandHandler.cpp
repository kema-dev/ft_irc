#include "../Classes/commandHandler.hpp"

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
    commands.push_back("WRITE");
    try {
        int pos;
        char *msg;
        std::string cmd = message;
        pos = cmd.find(' ');
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
                send(fd, "ratio:WTF?", strlen("ratio:WTF?"), MSG_DONTWAIT);
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
            case 5:
                std::cout << "WRITE" << std::endl;
                msg = new char[message.length() - pos + 1]();
                message.copy(msg, message.length(), pos + 1);
                std::cout << fd << std::endl;
                std::cout << msg << std::endl;
            
                send(fd, msg, strlen(msg), MSG_DONTWAIT);
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
