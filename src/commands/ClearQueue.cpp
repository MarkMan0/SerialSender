#include "ClearQueue.h"
#include "..\..\include\commands\ClearQueue.h"

void Commands::ClearQueueCmd::execute(const std::string& cmd) {
	msgHandler->clearQueue();
}
