

#include "commands\Send.h"
#include <algorithm>

Commands::SendCmd::SendCmd(SerialPort* _port) : Command("Send") {
    this->port = _port;
    //port has to be initialized
}

void Commands::SendCmd::execute(const std::string& line) {
    std::string msg = line.substr(std::min(line.find_first_of(" ") + 1, line.size()));     //from the first space or nothing
    port->send(msg);
    std::cout << port->lastResponse() << std::endl;
}


Command* Commands::SendCmd::clone() const {
    return new SendCmd(this->port);
}