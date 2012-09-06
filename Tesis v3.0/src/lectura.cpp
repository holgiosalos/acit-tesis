#include "lectura.h"


Lectura::Lectura(){} // O.K

Lectura::Lectura(string n){ // O.K
	nombre = n;
	saveLines();
}

Lectura::~Lectura() {}

void Lectura::nombreArchivo(string n){ // O.K
	nombre = n;
}

string Lectura::nombreArchivo(){ // O.K
	return nombre;
}

void Lectura::saveLines(){ // O.K
//	vector <string> lines;
	
	FILE *archivo;
	int tam = 157;
    char palabra[tam]; //Cuando no alcance a leer un fragmento de la línea, cambiar este tamaño.
    char *cadena;
    
    archivo = fopen(nombre.c_str(), "r");
    
    if (archivo == NULL){
        printf("Error al abrir el Lectura \n");
        exit(1);
    }

    do
    {
        cadena = fgets(palabra, tam, archivo);   /* Obtiene una linea del Lectura */
        if (cadena != NULL){
			lines.push_back(cadena);
		}
    }
    while (cadena != NULL);                  /* Hasta encontrar NULL */

    fclose(archivo);
	//return lines;
    //cout<<"Size Lines "<<lines.size()<<endl;
}

vector <string> Lectura::getLines(){ // O.K
	return lines;
}

int Lectura::numEspecialistas(){ // O.K
	int n;
	n = atoi(lines[0].c_str());
	//~ cout <<"numEspecialistas: "<<n<<endl;
	return n;
}

int Lectura::numEspecialidades(){ // O.K
	int n;
	n = atoi(lines[1].c_str());
	//~ cout << "numEspecialidades: " << n << endl;
	return n;
}

int Lectura::numPacientes(){ // O.K
	int n;
	n = atoi(lines[2].c_str());
	//~ cout << "numPacientes: " << n << endl;
	return n;
}


int Lectura::totCitas(){ //Modificación Versión 2.1

	int _numCitas=0;
	int _numPac = numPacientes();

	for(int i=0; i<_numPac; i++){
		_numCitas += totCitasPaciente(i);
	}

	//~ cout << "totalCitas: " << _numCitas << endl;
	return _numCitas;
}

int Lectura::totCitasPaciente(int i){ //Versión 2.1
	int nCitasP = 0;
	int nTratP = 0;

	nTratP = numTratamientosPac(i);

	for(int c=0; c<nTratP;c++){
		nCitasP += numCitas(i, c);
	}

	return nCitasP;
}

//-------------- METODOS RELACIONADOS CON LAS ESPECIALIDADES -----------------//

