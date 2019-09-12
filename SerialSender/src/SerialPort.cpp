#include "SerialPort.h"

#include <windows.h>
#include <iostream>
#include <string>


SerialPort::SerialPort() : hSerial(0), lastErrBuff({ 0 }) {
}

SerialPort::SerialPort(SerialPort&& old) : hSerial(old.hSerial), isOpen(old.isOpen), lastErrBuff({ 0 }) {
	old.open = false;
}

SerialPort& SerialPort::operator=(SerialPort&& old) noexcept {
	
	if (&old != this) {
		close();	//close this handle

		hSerial = old.hSerial;
		isOpen = old.isOpen;
		old.isOpen = false;	//do not close the handle on moving
	}
	return *this;

}



//closes the port 
SerialPort::~SerialPort() {
	close();
}


//opens a serial port
void SerialPort::open(const std::string& name, unsigned long baudRate, COMMTIMEOUTS timeouts) {

	if (isOpen) {
		close();
	}

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
	isOpen = true;
}


void SerialPort::close() {
	if (isOpen) {
		CloseHandle(hSerial);   //closes the port
		isOpen = false;
	}
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
	
	OVERLAPPED ov = { 0 };


	auto eventCloser = [](OVERLAPPED* ov) {	CloseHandle(ov->hEvent); };

	std::unique_ptr<OVERLAPPED, decltype(eventCloser)> osWrite(&ov, eventCloser);

	DWORD dwWritten = 0;
	DWORD dwRes = 0;
	BOOL fRes = 0;

    std::string msg = cmd + "\r";   //add return character to the string
	osWrite->hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (osWrite->hEvent == NULL) {
		return;
	}

	if (!WriteFile(hSerial, msg.c_str(), msg.size(), &dwWritten, osWrite.get())) {
		if (GetLastError() != ERROR_IO_PENDING) {
			// WriteFile failed, but isn't delayed. Report error and abort.
			fRes = FALSE;
			std::cout << "Send fail" << std::endl;
		}
		else {
			// Write is pending.
			dwRes = WaitForSingleObject(osWrite->hEvent, INFINITE);
		}
		switch (dwRes) {
			// OVERLAPPED structure's event has been signaled. 
		case WAIT_OBJECT_0:
			if (!GetOverlappedResult(hSerial, osWrite.get(), &dwWritten, FALSE)) {
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

}


inline void readAvailable(HANDLE* h, std::string& dest) {

	char chRead = '\0';

	DWORD dwRead = 0;

	OVERLAPPED ov = { 0 };

	bool handleOpen = false;

	auto eventCloser = [&handleOpen](OVERLAPPED* ov) {	if(handleOpen) CloseHandle(ov->hEvent); };

	std::unique_ptr<OVERLAPPED, decltype(eventCloser)> osReader(&ov, eventCloser);

	osReader->hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (osReader->hEvent == NULL) {
		return;
	}
	else {
		handleOpen = true;
	}

	BOOL fWaitingOnRead = FALSE;
	//receive char event occoured
	//reead all the available bytes
	do {
		//begin overlapped reading
		//returns TRUE if reading done immediately, false if error OR waiting
		if (!ReadFile(*h, &chRead, 1, &dwRead, osReader.get())) {
			if (GetLastError() != ERROR_IO_PENDING) {   // read not delayed?
				//Error occoured
				break;
			}
			else {
				//read not done immediately, need to wait
				fWaitingOnRead = TRUE;
			}
		}
		else {
			// read completed immediately
			fWaitingOnRead = FALSE;
			dest += chRead;
		}
		DWORD dwRes;	//the result of the wait

		if (fWaitingOnRead) {		//if we are waiting on a read
			dwRes = WaitForSingleObject(osReader->hEvent, INFINITE);		//wait forever
			switch (dwRes) {
				// Read completed.
			case WAIT_OBJECT_0:
				if (!GetOverlappedResult(*h, osReader.get(), &dwRead, FALSE)) {
					// Error in communications; report it.
					//break;
				}
				else {
					// Read completed successfully.
					dest += chRead;
				}

				//  Reset flag so that another opertion can be issued.
				fWaitingOnRead = FALSE;
				break;

			case WAIT_TIMEOUT:
				//TODO:
				//	count the timeouts, and decide, whether the com port was disconnected

				// Operation isn't complete yet. fWaitingOnRead flag isn't
				// changed since I'll loop back around, and I don't want
				// to issue another read until the first one finishes.
				//
				// This is a good time to do some background work.
				break;

			default:
				// Error in the WaitForSingleObject; abort.
				// This indicates a problem with the OVERLAPPED structure's
				// event handle.
				break;
			}
		}
	} while (dwRead);	//while the bytes read is greater than 0, read
}




//on receive event, reads the port while there is data available, and then returns
//the whole data as std::string
std::string SerialPort::readOnEvent() {
	DWORD dwCommEvent = 0;

	std::string msg("");

	
	if (WaitCommEvent(hSerial, &dwCommEvent, NULL)) {
		readAvailable(&hSerial, msg);
	}
	else {
		// Error in WaitCommEvent
	}
	return msg;
}