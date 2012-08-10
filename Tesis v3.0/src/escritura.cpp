/*
 * escritura.cpp
 *
 *  Created on: 8/07/2012
 *      Author: giovanny
 */

#include "escritura.h"

Escritura::Escritura(int sI, int iD, int iS) {
	_slotsIntervalo = sI;
	_intervalosDia = iD;
	_intervalosSemana = iS;
}

void Escritura::slotsIntervalo(int sI) {
	_slotsIntervalo = sI;
}

int Escritura::slotsIntervalo(void) {
	return _slotsIntervalo;
}

void Escritura::intervalosDia(int iD) {
	_intervalosDia = iD;
}

int Escritura::intervalosDia(void) {
	return _intervalosDia;
}

void Escritura::intervalosSemana(int iS) {
	_intervalosSemana = iS;
}

int Escritura::intervalosSemana(void) {
	return _intervalosSemana;
}

int Escritura::determinarHora(int slot) {
	int hora = floor((slot / _slotsIntervalo) % _intervalosDia) + 7;

	int minutos = (slot % _slotsIntervalo) * 5;

	return (hora * 100) + minutos;
}

void Escritura::transformarIntervalo(int i) { // O.K

	string strCita;
	int hora = 0;
	string strHora;
	int dia = 0;
	string strDia;
	//	int semana = 0; string strSemana;

	hora = i % 8;
	dia = (int) floor(i / 8);
	//semana = (int)i/40;
	//semana = semana+1;


	switch (dia) {
	case 0:
		strDia = "Lunes";
		break;
	case 1:
		strDia = "Martes";
		break;
	case 2:
		strDia = "Miercoles";
		break;
	case 3:
		strDia = "Jueves";
		break;
	case 4:
		strDia = "Viernes";
		break;
	default:
		strDia = "Invalido";
		break;
	}

	switch (hora) {
	case 0:
		strHora = "[08:00 - 09:00)";
		break;
	case 1:
		strHora = "[09:00 - 10:00)";
		break;
	case 2:
		strHora = "[10:00 - 11:00)";
		break;
	case 3:
		strHora = "[11:00 - 12:00)";
		break;
	case 4:
		strHora = "[14:00 - 15:00)";
		break;
	case 5:
		strHora = "[15:00 - 16:00)";
		break;
	case 6:
		strHora = "[16:00 - 17:00)";
		break;
	case 7:
		strHora = "[17:00 - 18:00)";
		break;
	default:
		strHora = "Invalido";
		break;
	}

	/*strSemana = itostr(semana);*/
	strCita = /*"Semana "+strSemana+" - "+*/"Dia: " + strDia + " - " + "Hora: "
			+ strHora;
	cout << strCita << endl;
	//return strCita;
}
