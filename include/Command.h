#pragma once

#include <string>

class Command
{
private:
    std::string name;
public:
    Command(const std::string& _name) : name(_name) {}
    virtual ~Command() {}
    virtual void execute(const std::string& line) = 0;
    const std::string& getName() const { return name; }
};