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

XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

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


    
    
    
    
