/*
 * AsignadorOptions.cpp
 *
 *  Created on: 22/02/2012
 *      Author: Giovanny
 */

#include "AsignadorOptions.h"

AsignadorOptions::AsignadorOptions(const char* s) :
	Options(s) {
}

void AsignadorOptions::reader(Archivo lector) {
	_reader = lector;
}

Archivo AsignadorOptions::getReader(void) const {
	return static_cast<Archivo> (_reader);
}

void AsignadorOptions::slotsIntervalo(int sI) {
	_slotsIntervalo = sI;
}

int AsignadorOptions::slotsIntervalo(void) const{
	return static_cast<int> (_slotsIntervalo);
}


void AsignadorOptions::totalCitas(int tC) {
	_totalCitas = tC;
}

int AsignadorOptions::totalCitas(void) const {
	return static_cast<int> (_totalCitas);
}

void AsignadorOptions::calcularTotalSlots(int intervalos) {
	//nSlotsHora es el valor en el que se divide cada intervalo (hora).
	_totalSlots = intervalos * _slotsIntervalo;
}

int AsignadorOptions::totalSlots(void) const {
	return static_cast<int> (_totalSlots);
}

void AsignadorOptions::calcularSlotsDia(int intervalosD) {
	//nSlotsHora es el valor en el que se divide cada intervaloD (hora).
	_slotsDia = intervalosD * _slotsIntervalo;
}

int AsignadorOptions::slotsDia(void) const {
	return static_cast<int> (_slotsDia);
}

void AsignadorOptions::totalEspecialistas(int tE) {
	_totalEspecialistas = tE;
}

int AsignadorOptions::totalEspecialistas(void) const {
	return static_cast<int> (_totalEspecialistas);
}

vector<Especialidad> AsignadorOptions::listaEspecialidades() const {
	return static_cast<vector<Especialidad> > (_listaEspecialidades);
}

vector<Especialista> AsignadorOptions::listaEspecialistas() const {
	return static_cast<vector<Especialista> > (_listaEspecialistas);
}

vector<Paciente> AsignadorOptions::listaPacientes() const {
	return static_cast<vector<Paciente> > (_listaPacientes);
}

void AsignadorOptions::listaCodEspecialistas(IntSet ids) {
	_listaCodEspecialistas = ids;
}

IntSet AsignadorOptions::listaCodEspecialistas(void) const {
	return static_cast<IntSet> (_listaCodEspecialistas);
}

void AsignadorOptions::iniciar(void) {
	//Agregamos los pacientes del archivo al vector listaPacientes
	vector<vector<int> > infoEs; //guarda los datos de todas las especialidades asociadas al paciente i
	vector<int> aux(2); //guarda el id y el numero de citas de cada especialidad asociada al paciente i

	for (int i = 0; i < _reader.numPacientes(); i++) {
		for(int e=0; e<_reader.numTratamientosPac(i); e++){
			aux[0] = _reader.obtenerEspecialidadPac(i, e);
			aux[1] = _reader.numCitas(i, e);
			infoEs.push_back(aux);
			aux.clear();
		}

		_listaPacientes.push_back(Paciente(_reader.idPaciente(i),
				_reader.numTratamientosPac(i), infoEs,
				_reader.dispPaciente(i)));

		infoEs.clear();
	}

	//Agregamos los especialistas del archivo al vector listaEspecialistas
	for (int j = 0; j < _reader.numEspecialistas(); j++) {
		_listaEspecialistas.push_back(Especialista(_reader.idEspecialista(j),
				_reader.numEspProfesional(j), _reader.especialidadesProf(j),
				_reader.dispEspecialista(j)));
	}
	//Agregamos los datos de las especialidades al vector listaEspecialidades
	vector<Especialista> auxE;
	vector<int> auxIdsE;
	vector<Paciente> auxP;
	vector<int> auxIdsP;
	for (int x = 0; x < _reader.numEspecialidades(); x++) {
		for (int j = 0; j < (int) _listaEspecialistas.size(); j++) {
			if (_listaEspecialistas[j].buscaEspecialidadProf(_reader.idEspecialidad(x))) {
				auxE.push_back(_listaEspecialistas[j]);
				auxIdsE.push_back(_listaEspecialistas[j].id());
			}
		}

		for (int p = 0; p < (int) _listaPacientes.size(); p++) {
			if (_listaPacientes[p].buscaEspecialidadPac(_reader.idEspecialidad(x))) {
				auxP.push_back(_listaPacientes[p]);
				auxIdsP.push_back(_listaPacientes[p].id());
			}
		}

		if (_reader.numPacEsp(x) == (int) auxP.size()) {
			_listaEspecialidades.push_back(Especialidad(
						_reader.idEspecialidad(x), _reader.nomEspecialidad(_reader.idEspecialidad(x)),
						_reader.numCitasEsp(x), _reader.duracionCita(x), auxE.size(), _reader.numPacEsp(x),
						auxE, auxP, auxIdsE, auxIdsP));
			auxE.clear();
			auxP.clear();
			auxIdsE.clear();
			auxIdsP.clear();
		} else {
			cout
					<< "ERROR: numero de pacientes en lector y en el vector no coinciden"
					<< endl;
			exit(1);
		}
	}
}

int* AsignadorOptions::settingCodigos(void) {
	int a = 0;
	int tam = _reader.numEspecialistas();
	int* codigos;
	codigos = (int*) malloc(sizeof(int) * tam);
	if (_listaEspecialidades.size() > 0) {
		for (int i = 0; i < (int) _listaEspecialidades.size(); i++) {
			int* aux;
			aux = (int*) malloc(sizeof(int)	* _listaEspecialidades[i].nEspecialistas());
			aux = _listaEspecialidades[i].idEspecialistasArray();
			for (int j = 0; j < _listaEspecialidades[i].nEspecialistas(); j++) {
				codigos[a] = aux[j];
				a++;
			}
		}
		a = 0;
	} else {
		cout << "ERROR: Lista de Especialidades no inicializada" << endl;
		exit(1);
	}
	return codigos;
}

