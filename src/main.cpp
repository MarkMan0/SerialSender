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
    Command* cmd2 = new Commands::SendCmd(*dynamic_cast<Commands::SendCmd*>(cmd));   

    cout << mng.use_count() << endl;

    delete cmd2;
    cout << mng.use_count() << endl;

    delete cmd;
    cout << mng.use_count() << endl;

    mng.reset();
    cout << mng.use_count() << endl;

    cout << "END" << endl;
    system("pause");
}