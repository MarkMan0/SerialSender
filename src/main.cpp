#include <iostream>

#include <map>
#include <memory>
#include <string>

#include <windows.h>

#include "Command.h"
#include "CommandHandler.h"
#include "SerialPort.h"
#include "commands\Send.h"

int main()
{ 
    using namespace std;
    
    CommandHandler handler;
    handler.run();

    cout << "END" << endl;
    system("pause");
}