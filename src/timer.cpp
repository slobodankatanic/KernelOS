/*
 * timer.cpp
 *
 *  Created on: Apr 9, 2020
 *      Author: OS1
 */

#include <dos.h>
#include "PCB.h"
#include "SCHEDULE.h"
#include "kernSem.h"
#include <iostream.h>

int syncPrintf(const char *format, ...);

extern void processSignals();
PCB* idle = new PCB();

volatile unsigned oldTimerSEG, oldTimerOFF;
extern void defaultSigHand();
volatile unsigned tsp;
volatile unsigned tss;
volatile unsigned tbp;

volatile int cntr = defaultTimeSlice;
volatile int context_switch_on_demand = 0;
volatile int lockFlag = 1;

extern volatile int numOfSemaphores;
extern volatile KernelSem* kernelSemaphores[maxNumOfSemaphores];

void tick();

void interrupt timer() {

	if(context_switch_on_demand == 0) {
		tick();
		int i;
		for (i = 0; i < numOfSemaphores; i++) {

			if (kernelSemaphores[i]->getLimitedQueue()->dohvBrojEl() > 0) {

				for (kernelSemaphores[i]->getLimitedQueue()->postaviTekPoc();
						kernelSemaphores[i]->getLimitedQueue()->imaTek();) {

					(kernelSemaphores[i]->getLimitedQueue()->dohvTek()->semaphoreTime)--;
					if (kernelSemaphores[i]->getLimitedQueue()->dohvTek()->semaphoreTime == 0) {
						//if (kernelSemaphores[i]->getLimitedQueue()->dohvTek()->status != FINISHED) {
							kernelSemaphores[i]->getLimitedQueue()->dohvTek()->status = READY;
							Scheduler::put(kernelSemaphores[i]->getLimitedQueue()->dohvTek());
						//}
						kernelSemaphores[i]->getLimitedQueue()->obrisiTek();
					} else kernelSemaphores[i]->getLimitedQueue()->naSled();
				}
			}
		}
		if (PCB::running->timeSlice != -1 && cntr > 0) cntr--;
		asm int 60h;
	}

	if (PCB::running->status != IDLE)
		processSignals();

	//if (context_switch_on_demand == 0) {if (PCB::running->timeSlice != -1 && cntr > 0) cntr--;}
	if ((cntr == 0 || context_switch_on_demand == 1)) {
		if (lockFlag == 1) {
			context_switch_on_demand = 0;
			asm {
				//save stack pointer and base pointer
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}

			PCB::running->bp = tbp;
			PCB::running->sp = tsp;
			PCB::running->ss = tss;
			// scheduler
			if (PCB::running->status != FINISHED && PCB::running->status != BLOCKED &&
					PCB::running->status != IDLE) {
				Scheduler::put(PCB::running);
			}
			PCB::running = Scheduler::get();
			if (PCB::running == 0) {
				PCB::running = idle;
			}
			tsp = PCB::running->sp;
			tss = PCB::running->ss;
			tbp = PCB::running->bp;

			cntr = PCB::running->timeSlice;

			asm {
				// restaurira sp
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}
		}
		else context_switch_on_demand = 1;
	}
}

void inic(){
	asm{
		cli
		push es
		push ax

		mov ax,0
		mov es,ax

		mov ax, word ptr es:0022h
		mov word ptr oldTimerSEG, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFF, ax

		// postavlja novu rutinu
		mov word ptr es:0022h, seg timer
		mov word ptr es:0020h, offset timer

		// postavlja staru rutinu
		mov ax, oldTimerSEG
		mov word ptr es:0182h, ax
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
	}
	////
	idle->id = PCB::idCnt++;
	unsigned* st = new unsigned[4096];
	idle->timeSlice = 1;
	st[4095] = 0x200;
	st[4094] = FP_SEG(PCB::idleFun);
	st[4093] = FP_OFF(PCB::idleFun);
	idle->sp = FP_OFF(st + 4084);
	idle->ss = FP_SEG(st + 4084);
	idle->bp = FP_OFF(st + 4084);
	idle->stack = st;
	idle->status = IDLE;
	idle->myThread = 0;
	idle->waitingToComplete = new Lista();
	asm sti
	////
}

void restore(){
	asm {
		cli
		push es
		push ax

		mov ax,0
		mov es,ax

		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}
}

void dispatch() {
	/*asm cli
	context_switch_on_demand = 1;
	timer();
	asm sti*/
	lock
	context_switch_on_demand = 1;
	timer();
	unlock
}


