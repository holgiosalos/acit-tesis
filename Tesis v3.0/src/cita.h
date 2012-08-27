/*
 * cita.h
 *
 *  Created on: 18/08/2012
 *      Author: giovanny
 */

#ifndef CITA_H
#define CITA_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include "CalculaFechas.h"

using namespace std;

class Cita {
private:
	int _idEspecialidad;
	int _inicio;
	int _fin;
	int _dia;
	int _diasSemana;
	tm_fecha _fechaCita;
	tm_fecha _fechaGnrlInicio;

public:
	Cita(int idE, int i, int f, int d);

	int idEspecialidad(void) const;
	int inicio(void) const;
	int fin(void) const;
	int dia(void) const;
	tm_fecha fecha(void) const;

	void setDiasSemana(int ds);
	void calcularFechaCita(int d);
};

#endif
