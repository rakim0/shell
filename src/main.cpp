#include <iostream>
#include <set>
#include <vector>

void notFoundHandler(const std::string &command)
{
    std::cout << command << ": not found" << std::endl;
    return;
}
void exitHandler(const std::string &input)
{
    std::string rest = input.substr(5);
    if (rest.empty()) {
        return;
    }
    int ret = stoi(rest);
    exit(ret);
}

void echoHandler(const std::string &input)
{
    std::string rest = input.substr(5);
    if (rest.empty()) {
        return;
    }
    std::cout << rest << std::endl;
}
bool checkBuiltin(std::string command)
{
    static std::set<std::string> commands({"type", "echo", "exit"});
    return commands.find(command) != commands.end();
}
void typeHandler(const std::string &input)
{
    std::string command = input.substr(5);
    if (checkBuiltin(command)) {
        std::cout << command << " is a shell builtin." << std::endl;
    }
    else {
        notFoundHandler(command);
    }
    return;
}

void prompt()
{
    std::cout << "$ ";

    std::string input;
    std::getline(std::cin, input);
    if (input.find("exit ") != std::string::npos) {
        exitHandler(input);
    }
    else if (input.find("echo ") != std::string::npos) {
        echoHandler(input);
        return;
    }
    else if (input.find("type ") != std::string::npos) {
        typeHandler(input);
        return;
    }
    notFoundHandler(input);
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
