#pragma once
#include "Command.h"
#include <iostream>

#include "SerialPort.h"

namespace Commands {

    class SendCmd : public Command {

    private:
        SerialPort* port;

    public:
        SendCmd() : Command("Send") {}
        void execute(const std::string&);

    };

}   