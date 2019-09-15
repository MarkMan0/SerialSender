#include "CommandHandler.h"

#include <map>
#include <memory>
#include <iostream>
#include <string>

#include "Command.h"

    
void CommandHandler::registerCommand(std::unique_ptr<Command>&& cmdPtr) {
    //insert a command to the map
	cmdMap[cmdPtr->getName()] = std::move(cmdPtr);
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
            cout << "Unknown command: " << cmd << endl;
        }

    } while( actCmd.compare("Exit"));        //character C stops the run();
}