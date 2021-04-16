/*
 * hello.cpp
 *
 *  Created on: Apr 1, 2015
 *      Author: OS1
 */

#include <iostream.h>
#include "PCB.h"
#include "list.h"

void inic();
void restore();

extern Lista* AllPCB;
extern PCB* idle;

int userMain(int argc, char* argv[]);

class MainThread : public Thread {
public:
	MainThread(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice) :
		Thread(stackSize, timeSlice) {}
};

int main(int argc, char* argv[]) {
	cout << "Pocetak\n";
	asm cli
	//PCB* pcb = new PCB(0, 56000, 2);
	//PCB::running = pcb;

	MainThread* r = new MainThread(56000, 2);
	//PCB* runn = new PCB(0, 56000, 2);
	PCB::running->status = READY;
	inic();
	asm sti

	int ret = userMain(argc, argv);

	asm cli
	restore();
	asm sti

	delete r;
	delete AllPCB;
	delete idle;
	return ret;
}


