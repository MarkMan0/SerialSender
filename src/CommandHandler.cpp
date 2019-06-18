#include "CommandHandler.hpp"

#include <map>
#include <memory>
#include <iostream>

#include "Command.hpp"

    
void CommandHandler::registerCommand(Command* cmd) {


    if(!cmdMap->insert(std::make_pair(cmd->getName(), std::unique_ptr<Command>(cmd))).second) {
        //TODO:: throw some exception
    }
}


void CommandHandler::deleteCommand(const std::string& name) {
    if(cmdMap->erase(name) == 0) {
        //TODO:: throw exception
    }
}

void CommandHandler::run() {
    
    using namespace std;
        
    string cmd;
    do {
        cout << "Enter cmd" << endl;
        getline(cin, cmd);
        try {
            cmdMap->at(cmd)->execute();
        } catch (out_of_range e) {
            cout << "Unknown command: " << cmd << endl;
        }

    } while( cmd[0] != 'C');
}