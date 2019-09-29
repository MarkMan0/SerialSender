#pragma once

#include <string>
#include <memory>

#include "SerialManager.h"
#include "MessageHandler.h"
#include "Command.h"

namespace Commands {
	class ExitCmd : public Command {
	private:
		std::shared_ptr<SerialManager> manager;
		std::shared_ptr<MessageHandler> msgHandler;

	public:
		ExitCmd(const std::shared_ptr<SerialManager>& _manager, const std::shared_ptr<MessageHandler>& handler) : Command("Exit"), manager(_manager), msgHandler(handler) {}

		void execute(const std::string&) override;

	};

}