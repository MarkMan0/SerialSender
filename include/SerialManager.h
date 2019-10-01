#pragma once

#include "SerialPort.h"
#include <queue>
#include <string>
#include <mutex>
#include <thread>
#include <list>
#include <memory>
#include <atomic>


class MessageHandler;

class SerialManager {
public:
    class SerialOptions {
        //TODO: implement
    };

private:

	enum ThreadStates {RUNNING, NORM_STOP, ERR_STOP};

	std::weak_ptr<MessageHandler> msgHandler;

    SerialPort port;        //the underlying serial port

	std::string portName;
	unsigned long baud;

	//		THREAD REALTED FUNCTIONS

	void startThread();		//helper functions that start the thread and set a flag
	void stopThread();

    void readThread();		//thread where we are waiting for serial events
    std::thread t;

	std::atomic<ThreadStates> threadState = NORM_STOP;	//used as condition in the thread infinite loop

public:
    SerialManager(const SerialManager&) = delete;       //no copy
    SerialManager& operator=(const SerialManager& ) = delete;   //no assign

	SerialManager() = default;

	//assing a handler to the shared_ptr
	void setHandler(const std::shared_ptr<MessageHandler>& h) {
		msgHandler = h;
	}

    //constructos also open the port
    SerialManager(const std::string& _name, unsigned long _baud);
    SerialManager(const std::string& _name, const SerialOptions& _options);

	//TODO: implement move constructor and assignment operator

	~SerialManager();	//closes the port and the thread


	void openPort();
	void openPort(const std::string& name, unsigned long _baud);

    void closePort() noexcept;	//close the port

    bool isOpen() const {return port.open(); }	//check if the port is open


    std::string nextMsg();      //gets the latest message
    std::string lastMsg();

    void writeMsg(const std::string& );     //sends a message through the port

};