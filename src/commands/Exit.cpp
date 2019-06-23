#include "commands\Exit.h"


void ExitCmd::execute(const std::string& cmd) {
    manager->closePort();
}


Command* ExitCmd::clone() const {
    return new ExitCmd(manager);
}