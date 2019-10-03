#include <iostream>

#include <map>
#include <memory>
#include <string>
#include <thread>
#include <chrono>
#include <windows.h>

#include "Command.h"
#include "CommandHandler.h"
#include "SerialPort.h"
#include "commands\Send.h"
#include "commands\Exit.h"
#include "commands\AllMsg.h"
#include "SerialManager.h"
#include "commands/Open.h"
#include "commands/Close.h"
#include "commands/LoadFile.h"
#include "commands/ClearQueue.h"
#include "MessageHandler.h"
#include <limits>

int main()
{ 
    using namespace std;

	{
		std::shared_ptr<SerialManager> mng(new SerialManager());
		cout << "end" << endl;
		auto msgHandler = std::make_shared<MessageHandler>(mng);

		mng->setHandler(msgHandler);

		CommandHandler handler;

		handler.registerCommand(std::make_unique<Commands::SendCmd>(msgHandler));
		handler.registerCommand(std::make_unique<Commands::ExitCmd>(mng, msgHandler));
		handler.registerCommand(std::make_unique<Commands::AllMsgCmd>(msgHandler));
		handler.registerCommand(std::make_unique<Commands::OpenCmd>(mng));
		handler.registerCommand(std::make_unique<Commands::CloseCmd>(mng));
		handler.registerCommand(std::make_unique<Commands::LoadFileCmd>(msgHandler));
		handler.registerCommand(std::make_unique<Commands::ClearQueueCmd>(msgHandler));


		handler.run();
	}

    cout << "END" << endl;
    system("pause");
}