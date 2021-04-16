/*
 * handlQ.cpp
 *
 *  Created on: Jun 10, 2020
 *      Author: OS1
 */

#include "handlQ.h"

HandlersQueue& HandlersQueue::dodajNaKraj(SignalHandler t) {
	Elem* e = new Elem(t);
	if (prvi == 0) prvi = e;
	else {
		posl->sled = e;
		e->pret = posl;
	}
	posl = e; duz++;
	return *this;
}

HandlersQueue& HandlersQueue::dodajNaPoc(SignalHandler t) {
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

HandlersQueue& HandlersQueue::obrisiTek() {
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

void HandlersQueue::brisi() {
	while (prvi) {
		Elem* e = prvi;
		prvi = prvi->sled;
		delete e;
	}
	posl = 0; tek = 0; prvi = 0; duz = 0;
}
