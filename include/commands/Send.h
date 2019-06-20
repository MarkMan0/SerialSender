#pragma once
#include "Command.h"
#include <iostream>

#include "SerialPort.h"

namespace Commands {

    class SendCmd : public Command {

    private:
        SerialPort* port;

    public:
        SendCmd(SerialPort* port);
        void execute(const std::string&);
        Command* clone() const;
    };

}   