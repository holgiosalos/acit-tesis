#include "lectura.h"

Lectura::Lectura(){}

Lectura::~Lectura(){}

Lectura::Lectura(string n){
	nombre = n;
	contador = 0;

	saveLines();
	init_suma_esp();
	set_nro_total_EP();
}

void Lectura::nombreArchivo(string n){
	nombre = n;
}

string Lectura::nombreArchivo(){
	return nombre;
}


void Lectura::saveLines(){

	ifstream archivo;
	string linea;

	archivo.open(nombre.c_str());

	if(archivo.fail()){
		printf("Error al abrir el Archivo \n");
		exit(1);
	}

	while(getline(archivo, linea)){
		lines.push_back(linea);
	}

	archivo.close();
    //cout<<"Size Lines "<<lines.size()<<endl;
}

vector <string> Lectura::getLines(){
	return lines;
}

int Lectura::numEspecialistas(){
	int n;
	n = atoi(lines[0].c_str());
	//~ cout <<"numEspecialistas: "<<n<<endl;
	return n;
}

int Lectura::numEspecialidades(){
	int n;
	n = atoi(lines[1].c_str());
	//~ cout << "numEspecialidades: " << n << endl;
	return n;
}

int Lectura::numPacientes(){
	int n;
	n = atoi(lines[2].c_str());
	//~ cout << "numPacientes: " << n << endl;
	return n;
}

int Lectura::totCitas(){

	int _numCitas=0;
	int _numPac = numPacientes();

	for(int i=0; i<_numPac; i++){
		_numCitas += totCitasPaciente(i);
	}
	//~ cout << "totalCitas: " << _numCitas << endl;
	return _numCitas;
}

int Lectura::totCitasPaciente(int i){
	int nCitasP = 0;
	int nTratP = 0;

	nTratP = numTratamientosPac(i);

	for(int c=0; c<nTratP;c++){
		nCitasP += numCitas(i, c);
	}

	return nCitasP;
}

int Lectura::numIntervalos(){
	vector<int> auxDisp = dispPaciente(0);
	return (int)auxDisp.size();
}

int Lectura::numIntervalosDia(void){
	int nInt = numIntervalos();
	return (int)nInt/5;
}

//-------------- METODOS RELACIONADOS CON LAS ESPECIALIDADES -----------------//

//Se cuenta apartir de Especialidad 0
string Lectura::infoEspecialidad(int x){
	
	string _infoEspecialidad = "";

	if(x>=numEspecialidades() || x<0){
		cout<<"Número de Especialidad Incorrecto"<<endl;
		exit(1);
	}
	else{
         _infoEspecialidad = lines[3+x].c_str();
	     //cout<<_infoEspecialidad;		
	}

    return _infoEspecialidad;		
}

string Lectura::strIdEspecialidad(int x){

     string strId = "";
     int esp = idEspecialidad(x);

	 strId = itostr(esp);        	      
    //cout<<strId<<endl;    
    return strId;
}

int Lectura::idEspecialidad(int x){

    int id = 0;
    string esp = infoEspecialidad(x);
    
    id = getField(esp, 1);
    //cout<<id<<endl;
    
    return id;    
}

vector <int> Lectura::listaIdEspecialidades(){

	vector <int> lista;
	for(int i=0; i<numEspecialidades(); i++){
		lista.push_back(idEspecialidad(i));
	}
	return lista;
}

string Lectura::nombreEspecialidadID(int id){

	vector <int> listaIDs(listaIdEspecialidades());
	vector <string> listaNombres(listaNomEspecialidades());

	int pos = get_position(listaIDs, id);
	string nombre = listaNombres.at(pos);

	return nombre;
}

string Lectura::nomEspecialidad(int j){

	string nombre = "";
    string cadena = infoEspecialidad(j);

    string caracter("'");
    size_t nomIni, nomFin;

    int pos_i = 0;
    int pos_f = 0;

    nomIni = cadena.find(caracter);

    if (nomIni!=string::npos){
    	pos_i = int(nomIni);
    }

    string subcadena = cadena.substr(nomIni+1);
    nomFin = subcadena.find(caracter);

    if (nomFin!=string::npos){
    	 pos_f = int(nomFin);
    }

    nombre = cadena.substr(pos_i+1,pos_f);

    return nombre;
}


