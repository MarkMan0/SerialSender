#pragma once

#include <boost/thread/mutex.hpp>
#include <queue>
#include <memory>


//wraps a queue<T> for thread-safe access
//TODO:: should we inherit?
template<class T>
class QueueWrapper : protected std::queue<T> {

private:
    boost::mutex mtx;
    using base = typename std::queue<T>;

public:

    using value_type = typename base::value_type;
    using reference =  typename base::reference;
    using const_reference =  typename base::const_reference;
    using size_type = typename base::size_type;

    QueueWrapper() {}


    bool popTo(reference dest) {
        mtx.lock();
        bool result = false;
        if(!base::empty()) {
            dest = base::front();        //copy front to tmp
            base::pop();       //delete front
            result = true;          //success
        }

        mtx.unlock();
        return result;
    }

    bool backTo(reference dest) {
        mtx.lock();
        bool res = false;
        if(!base::empty()) {
            dest = base::back();
            result = true;
        }
        mtx.unlock();
        return result();
    }

    size_type size() {
        mtx.lock();
        size_type sz;
        sz = base::size();
        mtx.unlock();
        return sz;
    }

    bool empty() {
        bool val;
        mtx.lock();
        val = base::empty();
        mtx.unlock();
        return val;
    }

    void push(const_reference val) {
        mtx.lock();
        base::push(val);
        mtx.unlock();
    }
};