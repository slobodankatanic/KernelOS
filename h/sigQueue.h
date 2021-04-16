/*
 * sigList.h
 *
 *  Created on: Jun 10, 2020
 *      Author: OS1
 */

#ifndef SIGQUEUE_H_
#define SIGQUEUE_H_

#include "thread.h"

class SignalQueue {
	struct Elem {
		SignalId sadrzaj;
		Elem* sled, *pret;
		Elem(SignalId t) {
			sadrzaj = t; sled = 0; pret = 0;
		}
	} *prvi, *posl, *tek;
	int duz;
	void brisi();
	void kopiraj(const SignalQueue&);
	void premesti(SignalQueue&);
public:
	SignalQueue() {
		prvi = 0; posl = 0; tek = 0;
		duz = 0;
	}
	~SignalQueue() { brisi(); }
	SignalQueue& dodajNaPoc(SignalId t);
	SignalQueue& dodajNaKraj(SignalId t);
	int dohvBrojEl() const { return duz; }
	void postaviTekPoc() { tek = prvi; }
	void tekNaKraj() { tek = posl; }
	void naSled() {
		if (tek) tek = tek->sled;
	}
	void naPret() {
		if (tek) tek = tek->pret;
	}
	SignalId dohvTek() {
		return tek->sadrzaj;
	}
	SignalQueue& obrisiTek();
	int imaTek() { return tek != 0; }
	void dodajNaPoz(int, SignalId);
	void postTek(SignalId id) {
		if (tek != 0) {
			tek->sadrzaj = id;
		}
	}
};


#endif /* SIGQUEUE_H_ */
