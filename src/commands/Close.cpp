#include "Close.h"
#include "..\..\include\commands\Close.h"

void Commands::CloseCmd::execute(const std::string&) {

	manager->closePort();
}
