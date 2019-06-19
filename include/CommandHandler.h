#pragma once

#include <map>
#include <string>
#include <memory>

#include "Command.h" 

class CommandHandler
{
private:
    /* data */
    typedef std::map< std::string, std::unique_ptr<Command> > cmdContainer;

    std::unique_ptr< cmdContainer > cmdMap;
public:
    CommandHandler(/* args */) : cmdMap(new cmdContainer) {}
    ~CommandHandler() { }
    void registerCommand(Command*);
    void deleteCommand(const std::string& name);

    void run();

};
