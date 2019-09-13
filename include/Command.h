#pragma once

#include <string>
#include <memory>

#include "SerialManager.h"

class Command
{
private:
    std::string name;       //the name of the command, with what to call

protected:
	std::shared_ptr<SerialManager> manager;

public:
    Command(const std::string& _name, const std::shared_ptr<SerialManager>& _manager) : name(_name), manager(_manager) {}      //all commands have a name
    virtual ~Command() {}
    virtual void execute(const std::string& line) = 0;  //must be implemented in derived
    const std::string& getName() const { return name; }
};