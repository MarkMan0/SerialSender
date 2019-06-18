#include <iostream>

#include <map>
#include <memory>
#include <string>
#include "Command.hpp"
#include "CommandHandler.hpp"


class cmdA : public Command {
public:
    cmdA() : Command("A") {}
    void execute() {
        std::cout << "cmd A" << std::endl;
    }
    ~cmdA() {
        std::cout << "A dest" << std::endl;
    }
};

class cmdB : public Command {
public:
    cmdB() : Command("B") {}
    void execute() {
        std::cout << "cmd B" << std::endl;
    }
    ~cmdB() {
        std::cout << "B dest" << std::endl;
    }
};

class cmdC : public Command {
public:
    cmdC() : Command("C") {}
    void execute() {
        std::cout << "cmd C - exit" << std::endl;
    }
    ~cmdC() {
        std::cout << "C dest" << std::endl;
    }
};

int main()
{ 
    using namespace std;
    CommandHandler handler;
    handler.registerCommand(new cmdA);
    handler.registerCommand(new cmdB);
    handler.registerCommand(new cmdC);
    handler.run();
    cout << "END" << endl;
    system("pause");
}