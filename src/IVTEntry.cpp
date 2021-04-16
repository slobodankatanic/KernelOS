#include "IVTEntry.h"

extern lockFlag;
extern context_switch_on_demand;

IVTEntry* IVTEntry::allIVTEn[NUM_OF_ENTRIES] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

IVTEntry::IVTEntry(unsigned char ivtNo, pInterrupt intRoutine) {
	allIVTEn[ivtNo] = this;
	entryNo = ivtNo;
	event = 0;
	lockf
	oldRoutine = getvect(ivtNo);
	setvect(ivtNo, intRoutine);
	unlockf
}

IVTEntry::~IVTEntry() {
	lockf
	setvect(entryNo, oldRoutine);
	unlockf
	//allIVTEntry[entryNo] = 0;
	event = 0;
	oldRoutine = 0;
}


