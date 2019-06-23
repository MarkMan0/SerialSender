#pragma once

#include <windows.h>
#include <string>

class SerialPort {
private:
    HANDLE hSerial;
    const size_t buffSz = 50000;
    char *buff;
    char lastErrBuff[1024];
    const size_t errBuffSz = 1024;
    std::string cmd;
    std::string response;
    void initSerial();

    void readErr();

public:
    SerialPort();

    void open(const std::string&, unsigned long, COMMTIMEOUTS);
    void close();

    ~SerialPort();

    void send(const std::string& cmd);
    const std::string& receive();
    const std::string& lastResponse() const;
};