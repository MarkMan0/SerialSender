#pragma once

#include <windows.h>
#include <string>

//class to handle the port
class SerialPort {
	friend class SerialManager;		//manipulates the port directly

private:


	SerialPort();       //just constructs, doesn't open
	SerialPort(const SerialPort&) = delete;		//copy makes no sense for a serialport
	SerialPort(SerialPort&&) noexcept;
	SerialPort& operator=(const SerialPort&) = delete;
	SerialPort& operator=(SerialPort&&) noexcept;
	~SerialPort();


    HANDLE hSerial;       //port reference  
    char lastErrBuff[1024];         //to read the last error
    const size_t errBuffSz = 1024; 

	bool isOpen = false;

    void readErr();                 //reads the last error

	void send(const std::string& cmd);      //sends a message


	void open(const std::string&, unsigned long, COMMTIMEOUTS);     //opens the port
	bool open() { return isOpen; }
	void close();       //closes the port

	std::string readOnEvent();
};