#include "especialidad.h"


Especialidad::Especialidad(int i, int nC, int nE, int nP)
{
	_id = i;
	_totalCitas = nC;
	_nEspecialistas = nE;
	_nPacientes = nP;
}

Especialidad::Especialidad(int i, int nC, int nE, int nP, vector<Especialista> esp, vector<Paciente> pac, vector<int> ids)
{
	_id = i;
	_totalCitas = nC;
	_nEspecialistas = nE;
	_nPacientes = nP;
	_especialistas = esp;
	_pacientes = pac;
	_idEspecialistas = ids;
}

int Especialidad::id() const
{
	return _id;
}

void Especialidad::totalCitas(int nC)
{
	_totalCitas = nC;
}

int Especialidad::totalCitas()
{
	return _totalCitas;
}

void Especialidad::nEspecialistas(int nE)
{
	_nEspecialistas = nE;
}

int Especialidad::nEspecialistas()
{
	return _nEspecialistas;
}

void Especialidad::nPacientes(int nP)
{
	_nPacientes = nP;
}

int Especialidad::nPacientes()
{
	return _nPacientes;
}

void Especialidad::especialistas(vector<Especialista> esp)
{
	_especialistas = esp;
}

void Especialidad::idEspecialistas(vector<int> ids)
{
	_idEspecialistas = ids;
}

int* Especialidad::idEspecialistas()
{
	int tam = (int)_idEspecialistas.size();
	int *idsArr;
	idsArr = (int*) malloc ( sizeof(int) * tam);
	for(int i=0; i<tam; i++){
		idsArr[i] = _idEspecialistas[i];
	}
	return idsArr;
}


vector<Especialista> Especialidad::especialistas()
{
	return _especialistas;
}

void Especialidad::pacientes(vector<Paciente> esp)
{
	_pacientes = esp;
}

vector<Paciente> Especialidad::pacientes() const
{
	return _pacientes;
}


