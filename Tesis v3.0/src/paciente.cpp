#include "paciente.h"


Paciente::Paciente(int i, int nT, vector<int> nC, vector<int> iE, vector<int> d)
{
	_id = i;
	_nTratamientos = nT;
	_nCitas = nC;
	_idEspecialidades = iE;
	_disponibilidad = d;
}

void Paciente::idCitas(vector<int> citas)
{
	_idCitas = citas;
}

vector<int> Paciente::idCitas(void)
{
	return _idCitas;
}

void Paciente::idEspecialista(vector<int> idEsp)
{
	_idEspecialista = idEsp;
}

vector<int> Paciente::idEspecialista(void)
{
	return _idEspecialista;
}

void Paciente::nCitas(vector<int> c)
{
	_nCitas = c;
}

vector<int> Paciente::nCitas(void) const
{
	return _nCitas;
}

void Paciente::nTratamientos(int nT)
{
	_nTratamientos = c;
}

int Paciente::nTratamientos(void) const
{
	return _nTratamientos;
}

void Paciente::id(int i)
{
	_id = i;
}

int Paciente::id(void)
{
	return _id;
}

void Paciente::idEspecialidades(vector<int> idE)
{
	_idEspecialidades = idE;
}

vector<int> Paciente::idEspecialidad(void) const
{
	return _idEspecialidades;
}

void Paciente::disponibilidad(vector<int> d)
{
	_disponibilidad = d;
}

vector<int> Paciente::disponibilidad(void)
{
	return _disponibilidad;
}

bool Paciente::buscaEspecialidadPac(int id)
{
	vector<int>::iterator it;
	it = find(_idEspecialidades.begin(), _idEspecialidades.end(), id);
	if(it==id){
		return true;
	}else{
		return false;
	}
}
