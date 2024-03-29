#pragma once
#include <list>
#include <string>
#include <queue>
#include <thread>
#include <mutex>
#include <atomic>
#include <functional>

#include "SerialManager.h"

class MessageHandler {

private:
	std::list<std::string> responses;
	std::list<std::string> statuses;

	typedef std::pair<std::string, int> StrPair;


	std::priority_queue<StrPair, std::deque<StrPair>, std::function<bool(const StrPair&, const StrPair&)> > sendQueue;
	std::mutex queueMtx;
	std::mutex notifyMtx;
	std::condition_variable queueCondVar;

	enum state { RUNNING, NORM_STOP, ERR_STOP, SHUTDOWN };

	void sendNow();

	std::atomic<state> senderState = NORM_STOP;

	std::thread senderThread;

	void sendParallel();
	std::shared_ptr<SerialManager> mng;


	std::atomic<bool> okFlag = false;
	void waitOK();
	std::mutex okMtx;
	std::condition_variable okCondVar;
	
	void runThread();

	void stopThread();

public:

	MessageHandler(std::shared_ptr<SerialManager> _mng) : mng(_mng), sendQueue([](const StrPair& l, const StrPair& r)->bool {
		return l.second < r.second; }) {
		runThread();
	}
	MessageHandler(const MessageHandler& old) = delete;	//no copy
	MessageHandler(MessageHandler&& old) noexcept;	//TODO: implement move constructor
	MessageHandler& operator=(const MessageHandler& rhs) = delete; //no assignment
	MessageHandler& operator=(MessageHandler&& rhs);	//TODO: implement move assignment
	~MessageHandler() {
		try {
			stopThread();
		}
		catch (...) {}
	}

	//adds a higher priority message to the queue
	void enqueueSend(const std::string&, int priority = 1);

	//emplaces the strings denoted by b and e to the queue, with a default low priority
	template<typename T>
	void enqueueSend(T b, T e, int priority = 0) {

		if (b == e)	return;	//check for empty range, to avoid unnecessary mutex locks

		if (senderState != RUNNING) {
			runThread();	//if the thread is not yet running, start it
		}
		std::lock_guard<std::mutex> lck(queueMtx);	//lock the queue

		std::lock_guard<std::mutex> notifLck(notifyMtx); //lock the condition mutex
		while (b != e)
			sendQueue.emplace(*b++, priority);	//perform modification

		if (senderState != SHUTDOWN && senderState != RUNNING) {
			runThread();
		}

		queueCondVar.notify_all();
	}

	std::string getLastResponse();

	void handleResponse(const std::string&);

	void clearQueue();

	//TODO: implement
	void pauseSend();
	void resumeSend();
};

