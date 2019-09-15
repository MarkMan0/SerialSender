#include "Open.h"

#include <vector>
#include <iostream>
#include <stdexcept>

#include "SerialManager.h"


void Commands::OpenCmd::execute(const std::string& line) {

	std::vector<std::string> strVec;

	std::string::size_type n = 0, curr = 0;

	while ( (curr = line.find(' ', n)) != std::string::npos) {
		strVec.push_back(line.substr(n, curr - n));
		n = curr + 1;
	}
	strVec.push_back(line.substr(n, line.size() - n));
	try {
		if (strVec.size() >= 3)
			manager->openPort(strVec.at(1), stoul(strVec.at(2)));
		else
			manager->openPort();
	} 
	catch (std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
	}
	catch (std::invalid_argument& e) {
		std::cerr << "Couldn't convert argument 2 to numeric" << std::endl;
	}
}
