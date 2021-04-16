/*
 * kernelEv.h
 *
 *  Created on: May 17, 2020
 *      Author: OS1
 */

#ifndef KERNELEV_H_
#define KERNELEV_H_

#include "PCB.h"

class KernelEv {
	int val;
	PCB* blocked;
	PCB* owner;
	unsigned char entryNo;
public:
	KernelEv(unsigned char);
	~KernelEv();
	void waitEv();
	void signalEv();
	PCB* getBlocked() { return blocked; }
	PCB* getOwner() { return owner; }
};

#endif /* KERNELEV_H_ */
