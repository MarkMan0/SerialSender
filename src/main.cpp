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
    
    SerialManager manager("COM6", 250000);
    manager.writeMsg("M503\rM122");
    manager.readPort();
    cout << manager.lastMsg() << endl;
    manager.closePort();

    manager.~SerialManager();

    cout << "END" << endl;
    system("pause");
}