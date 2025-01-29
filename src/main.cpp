#include <iostream>

void prompt()
{
    std::cout << "$ ";

    std::string input;
    std::getline(std::cin, input);

    std::cout << input << ": command not found\n";
}
int main()
{
    // Flush after every std::cout / std:cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    while (1) {
        prompt();
    }
}
