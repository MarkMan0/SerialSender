#include "MessageHandler.h"
#include "..\include\MessageHandler.h"

void MessageHandler::enqueueSend(const std::string& s, int priority = 1) {
	sendQueue.push(std::move(StrPair(s, priority)));
}


void MessageHandler::handleResponse(const std::string&) {
	/*there are four types of messages:
		1.: echo messages - these should be printed to the user
		2.: temp reports  - save internally, print only on request
		3.: position reports - send to user
		4.: ok - set flag only
		5.: resend requests - resend the requested line to the printer

	*/


}
