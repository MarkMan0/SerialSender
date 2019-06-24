
#include "SerialManager.h"
#include "SerialPort.h"
#include <windows.h>
#include <queue>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>


SerialManager::SerialManager(const std::string& name, const long& baud) {

    //defines how much to wait for a message
    COMMTIMEOUTS timeout = { 0 };
    timeout.ReadIntervalTimeout = MAXDWORD ;
    timeout.ReadTotalTimeoutConstant = 100;
    timeout.ReadTotalTimeoutMultiplier = MAXDWORD ;
    timeout.WriteTotalTimeoutConstant = 50;
    timeout.WriteTotalTimeoutMultiplier = 10;

    port.open(name, baud, timeout);   //open the port

    open = true;    //TODO: assuming everything works
}


void SerialManager::readPort() {
    if(!isOpen()) return;       //port no open, nothing to do

    portMtx.lock(); //TODO:: called periodically, need to wait???
    std::string msg = port.receive();
    if(msg.size() > 1)
        msgCont.push_back_mtx(msg);  //read a message and push to the queue

    portMtx.unlock();       //unclock the mutex
}


void SerialManager::closePort() {
    port.close();
}

std::string SerialManager::lastMsg() {
    std::string msg;
    msgCont.pop_back_to(msg);
    return msg;
}

std::string SerialManager::nextMsg() {
    std::string tmp;
    msgCont.pop_front_to(tmp);     //pops and returns the next message from the queue
    return tmp;
}

//sends a message when the port mutex is free
void SerialManager::writeMsg(const std::string& msg) {
    if(!isOpen()) return;
    portMtx.lock();

    port.send(msg);

    portMtx.unlock();
}

void SerialManager::readThread(unsigned long ms) {
    while(1) {
        this->readPort();
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }
}


void SerialManager::startPeriodicRead(unsigned long ms) {
    t = std::thread(&SerialManager::readThread, this, ms);
    //t.join();
}