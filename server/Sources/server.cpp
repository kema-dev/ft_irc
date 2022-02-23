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
        return (-1);
    }
    std::cout << "HELLO WORLD!" << std::endl;
    return (0);
}