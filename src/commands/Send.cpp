

#include "commands\Send.h"
#include <algorithm>


//executes a send command
void Commands::SendCmd::execute(const std::string& line) {
    //command is like: Send ...
    //this removes the Send part
    std::string msg = line.substr((std::min)(line.find_first_of(" ") + 1, line.size()));
    
	try {
		manager->writeMsg(msg);     //sends a message
	}
	catch (std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
	}
}
