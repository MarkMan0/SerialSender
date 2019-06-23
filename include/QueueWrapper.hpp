#pragma once

#include <boost/thread/mutex.hpp>
#include <queue>
#include <memory>


//wraps a queue<T> for thread-safe access
//TODO:: should we inherit?
template<class T>
class QueueWrapper {

private:
    boost::mutex mtx;
    std::unique_ptr<std::queue<T> > container;

public:

    QueueWrapper() : container(new std::queue<T>) {}

    typedef T value_type;

    bool popTo(value_type& dest) {
        mtx.lock();
        bool result = false;
        if(!container->empty()) {
            dest = container->front();        //copy front to tmp
            container->pop();       //delete front
            result = true;          //success
        }

        mtx.unlock();
        return result;
    }

    void push(const value_type& val) {
        mtx.lock();
        container->push(val);
        mtx.unlock();
    }
};