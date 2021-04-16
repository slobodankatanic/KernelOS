/*
 * PCB.cpp
 *
 *  Created on: Apr 9, 2020
 *      Author: OS1
 */

#include "PCB.h"
#include <dos.h>
#include "SCHEDULE.h"
#include <iostream.h>
#include "kernSem.h"

extern volatile int cntr;
extern int lockFlag;
extern int context_switch_on_demand;
extern void interrupt timer();
int activeThreads = 0;

int PCB::idCnt = 0;
PCB* PCB::running = 0;
int PCB::globalSignalStatus[16] = {0, 0, 0, 0, 0, 0, 0, 0};

PCB::PCB(Thread* t, StackSize stackSize, Time timeSlice) {
	int i;
	for (i = 0; i < 16; i++) {
		this->signalHandlers[i] = new HandlersQueue();
		this->signalStatus[i] = 0;
	}
	this->myThread = t;
	if (stackSize == 56000) {
		PCB::running = this;
		this->parent = 0;
	}
	else {
		this->parent = Thread::getThreadById(Thread::getRunningId());
		int k;
		if (this->parent != 0 && myThread != 0) {
			for (k = 0; k < 16; k++) {
				this->signalStatus[k] = PCB::running->signalStatus[k];
				for (PCB::running->signalHandlers[k]->postaviTekPoc();
					 PCB::running->signalHandlers[k]->imaTek();
					 PCB::running->signalHandlers[k]->naSled()) {
					this->myThread->registerHandler(k, PCB::running->signalHandlers[k]->dohvTek());
				}
			}
		}
	}
	semaphoreTime = 0;
	stackSize /= sizeof(unsigned);
	id = idCnt++;
	unsigned* st = new unsigned[stackSize];
	if (timeSlice == 0) {
		this->timeSlice = -1;
	} else this->timeSlice = timeSlice;
	st[stackSize - 1] = 0x200;
	st[stackSize - 2] = FP_SEG(PCB::wrapper);
	st[stackSize - 3] = FP_OFF(PCB::wrapper);
	this->sp = FP_OFF(st + stackSize - 12);
	this->ss = FP_SEG(st + stackSize - 12);
	this->bp = FP_OFF(st + stackSize - 12);
	this->stack = st;
	this->status = TERMINATED;
	this->waitingToComplete = new Lista();
	this->recievedSignals = new SignalQueue();
	//this->semResources = new SemQueue();
}

void processSignals();

void defaultSigHand() {
/*#ifndef BCC_BLOCK_IGNORE
	asm cli
#endif*/
	//lockFlag = 1;
	PCB::running->status = FINISHED;
	if (PCB::running->waitingToComplete->dohvBrojEl() > 0) {
		for(PCB::running->waitingToComplete->postaviTekPoc();
					PCB::running->waitingToComplete->imaTek();) {
			//if (PCB::running->waitingToComplete->dohvTek()->status != FINISHED) {
				PCB::running->waitingToComplete->dohvTek()->status = READY;
				Scheduler::put(PCB::running->waitingToComplete->dohvTek());
			//}
			//PCB::running->waitingToComplete->obrisiTek();
			//PCB::running->waitingToComplete->postaviTekPoc();
		}
		delete PCB::running->waitingToComplete;
	}

	if (PCB::running->parent != 0)
		PCB::running->parent->signal(1);

	//delete PCB::running->recievedSignals;
	//PCB::running->recievedSignals = new SignalQueue();
	PCB::running->myThread->signal(2);
	int i;
	for (i = 0; i < 15; i++) {
		if (i == 2)
			PCB::running->signalStatus[i] = 0;
		else PCB::running->signalStatus[i] = 1;
	}
	processSignals();

	/*if (PCB::running->semResources->dohvBrojEl() > 0) {
		for (PCB::running->semResources->postaviTekPoc();
				PCB::running->semResources->imaTek();) {
			PCB::running->semResources->dohvTek()->kernel_signal(0);
			PCB::running->semResources->postaviTekPoc();
		}
	}*/
	//dispatch();
	//ili probaj samo timer();
	//vidi i za lockFlag++
}

void processSignals() {
	lockFlag = 0;
	asm sti
	for (PCB::running->recievedSignals->postaviTekPoc(); PCB::running->recievedSignals->imaTek();) {
		if ((PCB::running->signalStatus[PCB::running->recievedSignals->dohvTek()] == 0)
				&& (PCB::globalSignalStatus[PCB::running->recievedSignals->dohvTek()] == 0)) {
			if (PCB::running->recievedSignals->dohvTek() == 0) {
				defaultSigHand();
				PCB::running->recievedSignals->obrisiTek();
				context_switch_on_demand = 1;
				break;
			}
			else {
				int i = PCB::running->recievedSignals->dohvTek();
				for ((PCB::running->signalHandlers[i])->postaviTekPoc();
					 (PCB::running->signalHandlers[i])->imaTek();
					 (PCB::running->signalHandlers[i])->naSled()) {
					(PCB::running->signalHandlers[i]->dohvTek())();
				}
			}
		}
		PCB::running->recievedSignals->obrisiTek();
		PCB::running->recievedSignals->postaviTekPoc();
	}
/*#ifndef BCC_BLOCK_IGNORE
	asm cli
#endif
	lockFlag = 1;
	if (context_switch_on_demand) dispatch();*/
	lockFlag = 1;
	asm cli
}

void PCB::wrapper() {
	running->myThread->run();
	lock
	running->status = FINISHED;
	if (running->waitingToComplete->dohvBrojEl() > 0) {
		for(running->waitingToComplete->postaviTekPoc();
					running->waitingToComplete->imaTek();) {
			//if (running->waitingToComplete->dohvTek()->status != FINISHED) {
				running->waitingToComplete->dohvTek()->status = READY;
				Scheduler::put(running->waitingToComplete->dohvTek());
			//}
			//running->waitingToComplete->obrisiTek();
			//running->waitingToComplete->postaviTekPoc();

		}
		delete running->waitingToComplete;
	}

	if (PCB::running->parent != 0)
			PCB::running->parent->signal(1);

	//delete PCB::running->recievedSignals;
	//PCB::running->recievedSignals = new SignalQueue();
	PCB::running->myThread->signal(2);
	int i;
	for (i = 0; i < 15; i++) {
		if (i == 2)
			PCB::running->signalStatus[i] = 0;
		else PCB::running->signalStatus[i] = 1;
	}
	processSignals();

	/*if (PCB::running->semResources->dohvBrojEl() > 0) {
		for (PCB::running->semResources->postaviTekPoc();
				PCB::running->semResources->imaTek();) {
			PCB::running->semResources->dohvTek()->kernel_signal(0);
			PCB::running->semResources->postaviTekPoc();
		}
	}*/

	/*if (PCB::running->parent != 0)
		PCB::running->parent->signal(1);
	if (PCB::running->myThread != 0)
		PCB::running->myThread->signal(2);*/
	unlock
	dispatch();
}

void PCB::idleFun() {
	//int i = 0;
	while(1) {
		//if (i == 0) cout << " ID ";
		//i = (i + 1) % 100000;
	}
}

PCB::~PCB() {
	delete[] stack;
	delete waitingToComplete;
	//delete semResources;
	delete recievedSignals;
	int k;
	for (k = 0; k < 16; k++)
		delete this->signalHandlers[k];
}
