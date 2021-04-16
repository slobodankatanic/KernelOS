/*
 * handlQ.h
 *
 *  Created on: Jun 10, 2020
 *      Author: OS1
 */

#ifndef HANDLQ_H_
#define HANDLQ_H_

#include "thread.h"

class HandlersQueue {
	struct Elem {
		SignalHandler sadrzaj;
		Elem* sled, *pret;
		Elem(SignalHandler t) {
			sadrzaj = t; sled = 0; pret = 0;
		}
	} *prvi, *posl, *tek;
	int duz;
	void brisi();
	void kopiraj(const HandlersQueue&);
	void premesti(HandlersQueue&);
public:
	HandlersQueue() {
		prvi = 0; posl = 0; tek = 0;
		duz = 0;
	}
	~HandlersQueue() { brisi(); }
	HandlersQueue& dodajNaPoc(SignalHandler t);
	HandlersQueue& dodajNaKraj(SignalHandler t);
	int dohvBrojEl() const { return duz; }
	void postaviTekPoc() { tek = prvi; }
	void tekNaKraj() { tek = posl; }
	void naSled() {
		if (tek) tek = tek->sled;
	}
	void naPret() {
		if (tek) tek = tek->pret;
	}
	SignalHandler dohvTek() {
		return tek->sadrzaj;
	}
	HandlersQueue& obrisiTek();
	int imaTek() { return tek != 0; }
	void postTek(SignalHandler id) {
		if (tek != 0) {
			tek->sadrzaj = id;
		}
	}
};

#endif /* HANDLQ_H_ */
