#include "MessageHandler.h"
#include "..\include\MessageHandler.h"

#include <mutex>

void MessageHandler::sendNow() {

	mng->writeMsg(sendQueue.top());
	std::lock_guard<std::mutex> lck(queueMtx);	//lock the queue
	sendQueue.pop();
}

void MessageHandler::sendParallel() {

	while (threadRunning) {
		if (sendQueue.empty()) {
			//wait for condition variable to change, becasue there are no messages
			std::unique_lock<std::mutex> lck(notifyMtx);
			queueCondVar.wait(lck, [&]()->bool { 
				if (!sendQueue.empty() || senderState == SHUTDOWN)
					return true;
				return false;
			});
		}
		if (senderState == SHUTDOWN)
			return;
		sendNow();
		waitOK();
	}	//TODO: create exit condition
}


void MessageHandler::waitOK() {
	std::unique_lock<std::mutex> lck(okMtx);
	if (!okFlag || okState == SHUTDOWN) {
		okCondVar.wait(lck, [&]()->bool { return okState == SHUTDOWN || (okState == WORK && okFlag); });
	}

	okFlag = false;
}

void MessageHandler::runThread() {
	threadRunning = true;
	senderThread = std::thread(&MessageHandler::sendParallel, this);
}

void MessageHandler::stopThread() {
	{
		std::lock_guard<std::mutex> lck(notifyMtx);
		senderState = SHUTDOWN;
		queueCondVar.notify_all();
	}
	threadRunning = false;
	senderThread.join();
}

void MessageHandler::enqueueSend(const std::string& s, int priority) {
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



