/*
 * kernSem.h
 *
 *  Created on: Apr 15, 2020
 *      Author: OS1
 */

#ifndef KERNSEM_H_
#define KERNSEM_H_

#define maxNumOfSemaphores 20

#include "list.h"

class KernelSem {
	Lista* limitedQueue;
	Lista* nonLimitedQueue;
	int max;
public:
	int val;
	KernelSem(int init);
	int kernel_wait(unsigned int);
	int kernel_signal(int);
	void limitedBlock(unsigned int);
	void nonLimitedBlock();
	int deblock(int);
	void setVal(int v) { val = v; }
	int getVal() const { return val; }
	Lista* getLimitedQueue() { return limitedQueue; }
	~KernelSem();
};

#endif /* KERNSEM_H_ */
