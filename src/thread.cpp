/*
 * thread.cpp
 *
 *  Created on: Apr 9, 2020
 *      Author: OS1
 */

#include "PCB.h"
#include "list.h"
#include "SCHEDULE.h"

extern lockFlag;
extern context_switch_on_demand;

Lista* AllPCB = new Lista();

Thread::Thread(StackSize stackSize, Time timeSlice) {
	lock
	myPCB = new PCB(this, stackSize, timeSlice);
	AllPCB->dodajNaKraj(myPCB);
	unlock
}

Thread::~Thread() {
	for (AllPCB->postaviTekPoc(); AllPCB->dohvTek() != myPCB; AllPCB->naSled());
	delete myPCB;
	myPCB = 0;
	AllPCB->obrisiTek();
}

void Thread::start() {
	if (myPCB->status == TERMINATED) {
		myPCB->status = READY;
		Scheduler::put(myPCB);
	}
}

void Thread::waitToComplete() {
	lock
	if (this->myPCB->status != FINISHED && this->myPCB->status != TERMINATED && PCB::running != this->myPCB) {
		PCB::running->status = BLOCKED;
		this->myPCB->waitingToComplete->dodajNaKraj(PCB::running);
		dispatch();
	}
	unlock
}

ID Thread::getRunningId() {
	return PCB::running->id;
}

ID Thread::getId() {
	return myPCB->id;
}

Thread* Thread::getThreadById(ID id) {
	if (AllPCB->dohvBrojEl() > 0) {
		for(AllPCB->postaviTekPoc(); AllPCB->imaTek() && AllPCB->dohvTek()->id != id; AllPCB->naSled());
	} else return 0;
	if (AllPCB->imaTek()) return AllPCB->dohvTek()->myThread;
	else return 0;
}

//signals

void Thread::signal(SignalId signal) {
	lock
	myPCB->recievedSignals->dodajNaKraj(signal);
	unlock
}

void Thread::registerHandler(SignalId signal, SignalHandler handler) {
	lock
	myPCB->signalHandlers[signal]->dodajNaKraj(handler);
	unlock
}

void Thread::unregisterAllHandlers(SignalId id) {
	if (myPCB->signalHandlers[id]->dohvBrojEl() > 0) {
		myPCB->signalHandlers[id]->postaviTekPoc();
		while(myPCB->signalHandlers[id]->imaTek()) {
			myPCB->signalHandlers[id]->obrisiTek();
			myPCB->signalHandlers[id]->postaviTekPoc();
		}
	}
}

void Thread::swap(SignalId id, SignalHandler hand1, SignalHandler hand2) {
	int p1 = 0, p2 = 0, h1 = 0, h2 = 0;

	for (myPCB->signalHandlers[id]->postaviTekPoc();
		 myPCB->signalHandlers[id]->imaTek() && myPCB->signalHandlers[id]->dohvTek() != hand1;
		 myPCB->signalHandlers[id]->postaviTekPoc(), p1++);
	if (myPCB->signalHandlers[id]->imaTek())
		h1 = 1;

	for (myPCB->signalHandlers[id]->postaviTekPoc();
		 myPCB->signalHandlers[id]->imaTek() && myPCB->signalHandlers[id]->dohvTek() != hand2;
		 myPCB->signalHandlers[id]->postaviTekPoc(), p2++);
	if (myPCB->signalHandlers[id]->imaTek())
		h2 = 1;

	if (h1 == 1 && h2 == 1) {
		for (myPCB->signalHandlers[id]->postaviTekPoc();
			 myPCB->signalHandlers[id]->imaTek() && p1 > 0;
			 myPCB->signalHandlers[id]->postaviTekPoc(), p1--);
		myPCB->signalHandlers[id]->postTek(hand2);

		for (myPCB->signalHandlers[id]->postaviTekPoc();
			 myPCB->signalHandlers[id]->imaTek() && p2 > 0;
			 myPCB->signalHandlers[id]->postaviTekPoc(), p2--);
		myPCB->signalHandlers[id]->postTek(hand1);
	}
}

void Thread::blockSignal(SignalId signal) {
	lock
	myPCB->signalStatus[signal] = 1;
	unlock
}

void Thread::blockSignalGlobally(SignalId signal) {
	PCB::globalSignalStatus[signal] = 1;
}

void Thread::unblockSignal(SignalId signal) {
	lock
	myPCB->signalStatus[signal] = 0;
	unlock
}

void Thread::unblockSignalGlobally(SignalId signal) {
	PCB::globalSignalStatus[signal] = 0;
}

//signals
