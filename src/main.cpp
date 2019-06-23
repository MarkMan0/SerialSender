#include <iostream>

#include <map>
#include <memory>
#include <string>

#include <windows.h>

#include "Command.h"
#include "CommandHandler.h"
#include "SerialPort.h"
#include "commands\Send.h"
#include "SerialManager.h"

int main()
{ 
    using namespace std;
    
    std::shared_ptr<SerialManager> mng(new SerialManager("COM6", 250000));

    Command* cmd = new Commands::SendCmd(mng);

    CommandHandler handler;
    handler.registerCommand(cmd);
    handler.run();

    cout << "END" << endl;
    system("pause");
}