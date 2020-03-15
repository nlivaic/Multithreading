# Multithreading

### Process and threading basics

* Process is a collection of resources. It is represented by a Process Control Block (PCB).
* On every context switch, process-specific state is stored in the PCB, which is in memory itself.
* What makes process context switching expensive is:
    * Registers have to be stored.
    * Virtual memory mappings have to be stored. Restoring these is very expensive.
    * CPU Cache is flushed so the new process can populate it. Repopulating it on every context switch requires fetching instructions from memory anew (very expensive).
* PCB has (among other things) following pieces of information:
    * Process number
    * Program Counter
    * Registers' state
    * Memory limits (i.e. virtual memory mappings)
    * Open files (handles)
    * Priority
    * CPU scheduling info
* Thread is a unit of executing and scheduling.
* Threads share address space.
![image](https://user-images.githubusercontent.com/26722936/76698186-3b112000-66a0-11ea-966e-e03455100dc3.png)
![image](https://user-images.githubusercontent.com/26722936/76698274-56c8f600-66a1-11ea-9959-fcf2dd4d7530.png)

### Concurrency control and coordination

* Synchronization mechanisms are:
    * Mutual exclusion (*mutex*) marks critical sections. Critical sections are pieces of code that, while the thread is in them, cannot get preempted by the OS. Make sure you are in critical sections __as briefly as possible__.
    * Condition variable - a specific condition which has to be fulfilled before proceeding. While the thread is in a You then make your thread wait on the condition variable (thread block here). Other threads then send a signal/broadcast on that same condition variable once some state changes. This puts your thread back into the ready state so it can get scheduled again by the CPU. Make sure you check the condition variable again before proceeding (use the *while* loop), because some other thread might have acquired the variable and changed its value before your thread got a chance to get scheduled.

### Thread Representation and Thread Creation

* As a general representation of a thread, a thread variable of a thread type contains the data as in the figure below:

![image](https://user-images.githubusercontent.com/26722936/76698638-d953b480-66a5-11ea-9e91-9b742d8cfe14.png)

* Threads are created by Thread 0 doing a *fork()*, thus creating a Thread 1.
* Thread 0 can decide whether he is interested in the result of Thread 1 by calling *join()* on Thread 1. This will block Thread 0 until Thread 1 finished. If Thread 0 doesn't require Thread 1 result, then both can continue to run concurrently.

![image](https://user-images.githubusercontent.com/26722936/76698614-706c3c80-66a5-11ea-9cad-096494c08889.png)

* Prevent spurious wakeups. 
* Deadlocks: deadlock prevention is best done by maintaing lock order. First acquire lock A, then lock B.
* Mutex safety:
    * Shared data should be accessed through a single mutex.
    * Mutex scope must be visible to all threads involved.
    * Lock mutexes in a specific order to prevent deadlocks.
* Condition variables safety tips:
    * Notify waiting threads.
    * When in doubt - broadcast. Keep in mind performance cost (since it is more expensive to broadcast as opposed to signalling).
    * Mutex is not neededfor broadcast/signal. Release mutex first, then broadcast/signal.

### Threading patterns

* Producer/Consumer - One producer and multiple consumers. Producer inserts at head of queue, consumers remove from end of queue. Lock is necessary so consumers do not consume the same item. Care must be taken so producer blocks once queue is full and consumers block once queue is empty.
* Reader/Writer - Zero or more readers read a resource (e.g. a variable or a file), while zero or one writers write to the resource. Placing the mutex to guard access to resource would be too restrictive as we want to allow multiple reader to read at the same time, while simultaneously only allow one writer if there are no readers reading the resource. A better approach is to create counters for readers and writers respectively and place mutex around those resources. Once a lock is successfuly acquired and condition variables are favourable, access to the resource can be granted.
* Boss/Workers - Boss receives a request and then passes it to worker thread(s). We want to keep the boss as simple as possible.
    * A few approaches here are:
        1. boss knows which threads are free and assigns work to one of them. Downside is the boss now has to keep track of thread status (increases complexity). Upside is worker threads do not have to synchronize (faster execution, simpler workers).
        2. a queue exists with boss writing into it and workers reading from it. This is similar to Reader/Writer pattern. Downside is worker threads have to synchronize on queue. Upside is boss has no overhead regarding worker threads. This approach is favoured because in practice it provides higher throughput.
    * How many workers?
        * On demand, each creation has a cost, can add overhead if there are too many of them.
        * A pool of ready workers is a better option.

### User-level threads vs kernel threads

* The difference is in how they are scheduled:
    * ULT are scheduled by the app itself in user-land. Kernel is not aware of these threads. If ULT blocks, entire process is blocked because kernel doesn't discern between threads belonging to the same process.
    * KLT are scheduled by the kernel, which implies the kernel is aware of these threads.
* Both ULT and KLT run user code.
* Java JVM uses KLT.
* One-to-one ULT:KLT is nowadays the dominant model.
* When request a thread using CLR, you will get a KLT (on Windows machine).
* Kernel-backed threading model: kernel schedules each thread individually. Allows for parallel execution of multiple threads belonging to the same process (in a multi-core system).
* "User level threads map to kernel level threads" - meaning of the phrase is as follows:
    * User space is presented an abstraction of a unit of execution (i.e. a thread can be thought of as a virtual CPU).
    * This abstraction is implemented through a kernel-level thread, owning kernel resources.
    * What this means is user level threads as such do not exist (on Linux), but rather each thread **is** kernel thread.


    
    
    
    
