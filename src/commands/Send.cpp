

#include "commands\Send.h"
#include <algorithm>

Commands::SendCmd::SendCmd(const std::shared_ptr<SerialManager>& _manager) : Command("Send"), manager(_manager) {
    //port has to be initialized
}

void Commands::SendCmd::execute(const std::string& line) {
    std::string msg = line.substr(std::min(line.find_first_of(" ") + 1, line.size()));     //from the first space or nothing
    manager->writeMsg(msg);
    std::cout << manager->lastMsg() << std::endl;
}


Command* Commands::SendCmd::clone() const {
    return new SendCmd(manager);
}