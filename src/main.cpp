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
#include "QueueWrapper.hpp"
#include "commands/Open.h"
#include <limits>

int main()
{ 
    using namespace std;

	{
		std::shared_ptr<SerialManager> mng(new SerialManager());
		cout << "end" << endl;


		CommandHandler handler;

		handler.registerCommand(std::make_unique<Commands::SendCmd>(mng));

		handler.registerCommand(std::make_unique<Commands::ExitCmd>(mng));

		handler.registerCommand(std::make_unique<Commands::AllMsgCmd>(mng));

		handler.registerCommand(std::make_unique<Commands::Open>(mng));

		handler.run();
	}

    cout << "END" << endl;
    system("pause");
}