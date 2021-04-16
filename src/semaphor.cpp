/*
 * semaphor.cpp
 *
 *  Created on: April 15, 2020
 *      Author: OS1
 */

#include "semaphor.h"
#include "kernSem.h"

extern lockFlag;
extern context_switch_on_demand;
extern volatile int numOfSemaphores;
extern volatile KernelSem* kernelSemaphores[maxNumOfSemaphores];

Semaphore::Semaphore (int init) {
	this->myImpl = new KernelSem(init);
	kernelSemaphores[numOfSemaphores++] = this->myImpl;
}

int Semaphore::wait (Time maxTimeToWait) {

/*#ifndef BCC_BLOCK_IGNORE
	lock
#endif*/

	int ret = myImpl->kernel_wait(maxTimeToWait);

/*#ifndef BCC_BLOCK_IGNORE
	unlock
#endif*/

	return ret;
}

int Semaphore::signal(int n) {

#ifndef BCC_BLOCK_IGNORE
	lock
#endif

	int ret = myImpl->kernel_signal(n);

#ifndef BCC_BLOCK_IGNORE
	unlock
#endif

	return ret;
}

int Semaphore::val () const {
	return myImpl->getVal();
}

Semaphore::~Semaphore () {
	delete myImpl;
}

