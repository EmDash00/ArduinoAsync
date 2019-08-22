//
// Created by emdash00 on 8/22/19.
//

#ifndef ARDUINOTHREAD_RESOURCEMANAGER_HPP
#define ARDUINOTHREAD_RESOURCEMANAGER_HPP

#include "Resource.hpp"
#include "Thread.h"

<template typename T>
class ResourceManager{
public:
    ResourceManager();
    explicit ResourceManager(T val) : resource(val) { };

    Resource<T> * Obtain(Thread *thread);
    bool Release(Thread *thread, Resource<T> *&resource);

protected:
    Resource<T> resource;
    Thread owner;
};


#endif //ARDUINOTHREAD_RESOURCEMANAGER_HPP
