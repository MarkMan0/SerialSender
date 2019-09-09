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
#include <limits>

int main()
{ 
    using namespace std;

    
    std::shared_ptr<SerialManager> mng(new SerialManager("COM6", 250000));
    mng->startPeriodicRead(5000);
    cout << "end" << endl;
    

    CommandHandler handler;
    Command* cmd = new Commands::SendCmd(mng);
    handler.registerCommand(cmd);

    cmd = new Commands::ExitCmd(mng);
    handler.registerCommand(cmd);

    cmd = new Commands::AllMsgCmd(mng);
    handler.registerCommand(cmd);

    handler.run();

    cout << "END" << endl;
    system("pause");
}