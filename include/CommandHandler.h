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

    cmdContainer cmdMap;
public:
    CommandHandler(/* args */) {}
    CommandHandler(const CommandHandler& );
    ~CommandHandler() { }
    void registerCommand(Command*);
    void deleteCommand(const std::string& name);

    void run();

};
