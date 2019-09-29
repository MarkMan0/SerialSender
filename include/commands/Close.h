#pragma once

#include <string>
#include "SerialManager.h"

#include "Command.h"

namespace Commands {
	class CloseCmd : public Command {
	private:
		std::shared_ptr<SerialManager> manager;
	public:
		CloseCmd(const std::shared_ptr<SerialManager>& _manager) : Command("Close"), manager(_manager) {}

		void execute(const std::string&) override;
	};

}

