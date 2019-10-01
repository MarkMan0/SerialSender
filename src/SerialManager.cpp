
#include "SerialManager.h"
#include "SerialPort.h"
#include <windows.h>
#include <queue>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>

#include "MessageHandler.h"

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


	stopThread();
	

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
	if (threadState == ERR_STOP) {
		//thread stopped on error, join it
		t.join();
		threadState = NORM_STOP;
	}
	if (threadState == NORM_STOP) {
		threadState = RUNNING;
		t = std::thread(&SerialManager::readThread, this);
	}
}


void SerialManager::stopThread() {
	if (threadState == RUNNING || threadState == ERR_STOP) {
		t.join();
		threadState = NORM_STOP;
	}
}



void SerialManager::closePort() noexcept {
	port.close();
	stopThread();
}


//sends a message when the port mutex is free
void SerialManager::writeMsg(const std::string& msg) {
	port.send(msg);
}

void SerialManager::readThread() {
	std::string oneLine;
	while (threadState == RUNNING) {
		if (port.open()) {
			try {
				auto msg = port.readOnEvent();

				//print the string char by char
				//indent every line by \t
				if (msg.size() > 0) {	//reponse was not empty
					for (char c : msg) {
						if (c == '\n') {
							auto shared = msgHandler.lock();
							shared->handleResponse(oneLine);
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
				threadState = ERR_STOP;
				//port.close();
				break;
			}
			catch (std::runtime_error& e) {
				std::cerr << "System error occoured:\n\t" << e.what() << std::endl;
				threadState = ERR_STOP;
				//port.close();
				break;
				//TODO: close port
			}
		}
		else {
			threadState = ERR_STOP;
			break;
		}
		
	}

}
