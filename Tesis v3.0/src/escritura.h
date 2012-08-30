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
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <cmath>

#define TIXML_USE_STL
#include "tinyxml.h"

#include "especialidad.h"
#include "especialista.h"
#include "paciente.h"
#include "cita.h"
#include <ctime>

using namespace std;

class Escritura
{
private:
	string _directorioSalida;
	string _nombreArchivo;
	int _semanas;
	int _slotsIntervalo;
	int _slotsDia;
	int _intervalosDia;
	int _intervalosSemana;

	string itostr(int n) const;
public:
	Escritura(string directorio);

	void calcularNombreArchivo(void);
	void semanas(int s);
	int semanas(void);
	void slotsIntervalo(int sI);
	void slotsDia(int sD);
	void intervalosDia(int iD);
	void intervalosSemana(int iS);

	string determinarHora(int slot) const;
	string determinarDia(int slot) const;
	int determinarDiaInt(int slot) const;
	void transformarIntervalo(int i);
	vector<vector<string> > getDatosDisponibilidad(Especialista esp, int idE) const;

	void escribirXml(vector<Especialidad> listaEspecialidades) const;


};


#endif /* ESCRITURA_H_ */
