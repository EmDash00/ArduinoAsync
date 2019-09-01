//
// Created by emdash00 on 9/1/19.
//

#include "Resource.hpp"

bool Resource::Acquire(Thread *thread, int priority)
{
    if (owner == nullptr || priority > owner_priority)
    {
        owner = thread;
        return true;
    }
    else
    {
        return false;
    }
}

bool Resource::HaveOwnership(Thread *thread){
    return (owner == thread);
}


void Resource::Release(Thread *thread)
{
    if (owner == thread)
    {
        owner = nullptr;
    }
}