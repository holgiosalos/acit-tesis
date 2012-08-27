/*
 * ACiTOptions.cpp
 *
 *  Created on: 22/02/2012
 *      Author: Giovanny
 */

#include "ACiTOptions.h"

ACiTOptions::ACiTOptions(const char* s) :
	Options(s) {
}

void ACiTOptions::reader(Lectura lector) {
	_reader = lector;
}

Lectura ACiTOptions::reader(void) const {
	return static_cast<Lectura> (_reader);
}

void ACiTOptions::file(string dirfile) {
	_file = dirfile;
}

string ACiTOptions::file(void) const{
	return static_cast<string> (_file);
}

/**** Calculos relacionados con el makespan y los slots de tiempo ****/

void ACiTOptions::slotsIntervalo(int sI) {
	_slotsIntervalo = sI;
}

int ACiTOptions::slotsIntervalo(void) const{
	return static_cast<int> (_slotsIntervalo);
}

void ACiTOptions::intervalosDia(int intervalosD){
	_intervalosDia = intervalosD;
	calcularSlotsDia();
}

int ACiTOptions::intervalosDia() const {
	return static_cast<int>(_intervalosDia);
}

void ACiTOptions::calcularSlotsDia() {
	//_slotsIntervalo es el valor en el que se divide cada intervaloD (hora).
	_slotsDia = (_intervalosDia * _slotsIntervalo) + 1;
}

int ACiTOptions::slotsDia(void) const {
	return static_cast<int> (_slotsDia);
}

void ACiTOptions::semanas(int opcion){
	_semanas = opcion;
}

int ACiTOptions::semanas() const {
	return static_cast<int> (_semanas);
}

void ACiTOptions::intervalosSemana(int intervalos) {
	_intervalosSemana = intervalos;
	calcularMakespan();
}

int ACiTOptions::intervalosSemana(void) const {
	return static_cast<int> (_intervalosSemana);
}

void ACiTOptions::calcularMakespan() {
	//_slotsIntervalo es el valor en el que se divide cada intervalo (hora).
	//(_intervalosSemana/_intervalosDia) define el numero de dias por semana
	_makespan = _slotsDia*int(ceil(_intervalosSemana/_intervalosDia))*_semanas - 1;
}

int ACiTOptions::makespan(void) const {
	return static_cast<int>(_makespan);
}

/**** Calculo relacionados con las citas y especialistas ****/

void ACiTOptions::totalCitas(int tC) {
	_totalCitas = tC;
}

int ACiTOptions::totalCitas(void) const {
	return static_cast<int> (_totalCitas);
}

void ACiTOptions::totalEspecialistas(int tE) {
	_totalEspecialistas = tE;
}

int ACiTOptions::totalEspecialistas(void) const {
	return static_cast<int> (_totalEspecialistas);
}

vector<Especialidad> ACiTOptions::listaEspecialidades() const{
	return _listaEspecialidades;
}

vector<Especialista> ACiTOptions::listaEspecialistas() const {
	return static_cast<vector<Especialista> > (_listaEspecialistas);
}

vector<Paciente> ACiTOptions::listaPacientes() const {
	return static_cast<vector<Paciente> > (_listaPacientes);
}

void ACiTOptions::listaCodEspecialistas(IntSet ids) {
	_listaCodEspecialistas = ids;
}

IntSet ACiTOptions::listaCodEspecialistas(void) const {
	return static_cast<IntSet> (_listaCodEspecialistas);
}

