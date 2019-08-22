//
// Created by emdash00 on 8/22/19.
//

#include "ResourceManager.hpp"

Resource<T> * ResourceManager::Obtain(Thread *thread)
{
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
    if (owner == thread){
       owner = nullptr;
       resource = nullptr;
       return true;
    }
    else{
        return false;
    }
}


