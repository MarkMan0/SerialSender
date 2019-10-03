#include "commands/PauseSend.h"
#include <string>


void Commands::PauseSendCmd::execute(const std::string& cmd) {
	msgHandler->pauseSend();
}