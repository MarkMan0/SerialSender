#include "CommandHandler.h"

#include <map>
#include <memory>
#include <iostream>

#include "Command.h"

    
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
        string actCmd = cmd.substr(0, cmd.find_first_of(" "));  //search the string for space or \0
        try {
            cmdMap->at(cmd)->execute(cmd);
        } catch (out_of_range e) {
            cout << "Unknown command: " << cmd << endl;
        }

    } while( cmd[0] != 'C');
}