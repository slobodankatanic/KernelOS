/*
 * kernSem.cpp
 *
 *  Created on: Apr 15, 2020
 *      Author: OS1
 */

#include "kernSem.h"
#include "SCHEDULE.h"
extern int lockFlag;
extern int context_switch_on_demand;
volatile int numOfSemaphores = 0;
volatile KernelSem* kernelSemaphores[maxNumOfSemaphores];

KernelSem::KernelSem(int init) {
	val = init;
	max = val;
	limitedQueue = new Lista();
	nonLimitedQueue = new Lista();
}

KernelSem::~KernelSem() {
	delete limitedQueue;
	delete nonLimitedQueue;
}

int KernelSem::kernel_wait(unsigned int time) {
	lock
	int ret;
	if (time == 0) PCB::running->semaphoreTime = -1;
	else PCB::running->semaphoreTime = time;
	val = val - 1;
	//added
	//added
	//PCB::running->semResources->dodajNaKraj(this);
	if (val < 0) {
		if (time == 0) nonLimitedBlock();
		else limitedBlock(time);
	}
	if (PCB::running->semaphoreTime == 0) ret = 0;
	else ret = 1;
	unlock
	return ret;
}

int KernelSem::kernel_signal(int n) {
	int ret = 0;
	//if (max == val) return ret;
	if (n >= 0) {
		if (val < 0) {
			ret = deblock(n);
		} else {
			ret = 0;
			if (n > 0) val += n;
			else val++;
		}
	} else ret = n;
	//added
	/*if (PCB::running->semResources->dohvBrojEl() > 0) {
		for (PCB::running->semResources->postaviTekPoc(); PCB::running->semResources->imaTek();)
			if (PCB::running->semResources->dohvTek() == this) {
				PCB::running->semResources->obrisiTek();
			}
			else PCB::running->semResources->naSled();
	}*/
	//added
	return ret;
}

void KernelSem::limitedBlock(unsigned int time) {
	PCB::running->status = BLOCKED;
	limitedQueue->dodajNaKraj(PCB::running);
	dispatch();
}

void KernelSem::nonLimitedBlock() {
	PCB::running->status = BLOCKED;
	PCB::running->semaphoreTime = -1;
	nonLimitedQueue->dodajNaKraj(PCB::running);
	dispatch();
}

int KernelSem::deblock(int n) {

	if (n == 0) {
		val++;
		if (limitedQueue->dohvBrojEl() > 0) {
			limitedQueue->postaviTekPoc();
			//if (limitedQueue->dohvTek()->status != FINISHED) {
				limitedQueue->dohvTek()->status = READY;
				Scheduler::put(limitedQueue->dohvTek());
			//}
			limitedQueue->obrisiTek();
			return 0;
		}
		if (nonLimitedQueue->dohvBrojEl() > 0) {
			nonLimitedQueue->postaviTekPoc();
			//if (nonLimitedQueue->dohvTek()->status != FINISHED) {
				nonLimitedQueue->dohvTek()->status = READY;
				Scheduler::put(nonLimitedQueue->dohvTek());
			//}
			nonLimitedQueue->obrisiTek();
			return 0;
		}
		return 0;
	}
	else {
		if (n > 0) {
			val += n;
			int ret = 0;
			while(limitedQueue->dohvBrojEl() > 0 && n > 0) {
				limitedQueue->postaviTekPoc();
				//if (limitedQueue->dohvTek()->status != FINISHED) {
					limitedQueue->dohvTek()->status = READY;
					Scheduler::put(limitedQueue->dohvTek());
				//}
				limitedQueue->obrisiTek();
				n--; ret++;
			}
			while(nonLimitedQueue->dohvBrojEl() > 0 && n > 0) {
				nonLimitedQueue->postaviTekPoc();
				//if (nonLimitedQueue->dohvTek()->status != FINISHED) {
					nonLimitedQueue->dohvTek()->status = READY;
					Scheduler::put(nonLimitedQueue->dohvTek());
				//}
				nonLimitedQueue->obrisiTek();
				n--; ret++;
			}
			return ret;
		} else return n;
	}
}
