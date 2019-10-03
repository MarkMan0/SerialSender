#pragma once
#include "Command.h"
#include <memory>
#include <string>
#include "MessageHandler.h"

namespace Commands {
	class PauseSendCmd : public Command {
	private:
		std::shared_ptr<MessageHandler> msgHandler;

	public:
		PauseSendCmd(const std::shared_ptr<MessageHandler>& handler) : Command("PauseSend"), msgHandler(handler) {}

		void execute(const std::string& cmd) override;
	};
}

