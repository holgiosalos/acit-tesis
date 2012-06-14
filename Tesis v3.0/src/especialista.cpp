#include "especialista.h"

Especialista::Especialista(int i, int nE, vector<int> iE, vector<vector<int> > d)
{
	_id = i;
	_nEspecialidades = nE;
	_idEspecialidades = iE;
	_disponibilidades = d;
}

void Especialista::id(int i)
{
	_id = i;
}

int Especialista::id(void)
{
	return _id;
}

void Especialista::idCitas(vector<int> citas)
{
	_idCitas = citas;
}

vector<int> Especialista::idCitas(void)
{
	return _idCitas;
}

void Especialista::idEspecialidades(vector<int> idE)
{
	_idEspecialidades = idE;
}

vector<int> Especialista::idEspecialidades(void)
{
	return _idEspecialidad;
}

void Especialista::nEspecialidades(int nE)
{
	_nEspecialidades = c;
}

int Especialista::nEspecialidades(void)
{
	return _nEspecialidades;
}

void Especialista::disponibilidades(vector<vector<int> > d)
{
	_disponibilidades = d;
}

vector<vector<int> > Especialista::disponibilidades(void)
{
	return _disponibilidades;
}

bool Especialista::buscaEspecialidadProf(int id)
{
	vector<int>::iterator it;
	it = find(_idEspecialidades.begin(), _idEspecialidades.end(), id);
	if(it==id){
		return true;
	}else{
		return false;
	}
}

