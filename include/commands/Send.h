#pragma once
#include "Command.h"
#include <iostream>
#include <memory>
#include "SerialManager.h"

#include "SerialPort.h"

namespace Commands {

    class SendCmd : public Command {

    private:
		std::shared_ptr<MessageHandler> msgHandler;

    public:
		SendCmd(const std::shared_ptr<MessageHandler>& handler) : Command("Send"), msgHandler(handler) {}
        void execute(const std::string&) override;
    };

}   