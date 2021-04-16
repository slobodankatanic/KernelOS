/*
 * IVTEntry.h
 *
 *  Created on: May 17, 2020
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#include "KernelEv.h"
#include <dos.h>
#define NUM_OF_ENTRIES 256

typedef void interrupt (*pInterrupt)(...);

class IVTEntry {
public:
	KernelEv* event;
	pInterrupt oldRoutine;
	unsigned char entryNo;
	IVTEntry(unsigned char, pInterrupt);
	~IVTEntry();
	static IVTEntry* allIVTEn[NUM_OF_ENTRIES];
	void signal() { event->signalEv(); }
	void callOld() { oldRoutine(); }
};

#endif /* IVTENTRY_H_ */
