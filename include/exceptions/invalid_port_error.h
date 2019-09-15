#pragma once

#include <stdexcept>
#include <string>

class invalid_port_error : public std::runtime_error {
private:

public:
	invalid_port_error(const std::string& s) : std::runtime_error(s) {}

};