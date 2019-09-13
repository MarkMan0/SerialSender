#pragma once
#include "Command.h"
#include <iostream>
#include <memory>
#include "SerialManager.h"

#include "SerialPort.h"

namespace Commands {

    class SendCmd : public Command {

    private:

    public:
		SendCmd(const std::shared_ptr<SerialManager>& _manager) : Command("Send", _manager) {}
        void execute(const std::string&) override;
    };

}   