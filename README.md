## ArduinoThreads Motivation

This is a fork of Ivan Seidel's [ArduinoThread](https://github.com/ivanseidel/ArduinoThread).

The protothread mechanism used in ArduinoThread is used to create asynchronous contexts. With C++
define statements, it is possible to create Python-like async code that runs on an Arduino.

Static Thread Controllers have been for the time being removed.

The name thread sticks around from the original but it may be renamed in a future release.

Documentation and library are both still very WIP. This may become its own seperate thing in the future.

### Concepts
Like in ArduinoThread, the `run` method of a thread is called once per loop. This allows a programmmer
to think of several tasks as "running at once" and the loop context switching between tasks that
are ready to run.

To accomplish this context switching, all threads have an internal `m_flag` int variable that makes 
them all act like finite state machines. Switch case statements or if statements can be 
used on the flag to allow the thread to execute code based on its state.

Setting the flag is as simple as calling `YIELD(flag_number)`, which yields execution back to the
`ThreadController` and sets the flag. `YIELD_DEFAULT` can be used for simple applications and simply
sets the flag to 0.

If `m_flag` is set to -1 and the thread has yielded execution back to the `ThreadController`,
the ThreadController removes the thread from the queue. The `HALT` keyword does exactly this.

Threads can also be paused in this framework for a certain number of milliseconds using the
`WAIT` keyword. Unlike `delay` `WAIT` will not block execution of the `ThreadController` and allows
context switching to continue for other tasks. Threads can also be paused indefinetely with
`FREEZE`. Threads paused with `FREEZE` will not enter an awake state again until another thread
uses `UNFREEZE(thread_ptr)`.

In addition to these mechanisms which control the control flow, ArduinoAsync also supports producer
consumer semantics.

All threads have an internal buffer of type void*. This is used to gather the return of an 
asynchronously executing thread. Users of the library will not have to worry about this, however.

The `AWAIT` statement will take a pointer to a thread and check its buffer for the result which is
otherwise `nulltpr`. If multiple consumers `AWAIT` on a producer, a semaphore internal to the producer
will block the producer from clearing the result and entering a sleeping state until all consumers have
gotten the result.

 