vector <string> Lectura::listaNomEspecialidades(){

	vector <string> lista;
	for(int i=0; i<numEspecialidades(); i++){
		lista.push_back(nomEspecialidad(i));
	}
	return lista;
}


int Lectura::capEspecialidad(int j){

	string cadena;
	int intCap = 0;

	cadena = subcadenaEspecialidad(j);
	intCap = getField(cadena, 1);

    return intCap;
}

vector<int> Lectura::listaCapEspecialidad(){
	vector <int> lista;
	for(int i=0; i<numEspecialidades(); i++){
		lista.push_back(capEspecialidad(i));
	}
	return lista;
}

int Lectura::duracionCita(int j){

	string cadena;
	int duracion = 0;

	cadena = subcadenaEspecialidad(j);
	duracion = getField(cadena, 2);

	return duracion;
}

vector<int> Lectura::listaDuracionesCitas(){
	vector <int> lista;
	for(int i=0; i<numEspecialidades(); i++){
		lista.push_back(duracionCita(i));
	}
	return lista;
}


int Lectura::numCitasEsp(int i){ //Donde i es el id de la especialidad

	int contador = 0;
	int nPac = numPacientes();
	int nTrat = 0;

	for(int j=0; j<nPac; j++){
		nTrat = numTratamientosPac(j);
		for(int k=0; k<nTrat;k++){
			if(obtenerEspecialidadPac(j,k) == i){
				contador += numCitas(j, k);
			}
		}
	}
	//~ cout << "numCitEsp: " << contador << endl;
	return contador;
}


int Lectura::numPacEsp(int i){ //Donde i es el id de la Especialidad

	int contador=0;
	int nEsp = 0;
	vector <vector<int> > especialidadesP (listaEspPacientes());

	for(int j=0; j<(int)especialidadesP.size(); j++){
		nEsp = numTratamientosPac(j);
		for(int k=0; k<nEsp; k++){
			if(obtenerEspecialidadPac(j,k) == i){
				contador++;
			}
		}
	}
	return contador;
}


//-------------- METODOS RELACIONADOS CON LOS ESPECIALISTAS -----------------//

//Se cuenta apartir de Especialistas 0
string Lectura::infoEspecialista(int j){ // O.K
	
	string _infoEspecialista = "";
	
	if(j>=numEspecialistas() || j<0){
		cout<<"No Existe el Especialista Número ["<<j<<"]"<<endl;
		exit(1);
	}
	else{

		count_esp(j);
		_infoEspecialista = lines[3 + j + numEspecialidades() + suma_esp_ant.at(j)].c_str();
	     //cout<<_infoEspecialista;		
	}

    return _infoEspecialista;
}

int Lectura::idEspecialista(int j){

    int id = 0;
    string esp = infoEspecialista(j); 
    
    id = getField(esp, 1);
    //cout<<id<<endl;
    
    return id;    
}	

vector <int> Lectura::listaIdEspecialistas(){
	vector <int> lista;
	for(int i=0; i<numEspecialistas(); i++){
		lista.push_back(idEspecialista(i));
	}
	return lista;
}


string Lectura::nomEspecialista(int j){

	string nombre = "";
    string cadena = infoEspecialista(j);

    string caracter("'");
    size_t nomIni, nomFin;

    int pos_i = 0;
    int pos_f = 0;

    nomIni = cadena.find(caracter);

    if (nomIni!=string::npos){
    	pos_i = int(nomIni);
    }

    string subcadena = cadena.substr(nomIni+1);
    nomFin = subcadena.find(caracter);

    if (nomFin!=string::npos){
    	 pos_f = int(nomFin);
    }

    nombre = cadena.substr(pos_i+1,pos_f);

    return nombre;
}


vector <string> Lectura::listaNomEspecialistas(){

	vector <string> lista;
	for(int i=0; i<numEspecialistas(); i++){
		lista.push_back(nomEspecialista(i));
	}
	return lista;
}


