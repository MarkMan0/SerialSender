

#include "commands\Send.h"
#include <algorithm>

//inits the Command name and saves the manager
Commands::SendCmd::SendCmd(const std::shared_ptr<SerialManager>& _manager) : Command("Send"), manager(_manager) {
    //port has to be initialized
}

//executes a send command
void Commands::SendCmd::execute(const std::string& line) {
    //command is like: Send ...
    //this removes the Send part
    std::string msg = line.substr(std::min(line.find_first_of(" ") + 1, line.size()));
    
    manager->writeMsg(msg);     //sends a message
    manager->readPort();        //reads the response
    std::cout << manager->lastMsg() << std::endl;   //prints the response to CMD
}

//not needed
Command* Commands::SendCmd::clone() const {
    return new SendCmd(manager);       
}