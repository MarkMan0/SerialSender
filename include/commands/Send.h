#pragma once
#include "Command.h"
#include <iostream>
#include <memory>
#include "SerialManager.h"

#include "SerialPort.h"

namespace Commands {

    class SendCmd : public Command {

    private:
        std::shared_ptr<SerialManager> manager;     //shared serial port

    public:
        SendCmd(const std::shared_ptr<SerialManager>& _manager);
        void execute(const std::string&);
        Command* clone() const;     //not rly needed
    };

}   