//Se cuenta apartir de Especialidad 0
string Lectura::infoEspecialidad(int x){ // O.K
	
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

string Lectura::strIdEspecialidad(int x){ // O.K

     string strId = "";
     int esp = idEspecialidad(x);

	 strId = itostr(esp);        	      
    //cout<<strId<<endl;    
    return strId;
}

int Lectura::idEspecialidad(int x){ // O.K

    int id = 0;
    string esp = infoEspecialidad(x);
    
    id = getField(esp, 1);
    //cout<<id<<endl;
    
    return id;    
}

vector <int> Lectura::listaIdEspecialidades(){ // O.K

	vector <int> lista;
	for(int i=0; i<numEspecialidades(); i++){
		lista.push_back(idEspecialidad(i));
	}
	return lista;
}

string Lectura::nombreEspecialidadID(int id){ //Versión 2.1 OK

	vector <int> listaIDs(listaIdEspecialidades());
	vector <string> listaNombres(listaNomEspecialidades());
	int pos = get_position(listaIDs, id);
	string nombre = listaNombres[pos];

	return nombre;
}

string Lectura::nomEspecialidad(int j){ // O.K //Modificación Versión 2.1

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

    nombre = cadena.substr(pos_i+1,pos_f); //Desde pos_i+1, hasta (pos_i+1+pos_f)

    return nombre;
}


vector <string> Lectura::listaNomEspecialidades(){ // O.K

	vector <string> lista;
	for(int i=0; i<numEspecialidades(); i++){
		lista.push_back(nomEspecialidad(i));
	}
	return lista;
}


int Lectura::capEspecialidad(int j){ //Versión 2.1

	string cadena;
	int intCap = 0;

	cadena = subcadenaEspecialidad(j);
	intCap = getField(cadena, 1);

    return intCap;
}

vector<int> Lectura::listaCapEspecialidad(){ //Versión 2.1
	vector <int> lista;
	for(int i=0; i<numEspecialidades(); i++){
		lista.push_back(capEspecialidad(i));
	}
	return lista;
}

int Lectura::duracionCita(int j){ //Versión 2.1

	string cadena;
	int duracion = 0;

	cadena = subcadenaEspecialidad(j);
	duracion = getField(cadena, 2);

	return duracion;
}

vector<int> Lectura::listaDuracionesCitas(){ //Versión 2.1
	vector <int> lista;
	for(int i=0; i<numEspecialidades(); i++){
		lista.push_back(duracionCita(i));
	}
	return lista;
}

int Lectura::numCitasEsp(int i){ //Modificación Versión 2.1 donde i es el id de la especialidad

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


int Lectura::numPacEsp(int i){ //Modificación Versión 2.1 donde i es el id de la Especialidad

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
         _infoEspecialista = lines[3 + j + numEspecialidades()].c_str();
	     //cout<<_infoEspecialista;		
	}

    return _infoEspecialista;
}

int Lectura::idEspecialista(int j){ // O.K

    int id = 0;
    string esp = infoEspecialista(j); 
    
    id = getField(esp, 1);
    //cout<<id<<endl;
    
    return id;    
}	

vector <int> Lectura::listaIdEspecialistas(){ // O.K
	vector <int> lista;
	for(int i=0; i<numEspecialistas(); i++){
		lista.push_back(idEspecialista(i));
	}
	return lista;
}


string Lectura::nomEspecialista(int j){ // Versión Lectura

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

    nombre = cadena.substr(pos_i+1,pos_f); //Desde pos_i+1, hasta (pos_i+1+pos_f)

    return nombre;
}


vector <string> Lectura::listaNomEspecialistas(){ // Versión Lectura

	vector <string> lista;
	for(int i=0; i<numEspecialistas(); i++){
		lista.push_back(nomEspecialista(i));
	}
	return lista;
}


string Lectura::subcadenaProfesional(int j){ //Versión Lectura

	string esp;
	esp = infoEspecialista(j);
    //cout<<esp<<endl;

    int pos = 0;
    pos = itostr(idEspecialista(j)).length()+nomEspecialista(j).length()+4;

	string cadena;
	cadena = esp.substr(pos);

	return cadena;
}


//CREAR MÉTODO PARA CREAR LA LISTA DE NÚMERO DE ESPECIALIDADES DE PROFESIONAL ------------------
int Lectura::numEspProfesional(int j){ //Modificación Versión Lectura

	int intProf = 0;
    string esp = subcadenaProfesional(j);
    intProf = getField(esp, 1);

    return intProf;
}

string Lectura::datosDispProfEspecialidad(int j, int e){ //Versión 2.1 ~
	string esp = "";
	esp = datosDispProfesional(j)[e];
	return esp;
}


vector <string> Lectura::datosDispProfesional(int j){ //Modificación Versión Lectura

	string esp = "";
	esp = subcadenaProfesional(j); //infoEspecialista(j);
	esp = esp.substr(2);
	vector <string> cadenas (subcadenasCorchetes(esp));

	return cadenas;
}


int Lectura::obtenerEspecialidadProf(int j, int e){ //Versión 2.1
	
	int intProf = 0;
	vector <string> esp (datosDispProfesional(j));
	intProf = getField(esp[e],1);

	return intProf;
}

//Profesiones Especialista j
vector <int> Lectura::especialidadesProf(int j){ //Modificación Versión 2.1

	vector <int> intProf;
	int numEP = numEspProfesional(j);

	for(int i=0; i<numEP;i++){
		intProf.push_back(obtenerEspecialidadProf(j,i));
	}

	return intProf;
}

vector <vector<int> > Lectura::listaEspecialidadesProfs(){ //Modificación Versión2.1
	vector <vector<int> > lista;
	for(int i=0; i<numEspecialistas(); i++){
		lista.push_back(especialidadesProf(i));
	}
	return lista;
}

int Lectura::obtenerCapacidad(int j, int e){ //Versión 2.1

	int intCap;
	vector <string> esp (datosDispProfesional(j));
	intCap = capEspecialidad(obtenerEspecialidadProf(j, e));

	return intCap;
}

vector <int> Lectura::capEspecialista(int j){ //Modificación Versión2.1

	vector <int> intCap;
   	int numEP = numEspProfesional(j);

    for(int i=0; i<numEP;i++){
    	intCap.push_back(obtenerCapacidad(j, i));
    }

    return intCap;
}

vector <vector<int> >Lectura::listaCapEspecialistas(){ //Modificación Versión 2.1
	vector <vector <int> > lista;
	for(int i=0; i<numEspecialistas(); i++){
		lista.push_back(capEspecialista(i));
	}
	return lista;
}


vector <string> Lectura::strDispEspecialista(int j){ //Modificación Versión2.1

    vector <string> disponibilidad = datosDispProfesional(j);
    size_t pos;

    int numEP = numEspProfesional(j);

    for(int i=0; i<numEP; i++){
    	pos = disponibilidad[i].find("-");
    	disponibilidad[i] = disponibilidad[i].substr(pos+2);
    }

    return disponibilidad;
}

vector<int> Lectura::dispProfesionalEsp(int j, int e){ //Versión 2.1 ~
	vector <int> dispo;
	dispo = dispEspecialista(j)[e];
	return dispo;
}

vector <vector<int> > Lectura::dispEspecialista(int j){ //Modificación Versión2.1

	vector <vector<int> > vectorAux;
	vector <string> strDisponibilidad = strDispEspecialista(j);

	int numEP = numEspProfesional(j);

    for(int i=0; i<numEP; i++){
    	vectorAux.push_back(splitStrDisponibilidad(strDisponibilidad[i]));
    }

	return vectorAux;
}


//---------------- METODOS RELACIONADOS CON LOS PACIENTES -------------------//

//Se cuenta apartir de Especialistas 0
string Lectura::infoPaciente(int i){ //OK
	
	string _infoPaciente = "";

	if(i>=numPacientes() || i<0){
		cout<<"No Existe el Paciente Número ["<<i<<"]"<<endl;
		exit(1);
	}
	else{
		 _infoPaciente = lines[3 + i + numEspecialidades() + numEspecialistas()].c_str();
	     //cout<<_infoPaciente<<endl;		
	}
    
    return _infoPaciente;		
}


int Lectura::idPaciente(int i){ //OK

    int id = 0;
    string pac = infoPaciente(i);
    
    id = getField(pac, 1);
    //cout<<id<<endl;
    
    return id;    
}

vector <int> Lectura::listaIdPacientes(){ //OK
	vector <int> lista;
	for(int i=0; i<numPacientes(); i++){
		lista.push_back(idPaciente(i));
	}
	return lista;
}


string Lectura::nomPaciente(int j){ // Versión Lectura

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

    nombre = cadena.substr(pos_i+1,pos_f); //Desde pos_i+1, hasta (pos_i+1+pos_f)

    return nombre;
}


vector <string> Lectura::listaNomPacientes(){ // Versión Lectura

	vector <string> lista;
	for(int i=0; i<numPacientes(); i++){
		lista.push_back(nomPaciente(i));
	}
	return lista;
}

//CREAR UN MÉTODO PARA LA LISTA DE TRATAMIENTOS----------------------------------
int Lectura::numTratamientosPac(int i){ //Modificación Versión Lectura

	int nTratamientos = 0;
    string infoPac = subcadenaPaciente(i);

    nTratamientos = getField(infoPac, 1);

    return nTratamientos;
}


string Lectura::subcadenaPaciente(int j){ //Versión Lectura

	string pac;
	pac = infoPaciente(j);
    //cout<<esp<<endl;

    int pos = 0;
    pos = itostr(idPaciente(j)).length()+nomPaciente(j).length()+4;

	string cadena;
	cadena = pac.substr(pos);

	return cadena;
}


vector<string> Lectura::datosTratamientosPaciente(int i){ //Versión 2.1

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
vector <int> Lectura::espPaciente(int i){ //Modificación Versión2.1
	
	vector <int> intEsp;
    int numTrat = numTratamientosPac(i);

    for(int n=0; n<numTrat; n++){
		intEsp.push_back(obtenerEspecialidadPac(i, n));
	}

    return intEsp;
}

vector <vector<int> > Lectura::listaEspPacientes(){ //Modificación Versión2.1
	vector <vector<int> > lista;
	int numP = numPacientes();
	for(int i=0; i<numP; i++){
		lista.push_back(espPaciente(i));
	}

	return lista;
}

// Número de Citas que requiere el Paciente i en la especialidad e
int Lectura::numCitas(int i, int e){  //Modificación Versión 2.1
	
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
vector <vector<int> > Lectura::listaNumCitas(){ //Modificación Versión 2.1
	vector <vector<int> > lista;
	for(int i=0; i<numPacientes(); i++){
		lista.push_back(numeroCitasPac(i));
	}
	return lista;
}

int Lectura::profPrefPac(int i, int e){  //Versión Lectura

	int intCitas;
	vector <string> datos (datosTratamientosPaciente(i));
	intCitas = getField(datos[e],3);

	return intCitas;
}

vector <int> Lectura::listaProfPrefPac(int i){ //Versión Lectura
	vector <int> intCitas;
	int numTrat = numTratamientosPac(i);

	for(int n=0; n<numTrat; n++){
		intCitas.push_back(profPrefPac(i,n));
	}

	return intCitas;
}


string Lectura::strDispPaciente(int i){ //OK
	string disponibilidad = ""; 
    size_t pos;
    
    disponibilidad = infoPaciente(i);
	pos = disponibilidad.find("-");   
	disponibilidad = disponibilidad.substr(pos+2); 
	
    return disponibilidad;
}

vector <int> Lectura::dispPaciente(int i){ //OK
	
	vector <int> vectorAux;
	string strDisponibilidad = strDispPaciente(i);
	vectorAux = splitStrDisponibilidad(strDisponibilidad);
	 
	return vectorAux;
}

vector <vector<int> > Lectura::listaDispPacientes(){ //OK
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
	char *caracteres;
	char *aux;
  
	caracteres = new char[cadena.size()];
	caracteres = (char*)cadena.data();
	
	aux = strtok(caracteres," ");
	
	while(aux != NULL){
		vectorAux.push_back(atoi(aux));
		aux = strtok(NULL," ");
    }
		
    return vectorAux;
}

string Lectura::subcadenaEspecialidad(int j){ //Versión 2.1

	string esp;
    esp = infoEspecialidad(j);
    //cout<<esp<<endl;

    int pos = 0;
    pos = strIdEspecialidad(j).length()+nomEspecialidad(j).length()+4;

	string cadena;
	cadena = esp.substr(pos);

	return cadena;
}


vector <string> Lectura::subcadenasCorchetes(string cadena){ //Versión2.1 OK

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

int Lectura::get_position(vector <int> vector, int num){ //Versión2.1 OK

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