int Lectura::numEspProfesional(int j){

	int intProf = 0;

	string esp;
	esp = infoEspecialista(j);
    int pos = 0;
    pos = itostr(idEspecialista(j)).length()+nomEspecialista(j).length()+4;

	string cadena;
	cadena = esp.substr(pos);

    intProf = getField(cadena, 1);

    return intProf;
}

string Lectura::datosDispProfEspecialidad(int j, int e){
	string esp = "";
	esp = datosDispProfesional(j)[e];
	return esp;
}


vector <string> Lectura::datosDispProfesional(int j){

	string esp = "";
	vector <string> cadenas;

	for(int i = 0; i < numEspProfesional(j);i++){
		esp = lines[3 + j + i + 1 + numEspecialidades() + suma_esp_ant.at(j)].c_str();
		cadenas.push_back(esp);
	}

	return cadenas;
}


int Lectura::obtenerEspecialidadProf(int j, int e){
	
	int intProf = 0;
	vector <string> esp (datosDispProfesional(j));
	intProf = getField(esp[e],1);

	return intProf;
}


//Profesiones Especialista j
vector <int> Lectura::especialidadesProf(int j){

	vector <int> intProf;
	int numEP = numEspProfesional(j);

	for(int i=0; i<numEP;i++){
		intProf.push_back(obtenerEspecialidadProf(j,i));
	}

	return intProf;
}

vector <vector<int> > Lectura::listaEspecialidadesProfs(){
	vector <vector<int> > lista;
	for(int i=0; i<numEspecialistas(); i++){
		lista.push_back(especialidadesProf(i));
	}
	return lista;
}

int Lectura::obtenerCapacidad(int j, int e){

	int intCap;
	vector <string> esp (datosDispProfesional(j));
	intCap = capEspecialidad(obtenerEspecialidadProf(j, e));

	return intCap;
}

vector <int> Lectura::capEspecialista(int j){

	vector <int> intCap;
   	int numEP = numEspProfesional(j);

    for(int i=0; i<numEP;i++){
    	intCap.push_back(obtenerCapacidad(j, i));
    }

    return intCap;
}

vector <vector<int> >Lectura::listaCapEspecialistas(){
	vector <vector <int> > lista;
	for(int i=0; i<numEspecialistas(); i++){
		lista.push_back(capEspecialista(i));
	}
	return lista;
}


vector <string> Lectura::strDispEspecialista(int j){

    vector <string> disponibilidad = datosDispProfesional(j);
    size_t pos;

    int numEP = numEspProfesional(j);

    for(int i=0; i<numEP; i++){
    	pos = disponibilidad[i].find("-");
    	disponibilidad[i] = disponibilidad[i].substr(pos+2);
    }

    return disponibilidad;
}

vector<int> Lectura::dispProfesionalEsp(int j, int e){
	vector <int> dispo;
	dispo = dispEspecialista(j)[e];
	return dispo;
}

vector <vector<int> > Lectura::dispEspecialista(int j){

	vector <vector<int> > vectorAux;
	vector <string> strDisponibilidad = strDispEspecialista(j);

	int numEP = numEspProfesional(j);

    for(int i=0; i<numEP; i++){
    	vectorAux.push_back(splitStrDisponibilidad(strDisponibilidad[i]));
    }

	return vectorAux;
}


//---------------- METODOS RELACIONADOS CON LOS PACIENTES -------------------//

//Se cuenta apartir de Paciente 0
string Lectura::infoPaciente(int i){
	
	string _infoPaciente = "";

	if(i>=numPacientes() || i<0){
		cout<<"No Existe el Paciente Número ["<<i<<"]"<<endl;
		exit(1);
	}
	else{
		 _infoPaciente = lines[3 + numEspecialidades() + numEspecialistas()+ 2*i + get_nro_total_EP()].c_str();
	     //cout<<_infoPaciente<<endl;		
	}
    
    return _infoPaciente;		
}


int Lectura::idPaciente(int i){

    int id = 0;
    string pac = infoPaciente(i);
    
    id = getField(pac, 1);
    //cout<<id<<endl;
    
    return id;    
}

