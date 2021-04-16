/*
 * idle.h
 *
 *  Created on: Apr 9, 2020
 *      Author: OS1
 */

#ifndef IDLE_H_
#define IDLE_H_

#include "thread.h"

class Idle : public Thread {
protected:
	virtual void run() {while(1);}
};

#endif /* IDLE_H_ */
