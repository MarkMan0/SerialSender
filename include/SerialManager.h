#pragma once

#include <SerialPort.h>
#include <stack>
#include <string>

class SerialManager {
public:
    class SerialOptions {
        //TODO: implement
    };

private:
    SerialPort port;
    typedef std::stack<std::string> MsgCont;
    MsgCont *msgStack;
    void logString(const std::string& );

public:
    SerialManager(const SerialManager&) = delete;       //no copy
    SerialManager& operator=(const SerialManager& ) = delete;   //no assign

    SerialManager(const std::string& name, const long& baud);
    SerialManager(const std::string& name, const SerialOptions& options);

    void closePort();
    bool isOpen();

    void writeMsg(const std::string& );

    void startPeriodicRead();

};