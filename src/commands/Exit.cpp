#include "commands\Exit.h"


void Commands::ExitCmd::execute(const std::string& cmd) {
    manager->closePort();
}

