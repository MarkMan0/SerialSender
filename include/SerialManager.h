#pragma once

#include <SerialPort.h>
#include <queue>
#include <string>
#include <boost/thread/mutex.hpp>
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

    boost::mutex portMtx;       //mutex for blocking the serial port

    bool open = false;      //is the port open?

    void readThread(unsigned long);

    std::thread t;

public:
    SerialManager(const SerialManager&) = delete;       //no copy
    SerialManager& operator=(const SerialManager& ) = delete;   //no assign

    //constructos also open the port
    SerialManager(const std::string& name, const long& baud);
    SerialManager(const std::string& name, const SerialOptions& options);

    void closePort();
    bool isOpen() {return open; }

    void readPort();

    std::string nextMsg();      //gets the latest message
    std::string lastMsg();

    void writeMsg(const std::string& );     //sends a message through the port

    void startPeriodicRead(unsigned long ms);           //starts a periodic reading on the port

};