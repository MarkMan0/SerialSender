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

    cmdContainer cmdMap;        //<string, *cmd> map, where first is the name, second is the command
public:
    CommandHandler(/* args */) {}
    CommandHandler(const CommandHandler& );
    ~CommandHandler() { }
    void registerCommand(std::unique_ptr<Command>&& cmdPtr);     //puts a command into the map
    void deleteCommand(const std::string& name);    //removes a command from the map

    void run();     //starts listening for commands

};
