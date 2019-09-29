
#pragma once

#include "Command.h"
#include <memory>
#include "MessageHandler.h"
#include <string>
#include <iostream>

namespace Commands {
    class AllMsgCmd : public Command {
    private:
		std::shared_ptr<MessageHandler> msgHandler;
    public:
        AllMsgCmd(const std::shared_ptr<MessageHandler> handler) : Command("AllMsg"), msgHandler(handler){}

        void execute(const std::string& cmd) override {
			std::cerr << "NOT IMPLEMNTED" << std::endl;
        } 
    };
}