#pragma once

#include <string>
#include <memory>

#include "SerialManager.h"
#include "Command.h"

namespace Commands {
	class ExitCmd : public Command {
	private:

	public:
		ExitCmd(const std::shared_ptr<SerialManager>& _manager) : Command("Exit", _manager) {}

		void execute(const std::string&) override;

	};

}