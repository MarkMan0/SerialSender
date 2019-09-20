#pragma once
#include <list>
#include <string>
#include <queue>
#include <thread>
#include <mutex>
#include <atomic>

#include "SerialManager.h"

class MessageHandler {

private:
	std::list<std::string> responses;
	std::list<std::string> statuses;

	struct StrPair
	{
		StrPair(const std::string& _str, int p = 0) : str(_str), priority(p) {}

		StrPair(StrPair&& old) = default;
		StrPair& operator=(StrPair&& old) = default;

		std::string str;
		int priority;

		bool operator<(const StrPair& s2) const {
			return this->priority < s2.priority;
		}

		operator const std::string&() const {
			return str;
		}
	};

	std::priority_queue<StrPair, std::deque<StrPair> > sendQueue;
	std::mutex queueMtx;
	std::mutex notifyMtx;
	std::condition_variable queueCondVar;

	enum state { HAS_WORK, NO_WORK, SHUTDOWN };

	void sendNow();

	state senderState = NO_WORK;

	std::thread senderThread;
	std::atomic<bool> threadRunning;

	void sendParallel();
	std::shared_ptr<SerialManager> mng;


	bool okFlag = false;
	void waitOK();
	std::mutex okMtx;
	std::condition_variable okCondVar;

	
	void runThread();

	void stopThread();

public:

	MessageHandler(std::shared_ptr<SerialManager> _mng) : mng(_mng) {
		runThread();
	}
	MessageHandler(const MessageHandler& old) = delete;	//no copy
	MessageHandler(const MessageHandler&& old);	//TODO: implement move constructor
	MessageHandler& operator=(const MessageHandler& rhs) = delete; //no assignment
	MessageHandler& operator=(MessageHandler&& rhs);	//TODO: implement move assignment
	~MessageHandler() {
		try {
			//stopThread();
		}
		catch (...) {}
	}

	//adds a higher priority message to the queue
	void enqueueSend(const std::string&, int);

	//emplaces the strings denoted by b and e to the queue, with a default low priority
	template<typename T>
	void enqueueSend(T b, T e, int priority = 0) {
		std::lock_guard<std::mutex> lck(queueMtx);	//lock the queue

		std::lock_guard<std::mutex> notifLck(notifyMtx); //lock the condition mutex
		while (b != e)
			sendQueue.emplace(*b++, priority);	//perform modification

		if (senderState != SHUTDOWN) {
			senderState = HAS_WORK;
		}

		queueCondVar.notify_all();
	}

	std::string getLastResponse();

	void handleResponse(const std::string&);

	
};

