#pragma once

#include <string>
#include "SerialManager.h"

#include "Command.h"

namespace Commands {
	class CloseCmd : public Command {

	public:
		CloseCmd(const std::shared_ptr<SerialManager>& _manager) : Command("Close", _manager) {}

		void execute(const std::string&) override;
	};

}

