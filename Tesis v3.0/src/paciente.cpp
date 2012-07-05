#include "paciente.h"


Paciente::Paciente(int i, string n, int nT, vector<vector<int> > iT, vector<int> d)
{
	_id = i;
	_nombre = n;
	_nTratamientos = nT;
	_infoTratamientos = iT;
	_disponibilidad = d;

	for(int i=0; i< (int) _infoTratamientos.size(); i++){
		_idEspecialidades.push_back(_infoTratamientos[i][0]);
	}
}

int Paciente::nCitas(int idE)
{
	int citas;
	for(int i=0; i< (int) _infoTratamientos.size(); i++){
		if(_infoTratamientos[i][0]==idE){
			citas = _infoTratamientos[i][1];
			break;
		}
	}
	return citas;
}

void Paciente::id(int i)
{
	_id = i;
}

int Paciente::id(void)
{
	return _id;
}

string Paciente::nombre(void) {
	return _nombre;
}

void Paciente::nTratamientos(int nT)
{
	_nTratamientos = nT;
}

int Paciente::nTratamientos(void)
{
	return _nTratamientos;
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
	bool aux=false;
	for(int i=0; i<(int)_idEspecialidades.size(); i++){
		if(_idEspecialidades[i]==id){
			aux=true;
			break;
		}
	}
	return aux;
}
