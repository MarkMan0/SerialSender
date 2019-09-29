#pragma once

#include "SerialPort.h"
#include <queue>
#include <string>
#include <mutex>
#include <thread>
#include <list>



class SerialManager {
public:
    class SerialOptions {
        //TODO: implement
    };

private:



    SerialPort port;        //the underlying serial port

	std::string portName;
	unsigned long baud;

	//		THREAD REALTED FUNCTIONS

	void startThread();		//helper functions that start the thread and set a flag
	void stopThread();

    void readThread();		//thread where we are waiting for serial events
    std::thread t;

	bool threadRunning = false;	//used as condition in the thread infinite loop

public:
    SerialManager(const SerialManager&) = delete;       //no copy
    SerialManager& operator=(const SerialManager& ) = delete;   //no assign

	SerialManager() = default;

    //constructos also open the port
    SerialManager(const std::string& _name, unsigned long _baud);
    SerialManager(const std::string& _name, const SerialOptions& _options);

	//TODO: implement move constructor and assignment operator

	~SerialManager();	//closes the port and the thread


	void openPort();
	void openPort(const std::string& name, unsigned long _baud);

    void closePort() noexcept;	//close the port

    bool isOpen() {return port.open(); }	//check if the port is open


    void writeMsg(const std::string& );     //sends a message through the port

};