vector <int> Lectura::listaIdPacientes(){
	vector <int> lista;
	for(int i=0; i<numPacientes(); i++){
		lista.push_back(idPaciente(i));
	}
	return lista;
}


string Lectura::nomPaciente(int j){

	string nombre = "";
    string cadena = infoPaciente(j);

    string caracter("'");
    size_t nomIni, nomFin;
    int pos_i = 0;
    int pos_f = 0;

    nomIni = cadena.find(caracter);

    if (nomIni!=string::npos){
    	pos_i = int(nomIni);
    }

    string subcadena = cadena.substr(nomIni+1);
    nomFin = subcadena.find(caracter);

    if (nomFin!=string::npos){
    	 pos_f = int(nomFin);
    }

    nombre = cadena.substr(pos_i+1,pos_f);

    return nombre;
}


vector <string> Lectura::listaNomPacientes(){

	vector <string> lista;
	for(int i=0; i<numPacientes(); i++){
		lista.push_back(nomPaciente(i));
	}
	return lista;
}


int Lectura::numTratamientosPac(int i){

	int nTratamientos = 0;
    string infoPac = subcadenaPaciente(i);

    nTratamientos = getField(infoPac, 1);

    return nTratamientos;
}


string Lectura::subcadenaPaciente(int j){

	string pac;
	pac = infoPaciente(j);

    int pos = 0;
    pos = itostr(idPaciente(j)).length()+nomPaciente(j).length()+4;

	string cadena;
	cadena = pac.substr(pos);

	return cadena;
}


vector<string> Lectura::datosTratamientosPaciente(int i){

	string pac = "";
	pac = infoPaciente(i);
	int pos = pac.find("-");
	int posIni = itostr(idPaciente(i)).length()+nomPaciente(i).length()+itostr(numTratamientosPac(i)).length()+5;
	pac = pac.substr(posIni, pos-posIni);

	vector <string> cadenas (subcadenasCorchetes(pac));

	return cadenas;
}

int Lectura::obtenerEspecialidadPac(int i, int e){

	int intEsp;
	vector <string> datos (datosTratamientosPaciente(i));
	intEsp = getField(datos[e],1);

	return intEsp;
}

// Especialidad que requiere el Paciente i
vector <int> Lectura::espPaciente(int i){
	
	vector <int> intEsp;
    int numTrat = numTratamientosPac(i);

    for(int n=0; n<numTrat; n++){
		intEsp.push_back(obtenerEspecialidadPac(i, n));
	}

    return intEsp;
}

vector <vector<int> > Lectura::listaEspPacientes(){
	vector <vector<int> > lista;
	int numP = numPacientes();
	for(int i=0; i<numP; i++){
		lista.push_back(espPaciente(i));
	}

	return lista;
}

// Número de Citas que requiere el Paciente i en la especialidad e
int Lectura::numCitas(int i, int e){
	
	int intCitas;
	vector <string> datos (datosTratamientosPaciente(i));
	intCitas = getField(datos[e],2);

	return intCitas;
}

vector <int> Lectura::numeroCitasPac(int i){
	vector <int> intCitas;
	int numTrat = numTratamientosPac(i);

	for(int n=0; n<numTrat; n++){
		intCitas.push_back(numCitas(i,n));
	}

	return intCitas;
}

// Lista con el numero de citas
vector <vector<int> > Lectura::listaNumCitas(){
	vector <vector<int> > lista;
	for(int i=0; i<numPacientes(); i++){
		lista.push_back(numeroCitasPac(i));
	}
	return lista;
}

int Lectura::profPrefPac(int i, int e){

	int intCitas;
	vector <string> datos (datosTratamientosPaciente(i));
	intCitas = getField(datos[e],3);

	return intCitas;
}

vector <int> Lectura::listaProfPrefPac(int i){
	vector <int> intCitas;
	int numTrat = numTratamientosPac(i);

	for(int n=0; n<numTrat; n++){
		intCitas.push_back(profPrefPac(i,n));
	}

	return intCitas;
}


string Lectura::strDispPaciente(int i){
	string disponibilidad = ""; 
    size_t pos;

    disponibilidad = lines[3 + numEspecialidades() + numEspecialistas() + (2*i + 1) + + get_nro_total_EP()].c_str();
	pos = disponibilidad.find("-");   
	disponibilidad = disponibilidad.substr(pos+1);
	
    return disponibilidad;
}

