#include "SerialPort.h"

#include <windows.h>
#include <iostream>
#include <string>

//closes the port and frees the internal buffer
SerialPort::~SerialPort() {
    close();
    delete[] buff;
}

SerialPort::SerialPort() : hSerial(0), buff(new char[buffSz]) {
}

//opens a serial port
void SerialPort::open(const std::string& name, unsigned long baudRate, COMMTIMEOUTS timeouts) {

    //VS Code shows error, but it compiles without warnings
    hSerial = CreateFile( name.c_str(), GENERIC_READ | GENERIC_WRITE, 
                                    0, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);

    DCB serialParams = { 0 };
    serialParams.DCBlength = sizeof(serialParams);
    GetCommState(hSerial, &serialParams);

    serialParams.BaudRate = baudRate;   //set the baud rate

    //from pronterface
    serialParams.fBinary = 1;
    serialParams.fParity = 0;
    serialParams.fOutxCtsFlow = 0;
    serialParams.fOutxDsrFlow = 0;
    serialParams.fDtrControl = DTR_CONTROL_DISABLE;
    serialParams.fDsrSensitivity = 0;
    serialParams.fTXContinueOnXoff = 0;
    serialParams.fOutX = 0;
    serialParams.fInX = 0;
    serialParams.fErrorChar = 0;
    serialParams.fNull = 0;
    serialParams.fRtsControl = RTS_CONTROL_DISABLE;
    serialParams.fAbortOnError = 0;
    serialParams.XonLim = 0;
    serialParams.XoffLim = 0;
    serialParams.ByteSize = 8;
    serialParams.Parity = 0;
    serialParams.StopBits = ONESTOPBIT;
    serialParams.XonChar = 0;
    serialParams.XoffChar = 0;
    serialParams.ErrorChar = 253;
    serialParams.EofChar = 160;
    serialParams.EvtChar = 0;

    if(!SetCommState(hSerial, &serialParams)) {
        //TODO: exceptions...
        std::cout << "Error setting state" << std::endl;
    }

    if(!SetCommTimeouts(hSerial, &timeouts)) {
        std::cout << "ERR setting timeouts" << std::endl; 
    }

	SetCommMask(hSerial, EV_RXCHAR);	//trigger event on every char receive
}


void SerialPort::close() {
    CloseHandle(hSerial);   //closes the port
}

void SerialPort::readErr() {
    //reads the last global error
    memset(lastErrBuff, '\0', errBuffSz);
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL,
                GetLastError(),
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                lastErrBuff,        //compiles w/o warning
                errBuffSz,
                NULL);
}


void SerialPort::send(const std::string& cmd) {
	
	OVERLAPPED osWrite = { 0 };
	DWORD dwWritten;
	DWORD dwRes;
	BOOL fRes;

    std::string msg = cmd + "\r";   //add return character to the string
	osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);


	if (!WriteFile(hSerial, msg.c_str(), msg.size(), &dwWritten, &osWrite)) {
		if (GetLastError() != ERROR_IO_PENDING) {
			// WriteFile failed, but isn't delayed. Report error and abort.
			fRes = FALSE;
			std::cout << "Send fail" << std::endl;
		}
		else {
			// Write is pending.
			dwRes = WaitForSingleObject(osWrite.hEvent, INFINITE);
		}
		switch (dwRes) {
			// OVERLAPPED structure's event has been signaled. 
		case WAIT_OBJECT_0:
			if (!GetOverlappedResult(hSerial, &osWrite, &dwWritten, FALSE)) {
				fRes = FALSE;
				std::cout << "Send fail" << std::endl;
			}
			else {
				// Write operation completed successfully.
				fRes = TRUE;
				std::cout << "Send success" << std::endl;
			}
				
			break;

		default:
			// An error has occurred in WaitForSingleObject.
			// This usually indicates a problem with the
		   // OVERLAPPED structure's event handle.
			fRes = FALSE;
			break;
		}
	}
	else {
		// WriteFile completed immediately.
		fRes = TRUE;
	}

	CloseHandle(osWrite.hEvent);
}

