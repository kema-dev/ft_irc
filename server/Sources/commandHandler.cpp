#include "../Classes/commandHandler.hpp"

int command_check(std::string message)
{
    std::vector<std::string> commands;
    commands.push_back("JOIN");
    commands.push_back("PART");
    commands.push_back("QUIT");
    commands.push_back("NICK");
    commands.push_back("USER");
    try {
        int pos;
        pos = message.find(' ');
        message.erase(pos, message.length() - pos);
        
        int index;
        std::vector<std::string>::iterator it = find(commands.begin(), commands.end(), message);
 
        if (it != commands.end())
            index = it - commands.begin();
        else {
            throw (InvalidCommand());
        }
        switch (index)
        {
            case 0:
                std::cout << "JOIN" << std::endl;
                break;
            case 1:
                std::cout << "LEAVE" << std::endl;
                break;
            case 2:
                std::cout << "QUIT" << std::endl;
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
