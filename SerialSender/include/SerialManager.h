#pragma once

#include <SerialPort.h>
#include <queue>
#include <string>
#include <mutex>
#include <thread>
#include "QueueWrapper.hpp"

class SerialManager {
public:
    class SerialOptions {
        //TODO: implement
    };

private:
    SerialPort port;        //the underlying serial port
    typedef ListWrapper<std::string> MsgCont;  
    MsgCont msgCont;        //read messages go here

    std::mutex portMtx;       //mutex for blocking the serial port

    void readThread();

    std::thread t;
	bool threadRunning = false;

public:
    SerialManager(const SerialManager&) = delete;       //no copy
    SerialManager& operator=(const SerialManager& ) = delete;   //no assign

    //constructos also open the port
    SerialManager(const std::string& name, const long& baud);
    SerialManager(const std::string& name, const SerialOptions& options);

	~SerialManager();

    void closePort();
    bool isOpen() {return port.open(); }

    void readPort();

    std::string nextMsg();      //gets the latest message
    std::string lastMsg();

    void writeMsg(const std::string& );     //sends a message through the port

};