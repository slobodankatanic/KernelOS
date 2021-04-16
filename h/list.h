/*
 * PCBList.h
 *
 *  Created on: Apr 9, 2020
 *      Author: OS1
 */

#ifndef _LISTA_H_
#define _LISTA_H_

#include "PCB.h"

class Lista {
	struct Elem {
		PCB* sadrzaj;
		Elem* sled, *pret;
		Elem(PCB* t) {
			sadrzaj = t; sled = 0; pret = 0;
		}
	} *prvi, *posl, *tek;
	int duz;
	void brisi();
	void kopiraj(const Lista&);
	void premesti(Lista&);
public:
	Lista() {
		prvi = 0; posl = 0; tek = 0;
		duz = 0;
	}
	~Lista() { brisi(); }
	Lista& dodajNaPoc(PCB* t);
	Lista& dodajNaKraj(PCB* t);
	int dohvBrojEl() const { return duz; }
	void postaviTekPoc() { tek = prvi; }
	void tekNaKraj() { tek = posl; }
	void naSled() {
		if (tek) tek = tek->sled;
	}
	void naPret() {
		if (tek) tek = tek->pret;
	}
	PCB* dohvTek() {
		return tek->sadrzaj;
	}
	Lista& obrisiTek();
	int imaTek() { return tek != 0; }
};

#endif
