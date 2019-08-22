//
// Created by emdash00 on 8/22/19.
//

#ifndef ARDUINOTHREAD_RESOURCE_HPP
#define ARDUINOTHREAD_RESOURCE_HPP

<template typename T>
class Resource{
public:
    Resource();
    explicit Resource(T val) : data(val) { }
    T Get();
    void Set(T val);

protected:
    T data;

};


#endif //ARDUINOTHREAD_RESOURCE_HPP
