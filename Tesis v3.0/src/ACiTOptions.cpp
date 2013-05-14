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

ACiTOptions::~ACiTOptions(){}

void ACiTOptions::reader(Lectura lector) {
	_reader = lector;
}

Lectura ACiTOptions::reader(void) const {
	return _reader;
}

void ACiTOptions::file(string dirfile) {
	_file = dirfile;
}

string ACiTOptions::file(void) const{
	return _file;
}

void ACiTOptions::preferencia(bool pref) {
	_preferencia = pref;
}

bool ACiTOptions::preferencia() const {
	return _preferencia;
}

/**** Calculos relacionados con el makespan y los slots de tiempo ****/

void ACiTOptions::slotsIntervalo(int sI) {
	_slotsIntervalo = sI;
}

int ACiTOptions::slotsIntervalo(void) const{
	return _slotsIntervalo;
}

void ACiTOptions::intervalosDia(int intervalosD){
	_intervalosDia = intervalosD;
	calcularSlotsDia();
}

int ACiTOptions::intervalosDia() const {
	return _intervalosDia;
}

void ACiTOptions::calcularSlotsDia() {
	//_slotsIntervalo es el valor en el que se divide cada intervaloD (hora).
	_slotsDia = (_intervalosDia * _slotsIntervalo) + 1;
}

int ACiTOptions::slotsDia(void) const {
	return _slotsDia;
}

void ACiTOptions::semanas(int opcion){
	_semanas = opcion;
}

int ACiTOptions::semanas() const {
	return _semanas;
}

void ACiTOptions::intervalosSemana(int intervalos) {
	_intervalosSemana = intervalos;
	calcularMakespan();
}

int ACiTOptions::intervalosSemana(void) const {
	return _intervalosSemana;
}

void ACiTOptions::calcularMakespan() {
	//_slotsIntervalo es el valor en el que se divide cada intervalo (hora).
	int dias = int(floor(_intervalosSemana/_intervalosDia));
	_makespan = (_slotsDia * dias *_semanas) - 1;
}

int ACiTOptions::makespan(void) const {
	return _makespan;
}

/**** Calculo relacionados con las citas y especialistas ****/

void ACiTOptions::totalCitas(int tC) {
	_totalCitas = tC;
}

int ACiTOptions::totalCitas(void) const {
	return _totalCitas;
}

void ACiTOptions::totalEspecialistas(int tE) {
	_totalEspecialistas = tE;
}

int ACiTOptions::totalEspecialistas(void) const {
	return _totalEspecialistas;
}

void ACiTOptions::nPacientesPreferencia(int n) {
	_nPacientesPreferencia = n;
}

int ACiTOptions::nPacientesPreferencia() const{
	return _nPacientesPreferencia;
}

void ACiTOptions::mayorIdProfesional(int m) {
	_mayorIdProfesional = m;
}

int ACiTOptions::mayorIdProfesional() const{
	return _mayorIdProfesional;
}

vector<Especialidad>* ACiTOptions::listaEspecialidades() const{
	return _listaEspecialidades;
}

vector<Especialista>* ACiTOptions::listaEspecialistas() const {
	return _listaEspecialistas;
}

vector<Paciente> ACiTOptions::listaPacientes() const {
	return _listaPacientes;
}

void ACiTOptions::listaCodEspecialistas(IntSet ids) {
	_listaCodEspecialistas = ids;
}

IntSet ACiTOptions::listaCodEspecialistas(void) const {
	return _listaCodEspecialistas;
}

void ACiTOptions::cutoff(int c){
	_cutoff = c;
}

int ACiTOptions::cutoff(void) const{
	return _cutoff;
}

void ACiTOptions::factor(double fc){
	_factor = fc;
}

double ACiTOptions::factor(void) const{
	return _factor;
}

void ACiTOptions::cutoffLuby(int p){
	_parametrer = p;
}

int ACiTOptions::cutoffLuby(void) const{
	return _parametrer;
}

void ACiTOptions::isLuby(bool value) {
	_isLuby = value;
}

bool ACiTOptions::isLuby(void) const {
	return _isLuby;
}

