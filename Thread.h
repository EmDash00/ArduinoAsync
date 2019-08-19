/*
 	Thread.h - An runnable object

	Thread is responsable for holding the "action" for something,
	also, it responds if it "should" or "should not" run, based on
	the current time;

	For instructions, go to https://github.com/ivanseidel/ArduinoThread

	Created by Ivan Seidel Gomes, March, 2013.
	Released into the public domain.
*/

#ifndef Thread_h
#define Thread_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

#include <inttypes.h>

// Yields execution to the main thread, setting a flag that represents the thread's state.
#define YIELD(...) GET_MACRO(__VA_ARGS__, YIELD_CUSTOM, YIELD_DEFAULT)(__VA_ARGS__)

// Called when YIELD is called with a custom flag.
#define YIELD_CUSTOM(_flag) (flag = _flag; return)

// Called when YIELD is called without a custom flag. Sets default flag 0.
#define YIELD_DEFAULT (YIELD_CUSTOM(0))

// Links the specified thread and YIELDs. The linked thread is called immediately from the main loop.
#define YIELD_TO(_thread, _flag) (_linked_thread = thread, YIELD(_flag))

// Gets a pointer to the eventual result of a thread's work of type 'type'.
// Warning, if the thread doesn't have a result, the AWAIT will never resolve
#define AWAIT(type, _thread, _flag) (_thread->result ? (type)_thread->result : YIELD_TO(_thread, _flag))

// Suspends the thread's execution for pause_interval ms.
#define PAUSE(_pause_interval, _flag) (pause_interval = _pause_interval, YIELD(_flag))

// YIELDs with flag -1. The ThreadController will remove any thread with flag -1 after calling run()
#define HALT(_finished = true, YIELD(-1))

/*
	Uncomment this line to enable ThreadName Strings.

	It might be usefull if you are loging thread with Serial,
	or displaying a list of threads...
*/
// #define USE_THREAD_NAMES	1


class Thread{
protected:
	// Desired interval between runs
	unsigned long interval;

	// Desired interval to pause a thread.
	// shouldRun will return false until this interval is over, upon which the thread is immediately run.
	unsigned long pause_interval = 0;

	// If time exceeds timeout (ms), shouldRun() will always return false
	unsigned long timeout;

	// Last runned time in Ms
	unsigned long last_run;

	// Scheduled run in Ms (MUST BE CACHED)	
	unsigned long _cached_next_run = 0;

    // Time that the thread was started
    unsigned long _t0;

    // True when shouldRun returns true for the first time.
    bool _started = false;

    // Used to store and restore the state of a thread.
    int flag = 0;

    // Thread to be run due to a call to AWAIT()
    Thread *_linked_thread = nullptr;


	/*
		IMPORTANT! Run after all calls to run()
		Updates last_run and cache next run.
		NOTE: This MUST be called if extending
		this class and implementing run() method
	*/
	void runned(unsigned long time);

	// Default is to mark it runned "now"
	void runned() { runned(millis()); }

	// Callback for run() if not implemented
	void (*_onRun)(void);		

public:

	// If the current Thread is enabled or not
	bool enabled = true;

	// ID of the Thread (initialized from memory adr.)
	int ThreadID;

    void *result = nullptr;

	#ifdef USE_THREAD_NAMES
		// Thread Name (used for better UI).
		String ThreadName;			
	#endif

	Thread(void (*callback)(void) = NULL, unsigned long _interval = 0, unsigned long _timeout = 0);

	// Set the desired interval for calls, and update _cached_next_run
	virtual void setInterval(unsigned long _interval);

	void setTimeout(unsigned long _timeout);

	// Return if the Thread should be runned or not
	virtual bool shouldRun(unsigned long time);

	// Default is to check whether it should run "now"
	bool shouldRun() { return shouldRun(millis()); }

	// Callback set
	void onRun(void (*callback)(void));

	// Runs Thread
	virtual void run();
};

#endif
