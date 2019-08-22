//
// Created by emdash00 on 8/22/19.
//

#include "ResourceManager.hpp"

Resource<T> * ResourceManager::Obtain(Thread *thread)
{

    // Give a pointer to the resource if it's unowned.
    if (owner == nullptr){
        owner = thread;
        return &resource;
    }
    else{
        return nullptr;
   }
}

bool ResourceManager::Release(Thread *thread, Resource<T> *&resource)
{
    // Release ownership and destroy the pointer to the resource if the owner releases so they no longer have access.
    // No memory leaks here... resource is a variable on the stack.
    if (owner == thread){
       owner = nullptr;
       resource = nullptr;
       return true;
    }
    else{
        return false;
    }
}