void ACiTOptions::iniciar(void) {

	//Agregamos los pacientes del archivo al vector listaPacientes
	vector<vector<int> > infoEs; //guarda los datos de todas las especialidades asociadas al paciente i
	vector<int> aux(3); //guarda el id, el numero de citas y duracion  de cada especialidad asociada al paciente i

	for (int i = 0; i < _reader.numPacientes(); i++) {
//		cout << "p[" << i << "]: " << _reader.numTratamientosPac(i) << "-> ";
		for(int e=0; e<_reader.numTratamientosPac(i); e++){
			aux[0] = _reader.obtenerEspecialidadPac(i, e);
			aux[1] = _reader.numCitas(i, e);
			aux[2] = _reader.duracionCitTrat(i, e)/5; //se guarda la duracion en numero de slots
//			cout << aux[0] << "," << aux[1] << " | ";
			infoEs.push_back(aux);
			//aux.clear();
		}
//		cout << endl;

		_listaPacientes.push_back(Paciente(_reader.idPaciente(i),
				_reader.nomPaciente(i), _reader.numTratamientosPac(i),
				infoEs,	_reader.dispPaciente(i)));

		infoEs.clear();
	}
	//Agregamos los especialistas del archivo al vector listaEspecialistas
	for (int j = 0; j < _reader.numEspecialistas(); j++) {
		_listaEspecialistas.push_back(Especialista(_reader.idEspecialista(j),
				_reader.nomEspecialista(j), _reader.numEspProfesional(j),
				_reader.especialidadesProf(j), _reader.dispEspecialista(j)));
	}

//	cout << "especialistas OK" << endl;

	//Agregamos los datos de las especialidades al vector listaEspecialidades
	vector<Especialista> auxE;
	vector<int> auxIdsE;
	vector<Paciente> auxP;
	vector<int> auxIdsP;

//	cout << "numEspdes: " << _reader.numEspecialidades() << endl;

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

		if (_reader.numPacEsp(_reader.idEspecialidad(x)) == (int) auxP.size()) {
			_listaEspecialidades.push_back(Especialidad(
						_reader.idEspecialidad(x), _reader.nomEspecialidad(x), _reader.capEspecialidad(x),
						_reader.numCitasEsp(_reader.idEspecialidad(x)),	_reader.duracionCita(x), auxE.size(),
						_reader.numPacEsp(_reader.idEspecialidad(x)), auxE, auxP, auxIdsE, auxIdsP));
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

int* ACiTOptions::settingCodigos(void) {
	int tam = _reader.numEspecialistas();
	int* codigos;
	codigos = (int*) malloc(sizeof(int) * tam);
	if (_listaEspecialistas.size() > 0) {
		for (int i = 0; i < (int) _listaEspecialistas.size(); i++) {
			codigos[i] = _listaEspecialistas[i].id();
		}
	} else {
		cout << "ERROR: Lista de Especialistas no inicializada" << endl;
		exit(1);
	}
	return codigos;
}

void ACiTOptions::help(void) {
	Options::help();
	cerr <<"\t-file (string) : default:" << file() << endl
			<<"\t\tDirección y nombre del archivo de entrada" << endl;
	cerr <<"\t-slInt (int) : default:" << slotsIntervalo() << endl
			<<"\t\tNumero de slots que componen un intervalo de tiempo" << endl;
	cerr <<"\t-intervalosDia (int) : default:" << intervalosDia() << endl
			<<"\t\tNumero de intervalos de tiempo que componen un dia" << endl;
	cerr <<"\t-semanas (int) : default:" << semanas() << endl
			<<"\t\tSemanas que se tienen disponibles para lograr la asignación del total de citas" << endl;
	cerr <<"\t-intervalosSem (int) : default:" << intervalosSemana() << endl
			<<"\t\tNumero total de intervalo de tiempo que componen una semana" << endl;
}

void ACiTOptions::parse(int & argc, char* argv[]) {
	Options::parse(argc, argv);
	int i=0;
	while (++i < argc){
		if (strcmp(argv[i],"-file")==0){
			file(argv[++i]);
		} else if(strcmp(argv[i],"-slInt")==0){
			slotsIntervalo(atoi(argv[++i]));
		}else if(strcmp(argv[i],"-intervalosDia")==0){
			intervalosDia(atoi(argv[++i]));
		}else if(strcmp(argv[i],"-semanas")==0){
			semanas(atoi(argv[++i]));
		}else if(strcmp(argv[i],"-intervalosSem")==0){
			intervalosSemana(atoi(argv[++i]));
		}
	}
}
