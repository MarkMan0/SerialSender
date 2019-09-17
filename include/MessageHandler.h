#pragma once
#include <list>
#include <string>
#include <queue>
#include <thread>
#include <shared_mutex>
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
	std::shared_mutex queueMtx;

	std::atomic<bool> okFlag;

	void sendNow();

	std::thread sender;

	std::shared_ptr<SerialManager> mng;

public:
	//adds a higher priority message to the queue
	void enqueueSend(const std::string&, int);

	//emplaces the strings denoted by b and e to the queue, with a default low priority
	template<typename T>
	void enqueueSend(T b, T e, int priority = 0) {
		while (b != e)
			sendQueue.push(std::move(StrPair(*b++, priority)));
	}

	std::string getLastResponse();

	void handleResponse(const std::string&);
};

