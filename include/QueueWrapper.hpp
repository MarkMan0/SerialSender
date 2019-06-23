#pragma once

#include <boost/thread/mutex.hpp>
#include <queue>
#include <memory>


//wraps a queue<T> for thread-safe access
//TODO:: should we inherit?
template<class T>
class QueueWrapper : public std::queue<T> {

private:
    boost::mutex mtx;
    typedef typename std::queue<T>::value_type          v_type;
    typedef typename std::queue<T>::reference           ref_type;
    typedef typename std::queue<T>::const_reference     const_ref_type;
    typedef std::queue<T>               base;

public:

    QueueWrapper() {}


    bool popTo(ref_type dest) {
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

    void push(const_ref_type val) {
        mtx.lock();
        base::push(val);
        mtx.unlock();
    }
};