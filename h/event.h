#ifndef _event_h_
#define _event_h_

typedef unsigned char IVTNo;

class KernelEv;

#define PREPAREENTRY(numEntry, callOld)\
	void interrupt inter##numEntry(...);\
	IVTEntry* newEntry##numEntry = new IVTEntry(numEntry, inter##numEntry);\
	void interrupt inter##numEntry(...) {\
		newEntry##numEntry->event->signalEv();\
		if (callOld == 1) newEntry##numEntry->oldRoutine();\
	}

class Event {
public:
	Event (IVTNo ivtNo);
	~Event ();
	void wait ();
protected:
	friend class KernelEv;
	void signal(); // can call KernelEv
private:
	KernelEv* myImpl;
};

#endif




