
#include "SerialManager.h"
#include "SerialPort.h"
#include <windows.h>
#include <queue>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>


SerialManager::SerialManager(const std::string& name, const long& baud) {

    //defines how much to wait for a message
    COMMTIMEOUTS timeout = { 0 };
    timeout.ReadIntervalTimeout = MAXDWORD ;
    timeout.ReadTotalTimeoutConstant = 0;
    timeout.ReadTotalTimeoutMultiplier = 0 ;
    timeout.WriteTotalTimeoutConstant = 0;
    timeout.WriteTotalTimeoutMultiplier = 0;

    port.open(name, baud, timeout);   //open the port

    open = true;    //TODO: assuming everything works
}


void SerialManager::readPort() {
    if(!isOpen()) return;       //port no open, nothing to do

    portMtx.lock(); //TODO:: called periodically, need to wait???
    std::string msg = "";
    if(msg.size() > 1)
        msgCont.push_back_mtx(msg);  //read a message and push to the queue

    portMtx.unlock();       //unclock the mutex
}


void SerialManager::closePort() {
    port.close();
}

std::string SerialManager::lastMsg() {
    std::string msg;
    msgCont.pop_back_to(msg);
    return msg;
}

std::string SerialManager::nextMsg() {
    std::string tmp;
    msgCont.pop_front_to(tmp);     //pops and returns the next message from the queue
    return tmp;
}

//sends a message when the port mutex is free
void SerialManager::writeMsg(const std::string& msg) {
    if(!isOpen()) return;
    portMtx.lock();

    port.send(msg);

    portMtx.unlock();
}

void SerialManager::readThread(unsigned long ms) {

	DWORD dwCommEvent;
	char  chRead;

	DWORD dwRead;


	while(1) {
		if (WaitCommEvent(port.hSerial, &dwCommEvent, NULL)) {
			//receive char event occoured
			do {
				if (ReadFile(port.hSerial, &chRead, 1, &dwRead, NULL)) {
					// A byte has been read; process it.
					std::cout << chRead;
				}
				else {
					// An error occurred in the ReadFile call.
					break;
				}
			} while (dwRead);	//while the bytes read is greater than 0, read
		}
		else {
			// Error in WaitCommEvent
			break;
		}
	}

}


void SerialManager::startPeriodicRead(unsigned long ms) {
    t = std::thread(&SerialManager::readThread, this, ms);
    //t.join();
}