#include "commands\Exit.h"


void Commands::ExitCmd::execute(const std::string& cmd) {
    manager->closePort();
}


Command* Commands::ExitCmd::clone() const {
    return new ExitCmd(manager);
}