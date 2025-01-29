#include <iostream>
void prompt()
{
    std::cout << "$ ";

    std::string input;
    std::getline(std::cin, input);
    if (input.find("exit ") != std::string::npos) {
        int ret = stoi(input.substr(input.find("exit ") + 5));
        if (ret == 0) {
            exit(ret);
        }
    }
    else if (input.find("echo ") != std::string::npos) {
        std::string rest = input.substr(5);
        std::cout << rest << '\n';
        return;
    }
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
