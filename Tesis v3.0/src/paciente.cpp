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
		_duracionesT.push_back(_infoTratamientos[i][2]);
	}
}

int Paciente::nCitas(int idE) const
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

int Paciente::id(void) const
{
	return _id;
}

string Paciente::nombre(void) const {
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

void Paciente::insertarCita(Cita c) {
	_listaCitas.push_back(c);
}

void Paciente::listaCitas(vector<Cita> lc) {
	_listaCitas = lc;
}

vector<Cita> Paciente::listaCitas(void) const{
	return _listaCitas;
}

void Paciente::disponibilidad(vector<int> d)
{
	_disponibilidad = d;
}

vector<int> Paciente::disponibilidad(void)
{
	return _disponibilidad;
}

int Paciente::duracionCitTrat(int idE)
{
	int duracion;
	for(int i=0; i< (int) _infoTratamientos.size(); i++){
		if(_infoTratamientos[i][0]==idE){
			duracion = _infoTratamientos[i][2];
			break;
		}
	}
	return duracion;
}

vector<int> Paciente::duracionesT(void)
{
	return _duracionesT;
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
