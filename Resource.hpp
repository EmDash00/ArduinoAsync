//
// Created by emdash00 on 8/24/19.
//

#ifndef ARDUINOTHREAD_RESOURCE_HPP
#define ARDUINOTHREAD_RESOURCE_HPP

#include "Thread.h"

class Resource
{

public:
    bool Acquire(Thread *thread, int priority);
    bool HaveOwnership(Thread *thread);
    void Release(Thread *thread);

protected:
    Resource( const Resource& ) = delete; // non construction-copyable
    Resource& operator=( const Resource& ) = delete; // non copyable

    Thread *owner = nullptr;
    int owner_priority = -1;
};


#endif //ARDUINOTHREAD_RESOURCE_HPP
