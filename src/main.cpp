#include <cstdlib>
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <filesystem>
#include <unistd.h>
#include <sys/wait.h>

namespace fs = std::filesystem;

class PathHandler {
  public: 
    static std::vector<std::string> getPaths() {
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

    static bool isCommandInFolder(const std::string &command, const std::string &dir) {
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
    static std::string isCommandInPath(const std::string &command) {
        std::vector<std::string> path = getPaths();
        for (const auto& dir: path) {
            if (isCommandInFolder(command, dir)) {
                return dir;
            }
        }
        return "";
    }
};

class commandHandler {
    public:
        static void runCommand(std::string command, std::string options, std::string path) {
            // input: command should have the following format
            // /usr/bin/ls
            pid_t pid = fork();
            // std::cout << command << ' ' << options << ' ' << path << '\n';
            if (pid < 0) {
                return;
            } else if (pid == 0) {
                if (options.empty()) {
                    const char *args[] = {command.c_str(), nullptr};
                    execv(path.c_str(), const_cast<char *const *>(args));
                } else {
                    const char *args[] = {command.c_str(), options.c_str(), nullptr};
                    execv(path.c_str(), const_cast<char *const *>(args));
                }                
            } else {
                int status;
                wait(&status);
            }
        }
};

class InputHandler {
  public:
    static std::string getCommand(const std::string &input) {
        return input.substr(0, input.find(' '));
    }
    static std::string getOptions(const std::string &input) {
        int spacePos = input.find(' ');
        if (spacePos == std::string::npos) {
            return "";
        }
        return input.substr(spacePos+1);
    }
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
            std::set<std::string>({"type", "echo", "exit", "pwd"});
        return commands.find(command) != commands.end();
    }
    static void pwdHandler(const std::string &input) {
        std::cout << fs::current_path().string() << '\n';
        return;
    }
    static void typeHandler(const std::string &input)
    {
        std::string command = input.substr(5);
        if (isBuiltin(command)) {
            std::cout << command << " is a shell builtin" << std::endl;
        } else if (PathHandler::isCommandInPath(command) != "") {
            std::string dir = PathHandler::isCommandInPath(command);
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
    std::string command = InputHandler::getCommand(input);
    if (command.find("exit") != std::string::npos) {
        InputHandler::exitHandler(input);
    }
    else if (command.find("echo") != std::string::npos) {
        InputHandler::echoHandler(input);
        return;
    }
    else if (command.find("type") != std::string::npos) {
        InputHandler::typeHandler(input);
        return;
    } 
    else if (command.find("pwd") != std::string::npos) {
        InputHandler::pwdHandler(command);
        return;
    } 
    else if (PathHandler::isCommandInPath(command) != "") {
        std::string dir = PathHandler::isCommandInPath(command);
        std::string options = InputHandler::getOptions(input);
        if (dir != "") {
            commandHandler::runCommand(command, options, dir+"/"+command);
            return;
        }

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
