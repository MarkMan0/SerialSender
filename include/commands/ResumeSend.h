#pragma once
#include "Command.h"
#include <memory>
#include <string>
#include "MessageHandler.h"

namespace Commands {
	class ResumeSendCmd : public Command {
	private:
		std::shared_ptr<MessageHandler> msgHandler;

	public:
		ResumeSendCmd(const std::shared_ptr<MessageHandler>& handler) : Command("ResumeSend"), msgHandler(handler) {}

		void execute(const std::string& cmd) override;
	};
}