vector <int> Lectura::dispPaciente(int i){
	
	vector <int> vectorAux;
	string strDisponibilidad = strDispPaciente(i);
	vectorAux = splitStrDisponibilidad(strDisponibilidad);
	 
	return vectorAux;
}

vector <vector<int> > Lectura::listaDispPacientes(){
	vector <vector<int> > lista;
	for(int i=0; i<numPacientes(); i++){
		lista.push_back(dispPaciente(i));
	}
	return lista;
}


int Lectura::duracionCitTrat(int i, int e){
	int idE = obtenerEspecialidadPac(i, e);
	int dur=0;
	for(int n=0; n<numEspecialidades(); n++){
		if(idEspecialidad(n)==idE){
			dur = duracionCita(n);
			break;
		}else if(n == numEspecialidades()-1){
			cout << "Error duracion no encontrada" << endl;
			exit(0);
		}
	}
	return dur;
}


//--------------- MÉTODOS AUXILIARES -----------------//


string Lectura::itostr(int num){
	stringstream ss;
	ss << num;
	return ss.str();
}


int Lectura::getField(string cadena, int field){
	
	int value = 0;
	char *caracteres;
	char *aux;
  
	caracteres = new char[cadena.size()];
	caracteres = (char*)cadena.data();

	switch(field){
		case 1:
			aux = strtok(caracteres," ");
		break;
		case 2:
			aux = strtok(caracteres," ");
			aux = strtok(NULL," ");
		break;
		case 3:
			aux = strtok(caracteres," ");
			aux = strtok(NULL," ");
			aux = strtok(NULL," ");
		break;
		default: cout<<"Campo "<<field<<" no Existe"<<endl;
		break;
	}

	value = atoi(aux);
	return value;
}


vector <int> Lectura::splitStrDisponibilidad(string cadena){
	
	vector <int> vectorAux;

	string buf;
	stringstream ss(cadena);

	vector<string> tokens;

	while(getline(ss, buf, ' ')) {
		vectorAux.push_back(atoi(buf.c_str()));
	}

	return vectorAux;
}

string Lectura::subcadenaEspecialidad(int j){

	string esp;
    esp = infoEspecialidad(j);
    //cout<<esp<<endl;

    int pos = 0;
    pos = strIdEspecialidad(j).length()+nomEspecialidad(j).length()+4;

	string cadena;
	cadena = esp.substr(pos);

	return cadena;
}


vector <string> Lectura::subcadenasCorchetes(string cadena){

	vector <string> vectorAux;
	char *caracteres;
	char *aux;

	caracteres = new char[cadena.size()];
	caracteres = (char*)cadena.data();

	aux = strtok(caracteres,"[]");

	while(aux != NULL){
		vectorAux.push_back(aux);
		aux = strtok(NULL,"[]");
    }

    return vectorAux;
}

int Lectura::get_position(vector <int> vector, int num){

	int pos;

	for (int i=0;i<(int)vector.size();i++){

		if (vector[i]==num){
			//cout<<"La posición del número buscado es "<<i;
			pos = i;
			break;
		}
		else{
			pos = -1;
		}
	}

	if(pos==-1){
		cout<<"El id "<<num<<" no se encuentra "<<endl;
	}

	return pos;
}

void Lectura::set_nro_total_EP(){

	nro_total_EP = 0;

	for(int i=0; i < numEspecialistas(); i++){
		nro_total_EP = nro_total_EP + numEspProfesional(i);
	}
}

int Lectura::get_nro_total_EP(){
	return nro_total_EP;
}


void Lectura::count_esp(int j){

	int aux = 0;

	if(suma_esp_ant.at(j) == -1){

		if(j == 0){
			aux = j;
		}
		else{
			aux = numEspProfesional(j-1);
			contador += aux;
		}
		suma_esp_ant.at(j) = contador;
	}
}

void Lectura::init_suma_esp(){
	for(int i=0; i<numEspecialistas(); i++)
		suma_esp_ant.push_back(-1);
}
