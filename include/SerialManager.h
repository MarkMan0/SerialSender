#pragma once

#include <SerialPort.h>
#include <queue>
#include <string>
#include <boost/thread/mutex.hpp>
#include "QueueWrapper.hpp"

class SerialManager {
public:
    class SerialOptions {
        //TODO: implement
    };

private:
    SerialPort port;        //the underlying serial port
    typedef QueueWrapper<std::string> MsgCont;  
    MsgCont msgCont;        //read messages go here

    boost::mutex portMtx;       //mutex for blocking the serial port

    bool open = false;      //is the port open?

public:
    SerialManager(const SerialManager&) = delete;       //no copy
    SerialManager& operator=(const SerialManager& ) = delete;   //no assign

    //constructos also open the port
    SerialManager(const std::string& name, const long& baud);
    SerialManager(const std::string& name, const SerialOptions& options);

    void closePort();
    bool isOpen() {return open; }

    void readPort();

    std::string lastMsg();      //gets the latest message

    void writeMsg(const std::string& );     //sends a message through the port

    void startPeriodicRead();           //starts a periodic reading on the port

};