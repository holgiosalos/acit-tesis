#include "paciente.h"

Paciente::Paciente(int i, int nC)
{
	_id = i;
	_nCitas = nC;
}

Paciente::Paciente(int i, int nC, int iE)
{
	_id = i;
	_nCitas = nC;
	_idEspecialidad = iE;
}

Paciente::Paciente(int i, int nC, int iE, vector<int> d)
{
	_id = i;
	_nCitas = nC;
	_idEspecialidad = iE;
	_disponibilidad = d;
}

void Paciente::idCitas(vector<int> citas)
{
	_idCitas = citas;
}

vector<int> Paciente::idCitas()
{
	return _idCitas;
}

void Paciente::idEspecialista(vector<int> idEsp)
{
	_idEspecialista = idEsp;
}

vector<int> Paciente::idEspecialista()
{
	return _idEspecialista;
}

void Paciente::nCitas(int c)
{
	_nCitas = c;
}

int Paciente::nCitas(void) const
{
	return _nCitas;
}

void Paciente::id(int i)
{
	_id = i;
}

int Paciente::id()
{
	return _id;
}

void Paciente::idEspecialidad(int idE)
{
	_idEspecialidad = idE;
}

int Paciente::idEspecialidad(void) const
{
	return _idEspecialidad;
}

void Paciente::disponibilidad(int d[], int size)
{
	_disponibilidad.resize(size);
	copy(d, d+size, _disponibilidad.begin());
}

vector<int> Paciente::disponibilidad()
{
	return _disponibilidad;
}
