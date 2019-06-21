
#include "SerialManager.h"
#include "SerialPort.h"
#include <windows.h>
#include <queue>
#include <string>


SerialManager::SerialManager(const std::string& name, const long& baud) {

    DCB params = {};
    
    params.BaudRate = baud;

    //generic for 3d printers
    params.ByteSize = 8;
    params.StopBits = ONESTOPBIT;
    params.Parity = NOPARITY;
    params.fOutxDsrFlow = TRUE;
    params.fRtsControl = RTS_CONTROL_ENABLE;


    //1 second read timeout
    COMMTIMEOUTS timeout = { 0 };
    timeout.ReadIntervalTimeout = MAXDWORD ;
    timeout.ReadTotalTimeoutConstant = 1000;
    timeout.ReadTotalTimeoutMultiplier = MAXDWORD ;
    timeout.WriteTotalTimeoutConstant = 50;
    timeout.WriteTotalTimeoutMultiplier = 10;

    port.open(name, params, timeout);   //open the port

    open = true;    //TODO: assuming everything works
}


void SerialManager::readPort() {
    if(!isOpen()) return;

    portMtx.lock();

    msgCont.push(port.receive());  //TODO: thread safe?

    portMtx.unlock();
}