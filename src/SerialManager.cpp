
#include "SerialManager.h"
#include "SerialPort.h"
#include <windows.h>
#include <queue>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>

SerialManager::SerialManager(const std::string& _name, unsigned long _baud) : portName(_name), baud(_baud) {
	openPort();
}


void SerialManager::openPort() {
	if (portName.size() > 0) {
		openPort(portName, baud);
	}
}

//opens the serial port specified in its parameters
//if the parameters have the same port, closes than reopens it
void SerialManager::openPort(const std::string& _name, unsigned long _baud) {

	if (port.open()) {
		port.close();
	}

	if (threadRunning) {
		stopThread();
	}

	portName = _name;
	baud = _baud;

	//return from readFile immediately, even if no data was read
	//must for non-blocking IO
	COMMTIMEOUTS timeout = { 0 };
	timeout.ReadIntervalTimeout = MAXDWORD;
	timeout.ReadTotalTimeoutConstant = 0;
	timeout.ReadTotalTimeoutMultiplier = 0;
	timeout.WriteTotalTimeoutConstant = 0;
	timeout.WriteTotalTimeoutMultiplier = 0;

	port.open(portName, baud, timeout);   //open the port

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
	port.send(msg);
}

void SerialManager::readThread() {
	while (threadRunning) {
		if (port.open()) {
			auto msg = port.readOnEvent();

			//print the string char by char
			//indent every line by \t
			if (msg.size() > 0) {	//reponse was not empty
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
