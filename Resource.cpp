//
// Created by emdash00 on 8/24/19.
//

#include "Resource.hpp"

T * Resource::Acquire(Thread *thread)
{
    if (owner == nullptr)
    {
       owner = thread;
       return data;
    }
    
    return nullptr;
}

void Resource::Release(Thread *thread, T *_data)
{
    if (owner == thread)
    {
        owner = nullptr;
        _data = nullptr;
    }
}

