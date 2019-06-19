#include <iostream>

#include <map>
#include <memory>
#include <string>

#include <windows.h>

#include "Command.hpp"
#include "CommandHandler.hpp"


class cmdA : public Command {
public:
    cmdA() : Command("A") {}
    void execute() {
        std::cout << "cmd A" << std::endl;
    }
    ~cmdA() {
        std::cout << "A dest" << std::endl;
    }
};

class cmdB : public Command {
public:
    cmdB() : Command("B") {}
    void execute() {
        std::cout << "cmd B" << std::endl;
    }
    ~cmdB() {
        std::cout << "B dest" << std::endl;
    }
};

class cmdC : public Command {
public:
    cmdC() : Command("C") {}
    void execute() {
        std::cout << "cmd C - exit" << std::endl;
    }
    ~cmdC() {
        std::cout << "C dest" << std::endl;
    }
};

int main()
{ 
    using namespace std;
    
    HANDLE hSerial = CreateFile( "\\\\.\\COM6", GENERIC_READ | GENERIC_WRITE, 
                                    0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    DCB serialParams = { 0 };
    serialParams.DCBlength = sizeof(serialParams);
    GetCommState(hSerial, &serialParams);

    serialParams.BaudRate = 250000;
    serialParams.ByteSize = 8;
    serialParams.StopBits = ONESTOPBIT;
    serialParams.Parity = NOPARITY;
    serialParams.fOutxDsrFlow = TRUE;
    serialParams.fRtsControl = RTS_CONTROL_ENABLE;

    if(!SetCommState(hSerial, &serialParams)) {
        cout << "Error setting state" << endl;
    }

    COMMTIMEOUTS timeout = { 0 };
    timeout.ReadIntervalTimeout = 50;
    timeout.ReadTotalTimeoutConstant = 50;
    timeout.ReadTotalTimeoutMultiplier = 50;
    timeout.WriteTotalTimeoutConstant = 50;
    timeout.WriteTotalTimeoutMultiplier = 10;

    if(!SetCommTimeouts(hSerial, &timeout)) {
        cout << "ERR setting timeouts" << endl; 
    }

    size_t n = 50000;

    char *buff = new char[n];
    memset(buff, '\0', n);

    strcpy(buff, "M122\r\n");

    DWORD dwBytesRead = 0;
    if(!WriteFile(hSerial, buff, 5, &dwBytesRead, NULL)) {
        cout << "ERR write" << endl;
    }

    memset(buff, '\0', n);

    if(!ReadFile(hSerial, buff, n, &dwBytesRead, NULL)){
        //error occurred. Report to user.
        cout << "ERR read" << endl;
    }

    cout << buff << endl;

    delete[] buff;

    CloseHandle(hSerial);
    cout << "END" << endl;
    system("pause");
}