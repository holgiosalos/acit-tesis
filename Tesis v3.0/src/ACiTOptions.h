/*
 * AsignadorOptions.h
 *
 *  Created on: 22/02/2012
 *      Author: Giovanny
 */

#ifndef ACITOPTIONS_H
#define ACITOPTIONS_H

#include <gecode/driver.hh>
#include <gecode/int.hh>

#include <iostream>
#include <string>

#include "lectura.h"
#include "paciente.h"
#include "especialista.h"
#include "especialidad.h"

using namespace Gecode;
using namespace std;


class ACiTOptions : public Options {

private:
	Lectura _reader;
	string _file;
	bool _preferencia;
	int _semanas;
	int _totalCitas;
	int _intervalosSemana;
	int _makespan;
	int _slotsDia;
	int _intervalosDia;
	int _slotsIntervalo;
	int _totalEspecialistas;
	int _nPacientesPreferencia;
	int _mayorIdProfesional;
	int _cutoff;
	double _factor;
	int _parametrer;
	bool _isLuby;
	vector<Especialidad>* _listaEspecialidades;
	vector<Paciente> _listaPacientes;
	vector<Especialista>* _listaEspecialistas;
	IntSet _listaCodEspecialistas;

public:
	ACiTOptions(const char* s);
	~ACiTOptions();

	void reader(Lectura lector);
	Lectura reader(void) const;

	void file(string dirfile);
	string file(void) const;

	void preferencia(bool pref);
	bool preferencia(void) const;

	void slotsIntervalo(int sI);
	int slotsIntervalo(void) const;

	void intervalosDia(int intervalosD);
	int intervalosDia() const;
	void calcularSlotsDia();
	int slotsDia(void) const;

	void semanas(int opcion); //Semanas disponibles para asignar el total de las citas
	int semanas(void) const;

	void intervalosSemana(int intervalos);
	int intervalosSemana(void) const;
	void calcularMakespan();
	int makespan() const;

	void totalCitas(int tC);
	int totalCitas(void) const;

	void totalEspecialistas(int tE);
	int totalEspecialistas(void) const;

	//Funciones para la optimizacion de la restriccion de preferencia
	void nPacientesPreferencia(int n);
	int nPacientesPreferencia() const;
	void mayorIdProfesional(int m);
	int mayorIdProfesional() const;

	vector<Especialidad>* listaEspecialidades() const;
	vector<Especialista>* listaEspecialistas() const;
	vector<Paciente> listaPacientes() const;

	void iniciar(void);
	int* settingCodigos(void);

	void listaCodEspecialistas(IntSet ids);
	IntSet listaCodEspecialistas(void) const;

	void cutoff(int c);
	int cutoff(void) const;

	void factor(double fc);
	double factor(void) const;

	void cutoffLuby(int p);
	int cutoffLuby(void) const;

	void isLuby(bool value);
	bool isLuby(void) const;

	virtual void help(void);
	void parse(int & argc, char* argv[]);
};

#endif /* ASIGNADOROPTIONS_H */
