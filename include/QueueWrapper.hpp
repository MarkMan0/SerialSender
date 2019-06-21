#pragma once

#include <boost/thread/mutex.hpp>
#include <queue>

template<class T>
class QueueWrapper {

private:
    boost::mutex mtx;
    std::queue<T> *container;

public:
    typedef Container ContType;
    typedef typename T value_type;

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