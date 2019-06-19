#include <iostream>

#include <map>
#include <memory>
#include <string>

#include <windows.h>

#include "Command.h"
#include "CommandHandler.h"
#include "SerialPort.h"

int main()
{ 
    using namespace std;
    
    
    SerialPort port;

    port.send("M122");

    cout << port.lastResponse() << endl;

    cout << "END" << endl;
    system("pause");
}