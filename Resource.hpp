//
// Created by emdash00 on 8/24/19.
//

#ifndef ARDUINOTHREAD_RESOURCE_HPP
#define ARDUINOTHREAD_RESOURCE_HPP

<template typename T>
class Resource
{

public:
    Resource();
    explicit Resource(T val) :  data(val) { }

    T* Acquire(Thread *thread);
    void Release(Thread *thread, T *_data);

    T data;


protected:
    Thread *owner = nullptr;

};


#endif //ARDUINOTHREAD_RESOURCE_HPP
