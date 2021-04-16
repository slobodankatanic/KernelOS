/*
 * semQueue.h
 *
 *  Created on: Jun 15, 2020
 *      Author: OS1
 */

#ifndef SEMQUEUE_H_
#define SEMQUEUE_H_

#include "semaphor.h"

class SemQueue {
	struct Elem {
		KernelSem* sadrzaj;
		Elem* sled, *pret;
		Elem(KernelSem* t) {
			sadrzaj = t; sled = 0; pret = 0;
		}
	} *prvi, *posl, *tek;
	int duz;
	void brisi();
	void kopiraj(const SemQueue&);
	void premesti(SemQueue&);
public:
	SemQueue() {
		prvi = 0; posl = 0; tek = 0;
		duz = 0;
	}
	~SemQueue() { brisi(); }
	SemQueue& dodajNaPoc(KernelSem* t);
	SemQueue& dodajNaKraj(KernelSem* t);
	int dohvBrojEl() const { return duz; }
	void postaviTekPoc() { tek = prvi; }
	void tekNaKraj() { tek = posl; }
	void naSled() {
		if (tek) tek = tek->sled;
	}
	void naPret() {
		if (tek) tek = tek->pret;
	}
	KernelSem* dohvTek() {
		return tek->sadrzaj;
	}
	SemQueue& obrisiTek();
	int imaTek() { return tek != 0; }
};


#endif /* SEMQUEUE_H_ */
