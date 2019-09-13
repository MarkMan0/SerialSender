
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

	startThread();

}

SerialManager::~SerialManager() {
	closePort();
	stopThread();
}


void SerialManager::startThread() {
	if (!threadRunning) {
		t = std::thread(&SerialManager::readThread, this);
		threadRunning = true;
	}
}


void SerialManager::stopThread() {
	if (threadRunning) {
		threadRunning = false;
		t.join();
	}
}



void SerialManager::closePort() {
	port.close();
	stopThread();
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
	portMtx.lock();

	port.send(msg);

	portMtx.unlock();
}

void SerialManager::readThread() {
	while (threadRunning) {
		if (port.open()) {
			auto msg = port.readOnEvent();

			//print the string char by char
			//indent every line by \t
			if (msg.size() > 0) {
				std::cout << "Received: \n\t";
				for (char c : msg) {
					std::cout << c;
					if (c == '\n' || c == '\r') {
						std::cout << '\t';
					}
				}
			}
		}
		
	}
	std::cout << "CLOSING THREAD" << std::endl;

}
