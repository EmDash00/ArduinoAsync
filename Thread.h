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

#include "ThreadController.h"


/*
   The following macros provide async style programming using ThreadController with some limitations. See below.
   Unfortunately, AWAIT, YIELD_TO, and HALT_TO cannot be used in a static context.
   Since threads cannot be removed in a static context, HALT is the same as FREEZE in StaticThreadControllers.
*/


/*
   YIELDs execution back to the ThreadController through the use of a return statement,
   also setting the specified flag. Note: flag -1 is special. See HALT.
   This and macros that depend on it must be used in run() because the return won't work as intended otherwise.
*/
#define YIELD(_flag) (flag = _flag); return

// Called when YIELD is called without a custom flag. Sets default flag 0.
#define YIELD_DEFAULT (YIELD(0))

// Links the specified thread and YIELDs. The linked thread is called immediately from the main loop.
#define YIELD_TO(_thread, _flag) (_linked_thread = thread), YIELD(_flag)

/*
   Gets a pointer to the eventual result of a WorkerThread's work of type 'type'. A "future" if you may.
   The pointer is from thread->result and is nullptr until the thread is done working.
   The work will pause and wait for all awaiting threads to retrieve the result before continuing work.
   Warning, if the thread doesn't have a result any code after the AWAIT in run() will never be executed.

   CANNOT be used as a function parameter due to having returns within the macro body.
*/
#define AWAIT(type, _thread, _flag)                     \
    _thread->result ? (type)_thread->result : nullptr); \
    awaiting = true;                                    \
    ({                                                  \
        if (!linked_thread)                             \
        {                                               \
            (_thread->num_awaiting)++;                  \
            YIELD_TO(_thread, _flag);                   \
        }                                               \
        else                                            \
        {                                               \
            if (_thread_result == nullptr)              \
            {                                           \
                YIELD(_flag);                           \
            }                                           \
            else                                        \
            {                                           \
                linked_thread = nullptr;                \
                awaiting = false;                       \
                (_thread->num_awaiting)--;              \
            }                                           \
        }                                               \
    })

// Suspends the thread's execution for pause_interval ms.
#define PAUSE(_pause_interval, _flag) (pause_interval = _pause_interval), YIELD(_flag)

// Suspends the thread's execution indefinetely. Thread will resume on a call to RESUME
#define FREEZE(_flag) (frozen = true), YIELD(_flag)

// Unsuspends a suspended thread by either unfreezing and/or unpausing it.
#define RESUME(_thread) (_thread->frozen = false, _thread->pause_interval = 0;)

/*
   YIELDs with flag -1. ThreadController will remove any thread with flag -1 after calling run()
   In StaticThreadController, it will simply FREEZE the thread since it cannot be removed.
*/
#define HALT YIELD(-1)

// HALTs the thread, offshooting another thread. Similar to YIELD_To but will remove the current thread.
#define HALT_TO(_thread) YIELD_TO(_thread, -1)

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

	// If the thread is frozen or not.
	bool frozen = false;

	// Last runned time in Ms
	unsigned long last_run;

	// Scheduled run in Ms (MUST BE CACHED)	
	unsigned long _cached_next_run = 0;

    // Time that the thread was started
    unsigned long _t0;

    // True when shouldRun returns true for the first time.
    bool _started = false;

    // The number of threads awaiting this one.
    // Used by workers to determine when every thread that wants the results gets it before clearing it and continuing.
    int num_awaiting = 0;



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
	void (*_onRun)(void) = nullptr;

public:

	// If the current Thread is enabled or not
	bool enabled = true;

	// If the current thread is awaiting another thread.
	bool awaiting = false;

	// ID of the Thread (initialized from memory adr.)
	int ThreadID;

    // Used to store and restore the state of a thread.
    int flag = 0;

    // Thread this thread has linked via YIELD_TO
    Thread *linked_thread = nullptr;

    //Represents the result of a thread's work, if any. "Void" threads have result always equal to nullptr.
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
