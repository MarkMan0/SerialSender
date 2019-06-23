#pragma once

#include <windows.h>
#include <string>

//class to handle the port
class SerialPort {
private:
    HANDLE hSerial;       //port reference  
    const size_t buffSz = 50000;    //buffer size for incoming messages
    char *buff;                     //incoming message buffer
    char lastErrBuff[1024];         //to read the last error
    const size_t errBuffSz = 1024; 
    std::string response;           //to stroe the last response

    void readErr();                 //reads the last error

public:
    SerialPort();       //just constructs, doesn't open

    void open(const std::string&, unsigned long, COMMTIMEOUTS);     //opens the port
    void close();       //closes the port

    ~SerialPort();

    void send(const std::string& cmd);      //sends a message
    const std::string& receive();           //reads a message
    const std::string& lastResponse() const;    //returns the last message
};