void ACiTOptions::iniciar(void) {

	//Agregamos los pacientes del archivo al vector listaPacientes
	vector<vector<int> > infoEs; //guarda los datos de todas las especialidades asociadas al paciente i
	//guarda el id, el numero de citas, duracion de cada especialidad y especialista de preferencia asociada al paciente i
	vector<int> aux(4);
	int pacientespref = 0; //cuenta el numero de pacientes que tienen algun especialista de preferencia
	int mayorId = 0; //almacena el id de profesional mas alto.

	for (int i = 0; i < _reader.numPacientes(); i++) {
//		cout << "p[" << i << "]: " << _reader.numTratamientosPac(i) << "-> ";
		for(int e=0; e<_reader.numTratamientosPac(i); e++){
			aux[0] = _reader.obtenerEspecialidadPac(i, e); //Id de especialidad del tratamiento
			aux[1] = _reader.numCitas(i, e); //Numero de citas en ese tratamiento
			aux[2] = _reader.duracionCitTrat(i, e)/5; //se guarda la duracion en numero de slots
			aux[3] = _reader.profPrefPac(i, e); //profesional de preferencia en esa especialidad
//			cout << aux[0] << "," << aux[1] << " | ";
			if (aux[3] > 0) {
				pacientespref++;
			}
			infoEs.push_back(aux);
			//aux.clear();
		}
//		cout << endl;
		_listaPacientes.push_back(Paciente(_reader.idPaciente(i),
				_reader.nomPaciente(i), _reader.numTratamientosPac(i),
				infoEs,	_reader.dispPaciente(i)));

		infoEs.clear();
	}

	nPacientesPreferencia(pacientespref);
	_listaEspecialidades = new vector<Especialidad>();
	_listaEspecialistas = new vector<Especialista>();

	//Agregamos los especialistas del archivo al vector listaEspecialistas
	for (int j = 0; j < _reader.numEspecialistas(); j++) {
		_listaEspecialistas->push_back(Especialista(_reader.idEspecialista(j),
				_reader.nomEspecialista(j), _reader.numEspProfesional(j),
				_reader.especialidadesProf(j), _reader.dispEspecialista(j)));
		if (mayorId < _reader.idEspecialista(j)) {
			mayorId = _reader.idEspecialista(j);
		}
	}

	mayorIdProfesional(mayorId);

//	cout << "especialistas OK" << endl;

	//Agregamos los datos de las especialidades al vector listaEspecialidades
	vector<Especialista>* auxE;
	vector<int> auxIdsE;
	vector<Paciente> auxP;
	vector<int> auxIdsP;

//	cout << "numEspdes: " << _reader.numEspecialidades() << endl;

	for (int x = 0; x < _reader.numEspecialidades(); x++) {
		auxE = new vector<Especialista>();
		for (int j = 0; j < (int) _listaEspecialistas->size(); j++) {
			if (_listaEspecialistas->at(j).buscaEspecialidadProf(_reader.idEspecialidad(x))) {
				auxE->push_back(_listaEspecialistas->at(j));
				auxIdsE.push_back(_listaEspecialistas->at(j).id());
			}
		}

		for (int p = 0; p < (int) _listaPacientes.size(); p++) {
			if (_listaPacientes[p].buscaEspecialidadPac(_reader.idEspecialidad(x))) {
				auxP.push_back(_listaPacientes[p]);
				auxIdsP.push_back(_listaPacientes[p].id());
			}
		}

		if (_reader.numPacEsp(_reader.idEspecialidad(x)) == (int) auxP.size()) {
			_listaEspecialidades->push_back(Especialidad(
						_reader.idEspecialidad(x), _reader.nomEspecialidad(x), _reader.capEspecialidad(x),
						_reader.numCitasEsp(_reader.idEspecialidad(x)),	_reader.duracionCita(x), auxE->size(),
						_reader.numPacEsp(_reader.idEspecialidad(x)), auxE, auxP, auxIdsE, auxIdsP));
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
	if (_listaEspecialistas->size() > 0) {
		for (int i = 0; i < (int) _listaEspecialistas->size(); i++) {
			codigos[i] = _listaEspecialistas->at(i).id();
		}
	} else {
		cout << "ERROR: Lista de Especialistas no inicializada" << endl;
		exit(1);
	}
	return codigos;
}

void ACiTOptions::help(void) {
	string prefStr;

	if (preferencia())
	{
		prefStr = "true";
	}
	else
	{
		prefStr = "false";
	}

	Options::help();
	cerr <<"\t-pref (false, true) : default: " << prefStr << endl
			<<"\t\tWheter the preference constraint is taken into account (true) or not (false)" << endl;
	cerr <<"\t-file (string) : default: " << file() << endl
			<<"\t\tPath and name of input file" << endl;
	cerr <<"\t-slsInt (int) : default: " << slotsIntervalo() << endl
			<<"\t\tSlots number that comprising a time interval" << endl;
	cerr <<"\t-dayInt (int) : default: " << intervalosDia() << endl
			<<"\t\tNumber of time intervals by which a day is composed" << endl;
	cerr <<"\t-weekInt (int) : default: " << intervalosSemana() << endl
			<<"\t\tTotal of time intervals by which a week is composed" << endl;
	cerr <<"\t-weeks (int) : default: " << semanas() << endl
			<<"\t\tAvailable weeks to schedule the medical appointments" << endl;
    cerr <<"\t-cutoff (unsigned int) default: "<< cutoff() << endl
    		<<"\t\tCutoff inicial" << endl;
    cerr <<"\t-factor (unsigned int) default:"<< factor() << endl
    		<<"\t\tFactor de restart" << endl;
    cerr <<"\t-cutoff luby (unsigned int) default: "<< cutoffLuby() << endl
    		<<"\t\tValor de la constante p" << endl;
}

void ACiTOptions::parse(int & argc, char* argv[]) {
	Options::parse(argc, argv);
	int i=0;
	while (++i < argc){
		if (strcmp(argv[i],"-pref") == 0)
		{
			if (strcmp(argv[++i],"true") == 0)
			{
				preferencia(true);
			}
			else
			{
				preferencia(false);
			}
		}
		else if (strcmp(argv[i],"-file") == 0)
		{
			file(argv[++i]);
		}
		else if (strcmp(argv[i],"-slsInt") == 0)
		{
			slotsIntervalo(atoi(argv[++i]));
		}
		else if (strcmp(argv[i],"-dayInt") == 0)
		{
			intervalosDia(atoi(argv[++i]));
		}
		else if (strcmp(argv[i],"-weekInt") == 0)
		{
			intervalosSemana(atoi(argv[++i]));
		}
		else if (strcmp(argv[i],"-weeks") == 0)
		{
			semanas(atoi(argv[++i]));
		}
		else if (strcmp(argv[i],"-cutoff") == 0){
			if (strcmp(argv[++i], "luby") == 0){
				cutoffLuby(atoi(argv[++i]));
				_isLuby = true;
			}
			else {
				_isLuby = false;
				cutoff(atoi(argv[i]));
			}
		}
		else if (strcmp(argv[i],"-factor") == 0) {
			factor(atof(argv[++i]));
		}
	}
}
