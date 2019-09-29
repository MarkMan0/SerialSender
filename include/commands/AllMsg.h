
#pragma once

#include "Command.h"
#include <memory>
#include "SerialManager.h"
#include <string>
#include <iostream>

namespace Commands {
    class AllMsgCmd : public Command {
    private:

    public:
        AllMsgCmd(const std::shared_ptr<SerialManager> _manager) : Command("AllMsg", _manager) {}

        void execute(const std::string& cmd) override {
			std::cerr << "NOT IMPLEMNTED" << std::endl;
        } 
    };
}