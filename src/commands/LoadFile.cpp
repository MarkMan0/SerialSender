#include "LoadFile.h"
#include <istream>
#include <fstream>
#include <iterator>

void Commands::LoadFileCmd::execute(const std::string& cmd) {
	auto filename = cmd.substr(cmd.find(' '));
	
	std::ifstream file(filename);
	std::string line;
	while (std::getline(file, line))
		msgHandler->enqueueSend(line, 0);	//low priority
}
