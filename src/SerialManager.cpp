
#include "SerialManager.h"
#include "SerialPort.h"
#include <windows.h>
#include <queue>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>

#include "exceptions/serial_io_error.h"

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
	if(msgCont.size() > 0)
		return msgCont.front();

	return "";
}

std::string SerialManager::nextMsg() {
	if (msgCont.size() > 0)
		return msgCont.back();
	return "";
}

//sends a message when the port mutex is free
void SerialManager::writeMsg(const std::string& msg) {
	port.send(msg);
}

void SerialManager::readThread() {
	std::string oneLine;
	while (threadRunning) {
		if (port.open()) {
			try {
				auto msg = port.readOnEvent();

				//print the string char by char
				//indent every line by \t
				if (msg.size() > 0) {	//reponse was not empty
					for (char c : msg) {
						if (c == '\n') {
							msgCont.push_front(oneLine);
							std::cout << oneLine << std::endl;
							oneLine.clear();
						}
						else {
							oneLine += c;
						}
					}
				}
			} 
			catch (serial_io_error& e) {
				std::cerr << "Serial error occoured:\n\t" << e.what() << std::endl;
			}
			catch (std::runtime_error& e) {
				std::cerr << "System error occoured:\n\t" << e.what() << std::endl;
				//TODO: close port
			}
		}
		
	}

}
