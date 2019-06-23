#include "SerialPort.h"

#include <windows.h>
#include <iostream>
#include <string>

//closes the port and frees the internal buffer
SerialPort::~SerialPort() {
    close();
    delete[] buff;
}

SerialPort::SerialPort() {
    buff = new char[buffSz];    //init the buffer
}

//opens a serial port
void SerialPort::open(const std::string& name, unsigned long baudRate, COMMTIMEOUTS timeouts) {

    //VS Code shows error, but it compiles without warnings
    hSerial = CreateFile( name.c_str(), GENERIC_READ | GENERIC_WRITE, 
                                    0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    DCB serialParams = { 0 };
    serialParams.DCBlength = sizeof(serialParams);
    GetCommState(hSerial, &serialParams);

    serialParams.BaudRate = baudRate;   //set the baud rate

    //from pronterface
    serialParams.fBinary = 1;
    serialParams.fParity = 0;
    serialParams.fOutxCtsFlow = 0;
    serialParams.fOutxDsrFlow = 0;
    serialParams.fDtrControl = 1;
    serialParams.fDsrSensitivity = 0;
    serialParams.fTXContinueOnXoff = 0;
    serialParams.fOutX = 0;
    serialParams.fInX = 0;
    serialParams.fErrorChar = 0;
    serialParams.fNull = 0;
    serialParams.fRtsControl = 1;
    serialParams.fAbortOnError = 0;
    serialParams.XonLim = 0;
    serialParams.XoffLim = 0;
    serialParams.ByteSize = 8;
    serialParams.Parity = 0;
    serialParams.StopBits = 0;
    serialParams.XonChar = 87;
    serialParams.XoffChar = 100;
    serialParams.ErrorChar = -3;
    serialParams.EofChar = -96;
    serialParams.EvtChar = 0;

    if(!SetCommState(hSerial, &serialParams)) {
        //TODO: exceptions...
        std::cout << "Error setting state" << std::endl;
    }

    if(!SetCommTimeouts(hSerial, &timeouts)) {
        std::cout << "ERR setting timeouts" << std::endl; 
    }
}


void SerialPort::close() {
    CloseHandle(hSerial);   //closes the port
}

void SerialPort::readErr() {
    //reads the last global error
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

    std::string msg = cmd + "\r";   //add return character to the string
    
    DWORD bytesSent = 0;
    if(!WriteFile(hSerial, msg.c_str(), msg.size(), &bytesSent, NULL)) {
        readErr();
        //TODO: Handle properly
        std::cout << "ERR Send" << std::endl << "\t" << lastErrBuff << std::endl;
    }
}

const std::string& SerialPort::receive() {
    DWORD bytesRead = 0;
    response.clear();
    //get response
    memset(buff, '\0', buffSz);
    //read while can
    while(ReadFile(hSerial, buff, buffSz, &bytesRead, NULL)){
        //error occurred. Report to user.
        if(bytesRead == 0) break;
        response += buff;
        memset(buff, '\0', buffSz);
    }
    
    return response;
}

const std::string& SerialPort::lastResponse() const {
    return response;
}