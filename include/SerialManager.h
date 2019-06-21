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
    void openPort(const std::string& name, const long& baud);
    void openPort(const std::string& name, const SerialOptions& options);

    void closePort();
    bool isOpen();

    void writeMsg(const std::string& );

    void startPeriodicRead();

};