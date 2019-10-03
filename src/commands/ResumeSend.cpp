#include "ResumeSend.h"
#include "..\..\include\commands\ResumeSend.h"

void Commands::ResumeSendCmd::execute(const std::string& cmd) {
	msgHandler->resumeSend();
}
