#pragma once
#include "Command.h"
#include "MessageHandler.h"
#include <memory>
#include <string>

namespace Commands {
	class ClearQueueCmd : public Command {
	private:
		std::shared_ptr<MessageHandler> msgHandler;

	public:
		ClearQueueCmd(const std::shared_ptr<MessageHandler>& handler) : Command("ClearQueue"), msgHandler(handler) {}

		void execute(const std::string& cmd) override;
	};
}
