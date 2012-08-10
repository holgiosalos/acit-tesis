/*
 * escritura.h
 *
 *  Created on: 8/07/2012
 *      Author: giovanny
 */

#ifndef ESCRITURA_H
#define ESCRITURA_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

class Escritura
{
private:
	string _directorioSalida;
	int _slotsIntervalo;
	int _intervalosDia;
	int _intervalosSemana;

public:
	Escritura(int sI, int iD, int iS);

	void slotsIntervalo(int sI);
	int slotsIntervalo(void);
	void intervalosDia(int iD);
	int intervalosDia(void);
	void intervalosSemana(int iS);
	int intervalosSemana(void);

	int determinarHora(int i);
	void transformarIntervalo(int i);
};


#endif /* ESCRITURA_H_ */
