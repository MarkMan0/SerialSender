#pragma once

#include <memory>

#include "Command.h"
#include "SerialManager.h"

namespace Commands {
	class OpenCmd : public Command {

	public:
		OpenCmd(const std::shared_ptr<SerialManager>& _manager) : Command("Open", _manager) {}

		void execute(const std::string& line) override;
	};

}

