#pragma once

#include <stdexcept>
#include <string>

class serial_io_error : public std::runtime_error {
private:

public:
	serial_io_error(const std::string& s) : std::runtime_error(s) {}

};