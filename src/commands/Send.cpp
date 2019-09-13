

#include "commands\Send.h"
#include <algorithm>


//executes a send command
void Commands::SendCmd::execute(const std::string& line) {
    //command is like: Send ...
    //this removes the Send part
    std::string msg = line.substr((std::min)(line.find_first_of(" ") + 1, line.size()));
    
    manager->writeMsg(msg);     //sends a message
    //manager->readPort();        //reads the response
    std::string resp = manager->nextMsg();
	return;
    while( resp.find("ok") == std::string::npos && resp.find("halted") == std::string::npos ) {
        //manager->readPort();
        if(resp.size() > 1)
            std::cout << resp << std::endl;
        resp = manager->nextMsg();
    }
    
    std::cout << resp << std::endl;   //prints the response to CMD
}
