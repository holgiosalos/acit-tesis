/*
 * cita.cpp
 *
 *  Created on: 18/08/2012
 *      Author: giovanny
 */

#include "cita.h"

Cita::Cita(int idE, int i, int f, int d){
	_inicio = i;
	_fin = f;
	_dia = d;
}

int Cita::idEspecialidad(void) const{
	return _idEspecialidad;
}
int Cita::inicio(void) const{
	return _inicio;
}
int Cita::fin(void) const{
	return _fin;
}
int Cita::dia(void) const{
	return _dia;
}
tm_fecha Cita::fecha(void) const{
	return _fechaCita;
}

void Cita::setDiasSemana(int ds){
	_diasSemana = ds;
}

void Cita::calcularFechaCita(int d){
	d--; //por si es el dia calculado por la clase escritura

	/* operaciones para calcular el dia en el calendario en el que se encuentra la cita */

	//para saber cuantos dias calendario han transcurrido desde el primer dia de asignacion de citas, hasta el dia en que se asigno ésta cita
	if( d >= _diasSemana ){
		//para indicar en que numero de semana se encuentra el dia
		int semana = int(floor(d/_diasSemana));
		d += ((7 - _diasSemana)*semana); //(7 - _diasSemana): Dias que no se asignan citas en la semana
	}

	//sumar d a fecha incial y tolisto
	CalculaFechas calculadora;
	_fechaGnrlInicio = calculadora.fechaInicioCitas();
	_fechaCita = calculadora.sumarDias(_fechaGnrlInicio, d);
}
