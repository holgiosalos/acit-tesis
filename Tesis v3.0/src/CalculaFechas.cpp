#include "CalculaFechas.h"

CalculaFechas::CalculaFechas() { }

CalculaFechas::~CalculaFechas() { }

bool CalculaFechas::bisiesto(int anio) const{
	// Finalidad: Calcula si un año es bisiesto
	// Argumentos: Se le pasará un año (int). No será necesario enviarle una fecha entera
	// Devuelve: true si es bisiesto, false si no lo es

	if (anio % 400 == 0 || (anio % 4 == 0 && anio % 100 != 0)){
		return true;
	}
	else{
		return false;
	}
}

int CalculaFechas::dias_mes(tm_fecha fecha) const{
	// Finalidad: Calcula los días de un mes
	// Argumentos: Se le pasará una fecha (tm_fecha).
	// Devuelve: Los días del mes de la fecha pasada (int).

	int dias=-1;

	switch (fecha.mes) {
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			dias = 31;
			break;

		case 4:
		case 6:
		case 9:
		case 11:
			dias = 30;
			break;

		case 2:
			if (bisiesto(fecha.anio)){ dias = 29; }
			else { dias = 28; }
			break;
	}
	return dias;
}

bool CalculaFechas::validarFecha(tm_fecha fecha) {
	// Finalidad: Determina si la fecha pasada existe
	// Argumentos: Se le pasará una fecha (tm_fecha).
	// Devuelve: true si la fecha es válida, false si no lo es.

	if (fecha.mes > 0 && fecha.mes <= 12) {
		if (fecha.dia > 0 && fecha.dia <= dias_mes(fecha)){
			return true;
		}
		else{
			return false;
		}
	} else{
		return false;
	}
}

int CalculaFechas::cuenta_dias(tm_fecha fecha1, tm_fecha fecha2) {
	// Finalidad: Calcular los días que hay entre dos fechas.
	// Argumentos: Se le pasará dos fechas (tm_fecha).
	// Devuelve: La distancia en días entre las dos fechas (int)
	// Nota: fecha1 deberá ser anterior a fecha2

	int acum = 0;

	while (true) {
		if (fecha1.anio == fecha2.anio && fecha1.mes == fecha2.mes){ break; }

		acum = acum + (dias_mes(fecha1) - fecha1.dia);

		if (fecha1.mes == 12) {
			fecha1.mes = 1;
			fecha1.anio++;
		} else{
			fecha1.mes++;
		}

		fecha1.dia = 0;
	}
	acum = acum + (fecha2.dia - fecha1.dia);
	return acum;
}

tm_fecha CalculaFechas::sumarDias(tm_fecha fecha, int x) const{
	// Finalidad: Suma a una fecha inicial una cantidad de días.
	// Argumentos: Una fecha (tm_fecha) y una cantidad de días (int).
	// Devuelve: La fecha final con los días sumados (tm_fecha).

	for (; x > 0; x--) {
		if (fecha.dia < dias_mes(fecha)){
			fecha.dia++;
		} else {
			if (fecha.mes == 12) {
				fecha.mes = 1;
				fecha.anio++;
			} else {
				fecha.mes++;
			}
			fecha.dia = 1;
		}
	}
	return fecha;
}

tm_fecha CalculaFechas::restarDias(tm_fecha fecha, int x) {
	// Finalidad: Resta a una fecha inicial una cantidad de días.
	// Argumentos: Una fecha (tm_fecha) y una cantidad de días (int).
	// Devuelve: La fecha final con los días restados (tm_fecha).
	for (; x > 0; x--) {
		if (fecha.dia > 1)
			fecha.dia--;
		else {
			if (fecha.mes == 1) {
				fecha.mes = 12;
				fecha.anio--;
			} else{
				fecha.mes--;
			}
			fecha.dia = dias_mes(fecha);
		}
	}
	return fecha;
}

int CalculaFechas::dia_semana(tm_fecha fecha2) {
	// Finalidad: Halla el día de la semana de una fecha (lunes, martes...)
	// Argumentos: Una fecha (tm_fecha).
	// Devuelve: 1 para lunes, 2 para martes...
	int diaSem = -1;
	tm_fecha fecha1 = { 14, 9, 1752 };
	switch (cuenta_dias(fecha1, fecha2) % 7) {
		case 3:
			diaSem = 7; // Domingo
			break;
		case 4:
			diaSem = 1; // Lunes
			break;
		case 5:
			diaSem = 2; // Martes
			break;
		case 6:
			diaSem = 3; // Miércoles
			break;
		case 0:
			diaSem = 4; // Jueves
			break;
		case 1:
			diaSem = 5; // Viernes
			break;
		case 2:
			diaSem = 6; // Sábado
			break;
	}
	return diaSem;
}

void CalculaFechas::calcularFechaInicioCitas(void){
	time_t tSac 	= time(NULL);       // instante actual
	struct tm* tmP 	= localtime(&tSac);

	tm_fecha fechaI = {tmP->tm_mday, tmP->tm_mon+1, tmP->tm_year+1900};

	int x; //representa los dias a sumar a la fecha actual
	if(tmP->tm_wday==0){
		x = 1;
	}else{
		x = 8 - tmP->tm_wday;
	}

	_fechaInicioCitas = sumarDias(fechaI, x);
	isFechaInicio = true;
}

tm_fecha CalculaFechas::fechaInicioCitas(void) {
	if(isFechaInicio != true){
		calcularFechaInicioCitas();
	}
	return _fechaInicioCitas;
}
