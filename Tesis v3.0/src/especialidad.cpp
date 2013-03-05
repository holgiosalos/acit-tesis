#include "especialidad.h"

Especialidad::Especialidad(int i, string n, int c, int nC, int dC, int nE, int nP,
		vector<Especialista>* esp, vector<Paciente> pac,
		vector<int> idsE, vector<int> idsP)
{
	_id = i;
	_nombre = n;
	_capacidad = c;
	_totalCitas = nC;
	_duracionCitas = dC;
	_nEspecialistas = nE;
	_nPacientes = nP;
	_especialistas = esp;
	_pacientes = pac;
	_idEspecialistas = idsE;
	_idPacientes = idsP;
}

Especialidad::~Especialidad() {}

int Especialidad::id(void) const
{
	return _id;
}

string Especialidad::nombre(void) const
{
	return _nombre;
}

int Especialidad::capacidad(void)
{
	return _capacidad;
}

void Especialidad::totalCitas(int nC)
{
	_totalCitas = nC;
}

int Especialidad::totalCitas(void)
{
	return _totalCitas;
}

int Especialidad::duracionCitasMinutos(void) const
{
	return _duracionCitas;
}

int Especialidad::duracionCitasSlots(void)
{
	//El numero 5 se refiere a la cantidad de minutos que posee cada slot
	return int(duracionCitasMinutos() / 5);
}

void Especialidad::nEspecialistas(int nE)
{
	_nEspecialistas = nE;
}

int Especialidad::nEspecialistas(void)
{
	return _nEspecialistas;
}

void Especialidad::nPacientes(int nP)
{
	_nPacientes = nP;
}

int Especialidad::nPacientes(void)
{
	return _nPacientes;
}

void Especialidad::especialistas(vector<Especialista>* esp)
{
	_especialistas = esp;
}

vector<Especialista>* Especialidad::especialistas(void) const
{
	return _especialistas;
}

string Especialidad::especialistasString(void)
{
	string resultado="[";
	for(int i=0; i<(int)_especialistas->size(); i++){
		resultado += _especialistas->at(i).id();
		if(i != (int)_especialistas->size()){
			resultado += ", ";
		}else{
			resultado +="]";
		}
	}
	return resultado;
}

void Especialidad::idEspecialistas(vector<int> ids)
{
	_idEspecialistas = ids;
}

int* Especialidad::idEspecialistasArray(void)
{
	int tam = (int)_idEspecialistas.size();
	int *idsArr;
	idsArr = (int*) malloc ( sizeof(int) * tam);
	for(int i=0; i<tam; i++){
		idsArr[i] = _idEspecialistas[i];
	}
	return idsArr;
}

vector<int> Especialidad::idEspecialistasVector(void)
{
	return _idEspecialistas;
}

void Especialidad::idPacientes(vector<int> ids)
{
	_idPacientes = ids;
}

int* Especialidad::idPacientesArray(void)
{
	int tam = (int)_idPacientes.size();
	int *idsArr;
	idsArr = (int*) malloc ( sizeof(int) * tam);
	for(int i=0; i<tam; i++){
		idsArr[i] = _idPacientes[i];
	}
	return idsArr;
}

vector<int> Especialidad::idPacientesVector(void)
{
	return _idPacientes;
}

Especialista Especialidad::buscarEspecialista(int idE)
{
	int i=0;
	for(; i<int(_especialistas->size()); i++){
		if(_especialistas->at(i).id() == idE){
			break;
		}
	}
	return _especialistas->at(i);
}

void Especialidad::actualizarEspecialista(Especialista esp){
	for(int i=0; i<int(_especialistas->size()); i++){
		if(_especialistas->at(i).id() == esp.id()){
			_especialistas->at(i) = esp;
			break;
		}
	}
}

void Especialidad::pacientes(vector<Paciente> pacs)
{
	_pacientes = pacs;
}

vector<Paciente> Especialidad::pacientes(void) const
{
	return _pacientes;
}

string Especialidad::pacientesString(void)
{
	string resultado="[";
	for(int i=0; i<(int)_pacientes.size(); i++){
		resultado += _pacientes[i].id();
		if(i != (int)_pacientes.size()){
			resultado += ", ";
		}else{
			resultado +="]";
		}
	}
	return resultado;
}
