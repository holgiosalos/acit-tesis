#include "especialista.h"

Especialista::Especialista(int i, int c, int iE)
{
	_id = i;
	_capacidad = c;
	_idEspecialidad = iE;
}

Especialista::Especialista(int i, int c, int iE, vector<int> d)
{
	_id = i;
	_capacidad = c;
	_idEspecialidad = iE;
	_disponibilidad = d;
}

void Especialista::idCitas(vector<int> citas)
{
	_idCitas = citas;
}

vector<int> Especialista::idCitas()
{
	return _idCitas;
}

void Especialista::idEspecialidad(int idE)
{
	_idEspecialidad = idE;
}

int Especialista::idEspecialidad()
{
	return _idEspecialidad;
}

void Especialista::id(int i)
{
	_id = i;
}

int Especialista::id()
{
	return _id;
}

void Especialista::capacidad(int c)
{
	_capacidad = c;
}

int Especialista::capacidad()
{
	return _capacidad;
}

void Especialista::disponibilidad(int d[], int size)
{
	_disponibilidad.resize(size);
	copy(d, d+size, _disponibilidad.begin());
}

vector<int> Especialista::disponibilidad()
{
	return _disponibilidad;
}


