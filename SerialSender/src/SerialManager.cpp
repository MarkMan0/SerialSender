
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
	timeout.ReadIntervalTimeout = MAXDWORD;
	timeout.ReadTotalTimeoutConstant = 0;
	timeout.ReadTotalTimeoutMultiplier = 0;
	timeout.WriteTotalTimeoutConstant = 0;
	timeout.WriteTotalTimeoutMultiplier = 0;

	port.open(name, baud, timeout);   //open the port

	open = true;    //TODO: assuming everything works
}


void SerialManager::readPort() {
	if (!isOpen()) return;       //port no open, nothing to do

	portMtx.lock(); //TODO:: called periodically, need to wait???
	std::string msg = "";
	if (msg.size() > 1)
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
	if (!isOpen()) return;
	portMtx.lock();

	port.send(msg);

	portMtx.unlock();
}

void SerialManager::readThread(unsigned long ms) {

	DWORD dwCommEvent;
	DWORD dwRead;
	char  chRead;


	OVERLAPPED osReader = { 0 };
	BOOL fWaitingOnRead = FALSE;
	osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	while (1) {
		if (WaitCommEvent(port.hSerial, &dwCommEvent, NULL)) {
			//receive char event occoured
			//reead all the available bytes
			do {
				//begin overlapped reading
				//returns TRUE if reading done immediately, false if error OR waiting
				if (!ReadFile(port.hSerial, &chRead, 1, &dwRead, &osReader)) {
					if (GetLastError() != ERROR_IO_PENDING) {   // read not delayed?
						//Error occoured
						//break;
					}
					else {
						//read not done immediately, need to wait
						fWaitingOnRead = TRUE;
					}
				}
				else {
					// read completed immediately
					fWaitingOnRead = FALSE;
					std::cout << chRead;
				}
				DWORD dwRes;	//the result of the wait

				if (fWaitingOnRead) {		//if we are waiting on a read
					dwRes = WaitForSingleObject(osReader.hEvent, INFINITE);		//wait forever
					switch (dwRes) {
						// Read completed.
					case WAIT_OBJECT_0:
						if (!GetOverlappedResult(port.hSerial, &osReader, &dwRead, FALSE)) {
							// Error in communications; report it.
							//break;
						}
						else {
							// Read completed successfully.
							std::cout << chRead;
						}

						//  Reset flag so that another opertion can be issued.
						fWaitingOnRead = FALSE;
						break;

					case WAIT_TIMEOUT:
						5 + 5;
						// Operation isn't complete yet. fWaitingOnRead flag isn't
						// changed since I'll loop back around, and I don't want
						// to issue another read until the first one finishes.
						//
						// This is a good time to do some background work.
						//break;

					default:
						5 + 5;
						// Error in the WaitForSingleObject; abort.
						// This indicates a problem with the OVERLAPPED structure's
						// event handle.
						//break;
					}
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