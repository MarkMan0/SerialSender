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
    void closeSerial();

    void readErr();

public:
    SerialPort();
    ~SerialPort();

    void send(const std::string& cmd);
    const std::string& receive();
    const std::string& lastResponse() const;
};