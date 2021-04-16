/*
 * sigList.cpp
 *
 *  Created on: Jun 10, 2020
 *      Author: OS1
 */

#include "sigQueue.h"

SignalQueue& SignalQueue::dodajNaKraj(SignalId t) {
	Elem* e = new Elem(t);
	if (prvi == 0) prvi = e;
	else {
		posl->sled = e;
		e->pret = posl;
	}
	posl = e; duz++;
	return *this;
}

SignalQueue& SignalQueue::dodajNaPoc(SignalId t) {
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

SignalQueue& SignalQueue::obrisiTek() {
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

void SignalQueue::brisi() {
	while (prvi) {
		Elem* e = prvi;
		prvi = prvi->sled;
		delete e;
	}
	posl = 0; tek = 0; prvi = 0; duz = 0;
}

void SignalQueue::dodajNaPoz(int p, SignalId id) {
	if (p < 0) return;
	Elem* e = new Elem(id);
	int poz = 0;
	Elem* tmp = prvi;
	while (tmp && poz < p) {
		tmp = tmp->sled;
		poz++;
	}
	if (tmp == 0) {
		if (prvi == 0) {
			prvi = e;
		}
		else {
			posl->sled = e;
			e->pret = posl;
		}
		posl = e;
	}
	else {
		if (tmp == prvi) {
			e->sled = prvi;
			prvi->pret = e;
			prvi = e;
		}
		else {
			tmp->pret->sled = e;
			e->sled = tmp;
			e->pret = tmp->pret;
			tmp->pret = e;
		}
	}
	duz++;
}
