#pragma once

#include <windows.h>
#include <string>

//class to handle the port
class SerialPort {
	friend class SerialManager;		//manipulates the port directly

private:
    HANDLE hSerial;       //port reference  
    const size_t buffSz = 50000;    //buffer size for incoming messages
    char *buff;                     //incoming message buffer
    char lastErrBuff[1024];         //to read the last error
    const size_t errBuffSz = 1024; 
    std::string response;           //to stroe the last response

    void readErr();                 //reads the last error

	void send(const std::string& cmd);      //sends a message

	SerialPort();       //just constructs, doesn't open

	void open(const std::string&, unsigned long, COMMTIMEOUTS);     //opens the port
	void close();       //closes the port

public:
	~SerialPort();
};