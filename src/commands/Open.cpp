#include "Open.h"

#include <vector>
#include <iostream>

#include "SerialManager.h"


void Commands::OpenCmd::execute(const std::string& line) {

	std::vector<std::string> strVec;

	std::string::size_type n = 0, curr = 0;

	while ( (curr = line.find(' ', n)) != std::string::npos) {
		strVec.push_back(line.substr(n, curr - n));
		n = curr + 1;
	}
	strVec.push_back(line.substr(n, line.size() - n));

	if (strVec.size() >= 3)
		manager->openPort(strVec[1], stoul(strVec[2]));
	else
		manager->openPort();
}
