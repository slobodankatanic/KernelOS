/*
 * semQueue.cpp
 *
 *  Created on: Jun 15, 2020
 *      Author: OS1
 */

#include "semQueue.h"

SemQueue& SemQueue::dodajNaKraj(KernelSem* t) {
	Elem* e = new Elem(t);
	if (prvi == 0) prvi = e;
	else {
		posl->sled = e;
		e->pret = posl;
	}
	posl = e; duz++;
	return *this;
}

SemQueue& SemQueue::dodajNaPoc(KernelSem* t) {
	Elem* e = new Elem(t);
	if (prvi == 0) {
		prvi = e; posl = prvi;
	}
	else {
		e->sled = prvi;
		prvi->pret = e;
		prvi = e;
	}
	duz++;
	return *this;
}

SemQueue& SemQueue::obrisiTek() {
	if (duz == 0) return *this;
	if (tek->pret) {
		(tek->pret)->sled = tek->sled;
		if (tek->sled) (tek->sled)->pret = tek->pret;
		if (tek == posl) posl = tek->pret;
	}
	else {
		prvi = prvi->sled;
		if (prvi) prvi->pret = 0;
	}
	if (!prvi) posl = prvi;
	Elem* t = tek; tek = tek->sled;
	delete t;
	duz--;
	return *this;
}

void SemQueue::brisi() {
	while (prvi) {
		Elem* e = prvi;
		prvi = prvi->sled;
		delete e;
	}
	posl = 0; tek = 0; prvi = 0; duz = 0;
}
