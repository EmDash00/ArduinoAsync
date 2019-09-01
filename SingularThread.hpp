//
// Created by emdash00 on 9/1/19.
//

#ifndef ARDUINOTHREAD_SINGULARTHREAD_HPP
#define ARDUINOTHREAD_SINGULARTHREAD_HPP

#include "Thread.h"

class SingularThread : public Thread
{
public:
    void runned(unsigned long time);

private:
    typedef Thread super;
};


#endif //ARDUINOTHREAD_SINGULARTHREAD_HPP
