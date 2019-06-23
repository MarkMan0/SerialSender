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
#include "SerialManager.h"
#include "QueueWrapper.hpp"
#include <limits>

int main()
{ 
    using namespace std;

    
    std::shared_ptr<SerialManager> mng(new SerialManager("COM6", 250000));
    mng->startPeriodicRead(5000);
    cout << "end" << endl;
    
    for(unsigned long i = 0; i < ULONG_MAX; ++i ) {}

    CommandHandler handler;
    Command* cmd = new Commands::SendCmd(mng);
    handler.registerCommand(cmd);

    cmd = new Commands::ExitCmd(mng);

    handler.run();

    cout << "END" << endl;
    system("pause");
}