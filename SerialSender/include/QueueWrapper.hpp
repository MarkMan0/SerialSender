#pragma once

#include <boost/thread/mutex.hpp>
#include <list>
#include <memory>


//wraps a queue<T> for thread-safe access
//TODO:: should we inherit?
template<class T>
class ListWrapper : protected std::list<T> {

private:
    boost::mutex mtx;
    using base = typename std::list<T>;

public:

    using value_type = typename base::value_type;
    using reference =  typename base::reference;
    using const_reference =  typename base::const_reference;
    using size_type = typename base::size_type;

    ListWrapper() {}

    reference front() {
        mtx.lock();
        reference ref = base::front();
        mtx.unlock();
        return ref;
    }

    reference back() {
        mtx.lock();
        reference ref = base::back();
        mtx.unlock();
        return ref;
    }

    void push_front_mtx(const value_type& val) {
        mtx.lock();
        base::push_front(val);
        mtx.unlock();
    }

    void push_back_mtx(const value_type& val) {
        mtx.lock();
        base::push_back(val);
        mtx.unlock();
    }
    


    bool pop_front_to(reference dest) {
        mtx.lock();
        bool result = false;
        if(!base::empty()) {
            dest = base::front();        //copy front to tmp
            base::pop_front();       //delete front
            result = true;          //success
        }

        mtx.unlock();
        return result;
    }

    bool pop_back_to(reference dest) {
        mtx.lock();
        bool result = false;
        if(!base::empty()) {
            dest = base::back();        //copy front to tmp
            base::pop_back();       //delete front
            result = true;          //success
        }

        mtx.unlock();
        return result;
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

    void push_back(const_reference val) {
        mtx.lock();
        base::push_back(val);
        mtx.unlock();
    }
};