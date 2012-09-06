#include "especialista.h"

Especialista::Especialista(int i, string n, int nE, vector<int> iE, vector<vector<int> > hsA)
{
	_id = i;
	_nombre = n;
	_nEspecialidades = nE;
	_idEspecialidades = iE;
	_horariosAtencion = hsA;
}

Especialista::~Especialista() {}

void Especialista::id(int i) {
	_id = i;
}

int Especialista::id(void) const {
	return _id;
}

string Especialista::nombre(void) const {
	return _nombre;
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
	return _idEspecialidades;
}

void Especialista::nEspecialidades(int nE)
{
	_nEspecialidades = nE;
}

int Especialista::nEspecialidades(void)
{
	return _nEspecialidades;
}

void Especialista::insertarPaciente(Paciente pac) {
	_pacientes.push_back(pac);
}

void Especialista::pacientes(vector<Paciente> pac) {
	_pacientes = pac;
}

vector<Paciente> Especialista::pacientes(void) const{
	return _pacientes;
}

void Especialista::horariosAtencion(vector<vector<int> > hsA)
{
	_horariosAtencion = hsA;
}

vector<vector<int> > Especialista::horariosAtencion(void)
{
	return _horariosAtencion;
}

vector<int> Especialista::horariosAtencionEsp(int idE)
{
	vector<int> hrsAtencion;
	for(int i=0; i< (int) _idEspecialidades.size(); i++){
		if(_idEspecialidades[i] == idE){
			hrsAtencion = _horariosAtencion[i];
			break;
		}
	}
	return hrsAtencion;
}

bool Especialista::buscaEspecialidadProf(int id)
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

