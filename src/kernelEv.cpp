#include "kernelEv.h"
#include "SCHEDULE.h"
#include "IVTEntry.h"

extern int lockFlag;
extern int context_switch_on_demand;

KernelEv::KernelEv(unsigned char ivtNo) {
	val = 0;
	blocked = 0;
	owner = PCB::running;
	if (IVTEntry::allIVTEn[ivtNo] != 0) {
		IVTEntry::allIVTEn[ivtNo]->event = this;
	}
	entryNo = ivtNo;
}

KernelEv::~KernelEv() {
	blocked = 0;
	owner = 0;
	IVTEntry::allIVTEn[entryNo] = 0;
}
////////////////////KOD DISPATCH MORA DA SE ZABRANI PREKID PRE CONTSWONDEMAND = 1
void KernelEv::waitEv() {
	lock
	if (PCB::running == owner) {
		if (val == 0) {
			PCB::running->status = BLOCKED;
			blocked = owner;
			dispatch();
		}
		else {
			if (val == 1) val = 0;
		}
	}
	unlock
}

void KernelEv::signalEv() {
	lock
	val = 1;
	if (blocked != 0) {
		//if (blocked->status != FINISHED) {
			blocked->status = READY;
			Scheduler::put(blocked);
		//}
		blocked = 0;
		dispatch();
	}
	unlock
}
