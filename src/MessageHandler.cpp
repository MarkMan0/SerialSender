#include "MessageHandler.h"
#include "..\include\MessageHandler.h"

#include <mutex>

void MessageHandler::sendNow() {

	if (!sendQueue.empty()) {
		mng->writeMsg(sendQueue.top());
		std::lock_guard<std::mutex> lck(queueMtx);	//lock the queue
		sendQueue.pop();
	}
}

void MessageHandler::enqueueSend(const std::string& s, int priority = 1) {
	sendQueue.push(std::move(StrPair(s, priority)));
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
		okFlag = true;
	}
	else if (s.find("") != notFound) {
		//resend request
	}
	else {
		responses.push_back(s);
	}
}



