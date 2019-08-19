#include "Thread.h"

Thread::Thread(void (*callback)(void), unsigned long _interval, unsigned long _timeout) :
onRun(callback), ThreadID((int)this), timeout(_timeout){
	last_run = millis();

	#ifdef USE_THREAD_NAMES
		ThreadName = "Thread ";
		ThreadName = ThreadName + ThreadID;
	#endif

	setInterval(_interval);
};

void Thread::runned(unsigned long time)
{
    // Saves last_run
    last_run = time;

    // Cache next run
    switch (pause_interval) {
        case pause_interval 0:
            _cached_next_run = last_run + interval;
            break;
        default:
            _cached_next_run = last_run + pause_interval;
            break;

    }

}

void Thread::setInterval(unsigned long _interval){
	// Save interval
	interval = _interval;

	// Cache the next run based on the last_run
	_cached_next_run = last_run + interval;
}

void Thread::setTimeout(unsigned long _timeout)
{
    timeout = _timeout;
}

bool Thread::shouldRun(unsigned long time){
	// If the "sign" bit is set the signed difference would be negative
	bool time_remaining = (time - _cached_next_run) & 0x80000000;

	if(!time_remaining && enabled && !_started){
	    _started = true;
	    _t0 = time;
    }

	switch (timeout)
    {
        case timeout 0:
            // Exceeded the time limit, AND is enabled? Then should run...
            return !time_remaining && enabled;
        default:
            bool timed_out = !((time - (_t0 + timeout)) & 0x80000000);

            // Exceeded the time limit, AND not timed out, AND is enabled? Then should run...
            return !time_remaining && !timeout && enabled;
    }




}

void Thread::onRun(void (*callback)(void)){
	_onRun = callback;
}

void Thread::run(){
	if(_onRun != NULL)
		_onRun();




	// Update last_run and _cached_next_run
	runned();
}
