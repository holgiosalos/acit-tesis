/*
 * AsignadorOptions.h
 *
 *  Created on: 22/02/2012
 *      Author: Giovanny
 */

#ifndef ASIGNADOROPTIONS_H
#define ASIGNADOROPTIONS_H

#include <gecode/driver.hh>
#include <gecode/int.hh>

#include <iostream>

#include "archivo.h"
#include "paciente.h"
#include "especialista.h"
#include "especialidad.h"

using namespace Gecode;
using namespace std;


class AsignadorOptions : public Options {

private:
	Archivo _reader;
	int _totalCitas;
	int _totalSlots;
	int _slotsDia;
	int _slotsIntervalo;
	int _totalEspecialistas;
	vector<Especialidad> _listaEspecialidades;
	vector<Paciente> _listaPacientes;
	vector<Especialista> _listaEspecialistas;
	IntSet _listaCodEspecialistas;

public:
	AsignadorOptions(const char* s);

	void reader(Archivo lector);
	Archivo getReader(void) const;

	void totalCitas(int tC);
	int totalCitas(void) const;

	void slotsIntervalo(int sI);
	int slotsIntervalo(void) const;

	void calcularTotalSlots(int intervalos);
	int totalSlots(void) const;

	void calcularSlotsDia(int intervalosD);
	int slotsDia(void) const;

	void totalEspecialistas(int tE);
	int totalEspecialistas(void) const;

	vector<Especialidad> listaEspecialidades() const;
	vector<Especialista> listaEspecialistas() const;
	vector<Paciente> listaPacientes() const;

	void iniciar(void);
	int* settingCodigos(void);

	void listaCodEspecialistas(IntSet ids);
	IntSet listaCodEspecialistas(void) const;

	virtual void help(void){
		Options::help();
		cerr <<"\t-file (string) : " << endl <<"\t\tArchivo de entrada" << endl;
	}
};




#endif /* ASIGNADOROPTIONS_H */
