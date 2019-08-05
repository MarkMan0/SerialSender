
#pragma once

#include "Command.h"
#include <memory>
#include "SerialManager.h"
#include <string>
#include <iostream>

namespace Commands {
    class AllMsgCmd : public Command {
    private:
        std::shared_ptr<SerialManager> manager;

    public:
        AllMsgCmd(const std::shared_ptr<SerialManager> _manager) : Command("AllMsg"), manager(_manager) {}

        Command* clone() const override {return nullptr;}
        void execute(const std::string& cmd) override {
            std::string str;
            while((str = manager->nextMsg()).size() > 1) {
                std::cout << str << std::endl;
            }
        } 
    };
}