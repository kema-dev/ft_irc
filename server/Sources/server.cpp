#include <iostream>
#include "../Classes/server.hpp"
#include "../Classes/error.hpp"

int main(int argc, char **argv)
{
    (void)argv;
    try {
        if (argc != 3)
            throw(BadNumberArgs());
    }
    catch(const BadNumberArgs e) {
        std::cerr << e.info() << std::endl;
        return (BAD_NUMBER_ARGS);
    }
    try {
        if (std::atoi(argv[1]) < 1 || std::atoi(argv[1]) > 65535)
            throw(InvalidPort());
    }
    catch (const InvalidPort e) {
        std::cerr << e.info() << std::endl;
        return (INVALID_PORT);
    }

    std::cout << "Port: " << argv[1] << std::endl;
    std::cout << "Password: " << argv[2] << std::endl;
    
    return (0);
}