/*
 * El codigo fuente de está clase esta basado en el presentado en el siguiente link:
 * http://foros.softonic.com/programacion/programa-c-fechas-48663#post365525
 */

#ifndef CALCULAFECHAS_H
#define CALCULAFECHAS_H

struct tm_fecha {
	int dia;
	int mes;
	int anio;
};

#include <ctime>
#include <cstdlib>
#include <iostream>

using namespace std;

class CalculaFechas {
private:
	tm_fecha _fechaInicioCitas;
	bool isFechaInicio;

public:
	CalculaFechas();

	bool bisiesto(int anio);
	int dias_mes(tm_fecha fecha);
	bool validarFecha(tm_fecha fecha);
	int cuenta_dias(tm_fecha fecha1, tm_fecha fecha2);
	tm_fecha sumarDias(tm_fecha fecha, int x);
	tm_fecha restarDias(tm_fecha fecha, int x);
	int dia_semana(tm_fecha fecha2);

	void calcularFechaInicioCitas(void);
	tm_fecha fechaInicioCitas(void);
};

#endif /* CALCULAFECHAS_H */
