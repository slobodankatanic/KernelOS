/*
 * PCB.h
 *
 *  Created on: Apr 9, 2020
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

#include "thread.h"
#include "list.h"
#include <iostream.h>
#include "sigQueue.h"
#include "handlQ.h"
#include "semQueue.h"

#define lock asm{\
 	pushf;\
 	cli;\
}

#define unlock asm popf

#define lockf lockFlag = 0;

#define unlockf lockFlag = 1;\
if (context_switch_on_demand) dispatch();

class Lista;

enum Status {MAIN, TERMINATED, READY, BLOCKED, FINISHED, IDLE, KILLED};

class PCB {
public:
	Thread* parent;
	static int idCnt;
	PCB() {}
	PCB(Thread* t, StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	unsigned* stack;
	volatile unsigned ss;
	volatile unsigned sp;
	volatile unsigned bp;
	Time timeSlice;
	ID id;
	Status status;
	Thread* myThread;

	HandlersQueue* signalHandlers[16];
	SignalQueue* recievedSignals;
	int signalStatus[16];
	static int globalSignalStatus[16];

	static PCB* running;
	static void wrapper();
	static void idleFun();
	Lista* waitingToComplete;
	unsigned int semaphoreTime;
	//SemQueue* semResources;
	~PCB();
};

#endif
