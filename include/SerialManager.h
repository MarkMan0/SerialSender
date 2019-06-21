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
    SerialPort port;
    typedef QueueWrapper<std::string> MsgCont;
    MsgCont msgCont;

    boost::mutex portMtx;

    bool open = false;

public:
    SerialManager(const SerialManager&) = delete;       //no copy
    SerialManager& operator=(const SerialManager& ) = delete;   //no assign

    SerialManager(const std::string& name, const long& baud);
    SerialManager(const std::string& name, const SerialOptions& options);

    void closePort();
    bool isOpen() {return open; }

    void readPort();

    void writeMsg(const std::string& );

    void startPeriodicRead();

};