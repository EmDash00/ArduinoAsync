//
// Created by emdash00 on 9/1/19.
//

#include "SingularThread.hpp"

void SingularThread::runned(unsigned long time)
{
    // Mark thread for removal. No need to call HALT.
    flag = -1;
    super::runned(time);
}