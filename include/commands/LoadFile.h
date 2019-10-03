#pragma once
#include "Command.h"
#include "MessageHandler.h"
#include <memory>
namespace Commands {
	class LoadFileCmd : public Command {
	private:
		std::shared_ptr<MessageHandler> msgHandler;

	public:
		LoadFileCmd(const std::shared_ptr<MessageHandler>& handler) : Command("LoadFile"), msgHandler(handler) {}

		void execute(const std::string& cmd) override;
	};
}
