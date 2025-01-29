#include <cstdlib>
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

std::vector<std::string> getPaths() {
    std::string path = std::getenv("PATH");
    std::vector<std::string> pathv;
    int i = 0;
    while(i < path.size()) {
        int end = path.find(':', i);
        if (end == std::string::npos) {
            pathv.push_back(path.substr(i));
            break;
        }
        pathv.push_back(path.substr(i, end-i));
        i = end+1;
    }
    return pathv;
}

bool isCommandInFolder(const std::string &command, const std::string &dir) {
    for (const auto& entry: fs::directory_iterator(dir)) {
        std::string name = entry.path();
        name = name.substr(dir.size()+1);
        // std::cout << name << '\n';
        if (name == command) {
            return true;
        }
    }
    return false;
}
std::string isCommandInPath(const std::string &command) {
    std::vector<std::string> path = getPaths();
    for (const auto& dir: path) {
        if (isCommandInFolder(command, dir)) {
            return dir;
        }
    }
    return "";
}

class InputHandler {
  public:
    static void notFoundHandler(const std::string &command)
    {
        std::cout << command << ": not found" << std::endl;
        return;
    }
    static void exitHandler(const std::string &input)
    {
        std::string rest = input.substr(5);
        if (rest.empty()) {
            return;
        }
        int ret = stoi(rest);
        exit(ret);
    }

    static void echoHandler(const std::string &input)
    {
        std::string rest = input.substr(5);
        if (rest.empty()) {
            return;
        }
        std::cout << rest << std::endl;
    }
    static bool isBuiltin(std::string command)
    {
        std::set<std::string> commands =
            std::set<std::string>({"type", "echo", "exit"});
        return commands.find(command) != commands.end();
    }
    static void typeHandler(const std::string &input)
    {
        std::string command = input.substr(5);
        if (isBuiltin(command)) {
            std::cout << command << " is a shell builtin" << std::endl;
        } else if (isCommandInPath(command) != "") {
            std::string dir = isCommandInPath(command);
            std::cout << command << " is" <<' ' << dir << '/'<<command << std::endl;
        }
        else {
            notFoundHandler(command);
        }
        return;
    }
};

void prompt()
{
    std::cout << "$ ";

    std::string input;
    std::getline(std::cin, input);
    if (input.find("exit ") != std::string::npos) {
        InputHandler::exitHandler(input);
    }
    else if (input.find("echo ") != std::string::npos) {
        InputHandler::echoHandler(input);
        return;
    }
    else if (input.find("type ") != std::string::npos) {
        InputHandler::typeHandler(input);
        return;
    }
    InputHandler::notFoundHandler(input);
}
int main()
{
    // Flush after every std::cout / std:cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;
    // std::vector<std::string> path = getPaths(); 
    // for (auto dir: path) {
    //     std::cout << dir << '\n';
    // }
    while (1) {
        prompt();
    }
}
