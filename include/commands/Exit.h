#pragma once

#include <string>
#include <memory>

#include "SerialManager.h"
#include "Command.h"

class ExitCmd : public Command {
private:

    std::shared_ptr<SerialManager> manager;

public:
    ExitCmd(const std::shared_ptr<SerialManager>& _manager) : Command("Exit"), manager(_manager) {}

    void execute(const std::string&) override;

    Command* clone() const override;
};