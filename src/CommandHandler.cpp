#include "CommandHandler.h"

#include <map>
#include <memory>
#include <iostream>
#include <string>

#include "Command.h"

    
void CommandHandler::registerCommand(Command* cmd) {
    //insert a command to the map
    if(!cmdMap.insert(std::make_pair(cmd->getName(), std::unique_ptr<Command>(cmd))).second) {
        //TODO:: throw some exception
    }
}

CommandHandler::CommandHandler(const CommandHandler& old) {
    //creates a copy of the map
    for(const auto& it : old.cmdMap) {
        this->cmdMap.insert(std::make_pair(it.first, std::unique_ptr<Command>(it.second->clone())));
    }
}

//deletes a command from the map
void CommandHandler::deleteCommand(const std::string& name) {
    if(cmdMap.erase(name) == 0) {
        //TODO:: throw exception
    }
}

void CommandHandler::run() {
    
    using namespace std;
        
    string cmd;
    string actCmd;
    do {
        cout << "Enter cmd" << endl;
        getline(cin, cmd);      //read a line from the serial
        actCmd = cmd.substr(0, cmd.find_first_of(" "));  //extract the first word
        try {
            cmdMap.at(actCmd)->execute(cmd);    //try to find&execute a command
        } catch (out_of_range e) {      //TODO:: fix the warning
            //command not found
            cout << "Unknown command: " << cmd << endl << e.what() << endl;
        }

    } while( actCmd.compare("Exit"));        //character C stops the run();
}