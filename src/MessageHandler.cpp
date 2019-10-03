#include "MessageHandler.h"
#include "..\include\MessageHandler.h"

#include <mutex>
#include <iostream>
#include "exceptions\serial_io_error.h"

void MessageHandler::sendNow() {

	mng->writeMsg(sendQueue.top());
	std::lock_guard<std::mutex> lck(queueMtx);	//lock the queue
	sendQueue.pop();
}

void MessageHandler::sendParallel() {

	while (senderState == RUNNING) {
		if (sendQueue.empty()) {
			//wait for condition variable to change, becasue there are no messages
			std::unique_lock<std::mutex> lck(notifyMtx);
			queueCondVar.wait(lck, [&]()->bool { 
				if (!sendQueue.empty() || senderState == NORM_STOP || senderState == SHUTDOWN)
					return true;
				return false;
			});
		}
		if (senderState == NORM_STOP || senderState == SHUTDOWN)
			break;
		try {
			sendNow();
			waitOK();
		}
		catch (serial_io_error& e) {
			std::cerr << "Send failed: \n\t" << e.what() << std::endl;
			senderState = ERR_STOP;
			break;
		}
	}
}


void MessageHandler::waitOK() {
	std::unique_lock<std::mutex> lck(okMtx);
	if (!okFlag) {
		okCondVar.wait(lck, [&]()->bool {
			if (okFlag) return true;	//there was an OK
			else if (senderState == SHUTDOWN || senderState == NORM_STOP)	return true;	//thread needs to be shut down
			else return false;	//keep waiting
			});
	}

	okFlag = false;
}

void MessageHandler::runThread() {
	stopThread();
	senderState = RUNNING;
	senderThread = std::thread(&MessageHandler::sendParallel, this);
}

void MessageHandler::stopThread() {
	if (senderState == RUNNING || senderState == ERR_STOP) {
		{	//scoped so condition variable mutexes are released
			std::lock_guard<std::mutex> lck(notifyMtx);
			senderState = SHUTDOWN;
			queueCondVar.notify_all();	//this gets it out from waiting on a queue insert
			okCondVar.notify_all();		//if waiting on ok, notify with shutdown
		}
		senderThread.join();		//wait for the thread to finish
		senderState = SHUTDOWN;		//if there was an error, consider it handled, since the thread is no longer running
	}
}

void MessageHandler::enqueueSend(const std::string& s, int priority) {

	if (senderState != SHUTDOWN && senderState != RUNNING) {	//if not running, and not stopped intentionally
		runThread();	//if the thread is not yet running, start it
	}

	std::lock_guard<std::mutex> lck(queueMtx);	//lock the queue
	std::lock_guard<std::mutex> notifLck(notifyMtx); //lock the condition mutex
	
	sendQueue.emplace(s, priority);
	queueCondVar.notify_all();
}


std::string MessageHandler::getLastResponse() {
	std::string resp(responses.back());
	return std::string();
}

void MessageHandler::handleResponse(const std::string& s) {
	/*there are four types of messages:
		1.: echo messages - these should be printed to the user
		2.: temp reports  - save internally, print only on request
		3.: position reports - send to user
		4.: ok - set flag only
		5.: resend requests - resend the requested line to the printer
	*/

	const auto notFound = std::string::npos;

	if (s.find("echo") != notFound) {
		//echo response
		responses.push_back(s);
	}
	else if(s.find(" T:") != notFound) {
		//temp report
	}
	else if(s.find("X:") != notFound) {
		//position report
	}
	else if(s.find("ok") != notFound) {
		//ok received
		std::lock_guard<std::mutex> lck(okMtx);
		okFlag = true;
		okCondVar.notify_all();
	}
	else if (s.find("") != notFound) {
		//resend request
	}
	else {
		responses.push_back(s);
	}
}

void MessageHandler::clearQueue() {
	std::unique_lock<std::mutex> lck(queueMtx);
	while (!sendQueue.empty())
	{
		sendQueue.pop();
	}
}

void MessageHandler::pauseSend() {
	stopThread();
}

void MessageHandler::resumeSend() {
	runThread();
}



