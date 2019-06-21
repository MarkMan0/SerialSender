#include "SerialPort.h"

#include <windows.h>
#include <iostream>
#include <string>

SerialPort::~SerialPort() {
    closeSerial();
    delete[] buff;
}

SerialPort::SerialPort() {
}


void SerialPort::open(const std::string& name, DCB paramsArg, COMMTIMEOUTS timeouts) {

    //VS Code shows error, but it compiles without warnings
    hSerial = CreateFile( name.c_str(), GENERIC_READ | GENERIC_WRITE, 
                                    0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    DCB serialParams = { 0 };
    serialParams.DCBlength = sizeof(serialParams);
    GetCommState(hSerial, &serialParams);

    //TODO:: this is rly bad
    serialParams.BaudRate = paramsArg.BaudRate;
    serialParams.ByteSize = paramsArg.ByteSize;
    serialParams.StopBits = paramsArg.StopBits;
    serialParams.Parity = paramsArg.Parity;
    serialParams.fOutxDsrFlow = paramsArg.fOutxDsrFlow;
    serialParams.fRtsControl = paramsArg.fRtsControl;

    if(!SetCommState(hSerial, &serialParams)) {
        //TODO: exceptions...
        std::cout << "Error setting state" << std::endl;
    }

    if(!SetCommTimeouts(hSerial, &timeouts)) {
        std::cout << "ERR setting timeouts" << std::endl; 
    }
}


void SerialPort::closeSerial() {
    CloseHandle(hSerial);
}

void SerialPort::readErr() {
    memset(lastErrBuff, '\0', errBuffSz);
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL,
                GetLastError(),
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                lastErrBuff,        //compiles w/o warning
                errBuffSz,
                NULL);
}


void SerialPort::send(const std::string& cmd) {

    std::string msg = cmd + "\r";
    
    DWORD bytesSent = 0;
    memset(buff, '\0', buffSz);
    if(!WriteFile(hSerial, msg.c_str(), msg.size(), &bytesSent, NULL)) {
        readErr();
        //TODO: Handle properly
        std::cout << "ERR Send" << std::endl << "\t" << lastErrBuff << std::endl;
    }
    std::cout << receive() << std::endl;
}

const std::string& SerialPort::receive() {
    DWORD bytesRead = 0;
    //get response
    if(!ReadFile(hSerial, buff, buffSz, &bytesRead, NULL)){
        //error occurred. Report to user.
        readErr();
        std::cout << "ERR read" << std::endl << "\t" << lastErrBuff << std::endl;
    }
    response.clear();
    response = buff;
    return response;
}

const std::string& SerialPort::lastResponse() const {
    return response;
}