#include "archivo.h"


Archivo::Archivo(){} // O.K

Archivo::Archivo(string n){ // O.K
	nombre = n;
	saveLines();
}

void Archivo::nombreArchivo(string n){ // O.K
	nombre = n;
}

string Archivo::nombreArchivo(){ // O.K
	return nombre;
}

void Archivo::getInfoCita(int i){ // O.K
	
	string strCita;
	int hora =  0; 	string strHora;
	int dia = 0;	string strDia;
	//int semana = 0; string strSemana;
	
	hora = i%8;
	dia = (int)floor(i/8);
	//semana = (int)i/40;
	//semana = semana+1;
	
		
	switch(dia){
		case 0: strDia = "Lunes";
		break;
		case 1: strDia = "Martes";
		break;
		case 2: strDia = "Miercoles";
		break;
		case 3: strDia = "Jueves";
		break;
		case 4: strDia = "Viernes";
		break;
		default: strDia = "Invalido";
		break;
	}
	
	switch(hora){
		case 0: strHora = "[08:00 - 09:00)";
		break;
		case 1: strHora = "[09:00 - 10:00)";
		break;
		case 2: strHora = "[10:00 - 11:00)";
		break;
		case 3: strHora = "[11:00 - 12:00)";
		break;
		case 4: strHora = "[14:00 - 15:00)";
		break;
		case 5: strHora = "[15:00 - 16:00)";
		break;
		case 6: strHora = "[16:00 - 17:00)";
		break;
		case 7: strHora = "[17:00 - 18:00)";
		break;
		default: strHora = "Invalido";
		break;
	}
	
	/*strSemana = itostr(semana);*/
	strCita = /*"Semana "+strSemana+" - "+*/"Dia: "+strDia+" - "+"Hora: "+strHora;
	cout<<strCita<<endl;
	//return strCita;
}

void Archivo::saveLines(){ // O.K
//	vector <string> lines;
	
	FILE *archivo;
	int tam = 150;
    char palabra[tam]; //Cuando no alcance a leer un fragmento de la línea, cambiar este tamaño.
    char *cadena;
    
    archivo = fopen(nombre.c_str(), "r");
    
    if (archivo == NULL){
        printf("Error al abrir el archivo \n");
        exit(1);
    }

    do
    {
        cadena = fgets(palabra, tam, archivo);   /* Obtiene una linea del archivo */
        if (cadena != NULL){
			lines.push_back(cadena);
		}
    }
    while (cadena != NULL);                  /* Hasta encontrar NULL */

    fclose(archivo);
	//return lines;
    //cout<<"Size Lines "<<lines.size()<<endl;
}

vector <string> Archivo::getLines(){ // O.K
	return lines;
}

int Archivo::numEspecialistas(){ // O.K
	int n;
	n = atoi(lines[0].c_str());
	//~ cout <<"numEspecialistas: "<<n<<endl;
	return n;
}

int Archivo::numEspecialidades(){ // O.K
	int n;
	n = atoi(lines[1].c_str());
	//~ cout << "numEspecialidades: " << n << endl;
	return n;
}

int Archivo::numPacientes(){ // O.K
	int n;
	n = atoi(lines[2].c_str());
	//~ cout << "numPacientes: " << n << endl;
	return n;
}

int Archivo::numSemanas(){ // O.K //Versión 2.1
	int n;
	n = atoi(lines[3].c_str());
	//~ cout << "numSemanas: " << n << endl;
	return n;
}

int Archivo::totCitas(){ //Modificación Versión 2.1

	int _numCitas=0;
	int _numPac = numPacientes();

	for(int i=0; i<_numPac; i++){
		_numCitas += totCitasPaciente(i);
	}

	//~ cout << "totalCitas: " << _numCitas << endl;
	return _numCitas;
}

int Archivo::totCitasPaciente(int i){ //Versión 2.1
	int nCitasP = 0;
	int nTratP = 0;

	nTratP = numTratamientosPac(i);

	for(int c=0; c<nTratP;c++){
		nCitasP += numCitas(i, c);
	}

	return nCitasP;
}

int Archivo::numIntervalos(){ //OK
	vector<int> auxDisp = dispPaciente(0);
	return (int)auxDisp.size();
}

int Archivo::numIntervalosDia(void){ //OK
	int nInt = numIntervalos();
	return (int)nInt/5;
}

//-------------- METODOS RELACIONADOS CON LAS ESPECIALIDADES -----------------//

//Se cuenta apartir de Especialidad 0
string Archivo::infoEspecialidad(int x){ // O.K
	
	string _infoEspecialidad = "";
	
	if(x>=numEspecialidades() || x<0){
		cout<<"Número de Especialidad Incorrecto"<<endl;
		exit(1);
	}
	else{
         _infoEspecialidad = lines[4+x].c_str();
	     //cout<<_infoEspecialidad;		
	}

    return _infoEspecialidad;		
}

string Archivo::strIdEspecialidad(int x){ // O.K

     string strId = "";
     int esp = idEspecialidad(x);

	 strId = itostr(esp);        	      
    //cout<<strId<<endl;    
    return strId;
}

int Archivo::idEspecialidad(int x){ // O.K

    int id = 0;
    string esp = infoEspecialidad(x);
    
    id = getField(esp, 1);
    //cout<<id<<endl;
    
    return id;    
}

vector <int> Archivo::listaIdEspecialidades(){ // O.K

	vector <int> lista;
	for(int i=0; i<numEspecialidades(); i++){
		lista.push_back(idEspecialidad(i));
	}
	return lista;
}

string Archivo::nombreEspecialidadID(int id){ //Versión 2.1 OK

	vector <int> listaIDs(listaIdEspecialidades());
	vector <string> listaNombres(listaNomEspecialidades());
	int pos = get_position(listaIDs, id);
	string nombre = listaNombres[pos];

	return nombre;
}

string Archivo::nomEspecialidad(int j){ // O.K //Modificación Versión 2.1

	string nombre = "";
    string cadena = infoEspecialidad(j);

    string caracter("'");
    size_t nomIni, nomFin;
    int pos_i, pos_f;

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


vector <string> Archivo::listaNomEspecialidades(){ // O.K

	vector <string> lista;
	for(int i=0; i<numEspecialidades(); i++){
		lista.push_back(nomEspecialidad(i));
	}
	return lista;
}


int Archivo::capEspecialidad(int j){ //Versión 2.1

	string cadena;
	int intCap = 0;

	cadena = subcadenaEspecialidad(j);
	intCap = getField(cadena, 1);

    return intCap;
}

vector<int> Archivo::listaCapEspecialidad(){ //Versión 2.1
	vector <int> lista;
	for(int i=0; i<numEspecialidades(); i++){
		lista.push_back(capEspecialidad(i));
	}
	return lista;
}

int Archivo::duracionCita(int j){ //Versión 2.1

	string cadena;
	int duracion = 0;

	cadena = subcadenaEspecialidad(j);
	duracion = getField(cadena, 2);

	return duracion;
}

vector<int> Archivo::listaDuracionesCitas(){ //Versión 2.1
	vector <int> lista;
	for(int i=0; i<numEspecialidades(); i++){
		lista.push_back(duracionCita(i));
	}
	return lista;
}


int Archivo::numCitasEsp(int i){ //Modificación Versión 2.1 donde i es el id de la especialidad

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


int Archivo::numPacEsp(int i){ //Modificación Versión 2.1 donde i es el id de la Especialidad

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
string Archivo::infoEspecialista(int j){ // O.K
	
	string _infoEspecialista = "";
	
	if(j>=numEspecialistas() || j<0){
		cout<<"No Existe el Especialista Número ["<<j<<"]"<<endl;
		exit(1);
	}
	else{
         _infoEspecialista = lines[4 + j + numEspecialidades()].c_str();
	     //cout<<_infoEspecialista;		
	}

    return _infoEspecialista;
}

int Archivo::idEspecialista(int j){ // O.K

    int id = 0;
    string esp = infoEspecialista(j); 
    
    id = getField(esp, 1);
    //cout<<id<<endl;
    
    return id;    
}	

vector <int> Archivo::listaIdEspecialistas(){ // O.K
	vector <int> lista;
	for(int i=0; i<numEspecialistas(); i++){
		lista.push_back(idEspecialista(i));
	}
	return lista;
}

//CREAR MÉTODO PARA CREAR LA LISTA DE NÚMERO DE ESPECIALIDADES DE PROFESIONAL ------------------
int Archivo::numEspProfesional(int j){ //Versión 2.1

	int intProf = 0;
    string esp = infoEspecialista(j);
    intProf = getField(esp, 2);

    return intProf;
}

string Archivo::datosDispProfEspecialidad(int j, int e){ //Versión 2.1 ~
	string esp = "";
	esp = datosDispProfesional(j)[e];
	return esp;
}


vector <string> Archivo::datosDispProfesional(int j){ //Versión 2.1

	string esp = "";
	esp = infoEspecialista(j);
	esp = esp.substr(itostr(idEspecialista(j)).length()+itostr(numEspProfesional(j)).length()+2);

	vector <string> cadenas (subcadenasCorchetes(esp));

	return cadenas;
}


int Archivo::obtenerEspecialidadProf(int j, int e){ //Versión 2.1
	
	int intProf = 0;
	vector <string> esp (datosDispProfesional(j));
	intProf = getField(esp[e],1);

	return intProf;
}

//Profesiones Especialista j
vector <int> Archivo::especialidadesProf(int j){ //Modificación Versión 2.1

	vector <int> intProf;
	int numEP = numEspProfesional(j);

	for(int i=0; i<numEP;i++){
		intProf.push_back(obtenerEspecialidadProf(j,i));
	}

	return intProf;
}

vector <vector<int> > Archivo::listaEspecialidadesProfs(){ //Modificación Versión2.1
	vector <vector<int> > lista;
	for(int i=0; i<numEspecialistas(); i++){
		lista.push_back(especialidadesProf(i));
	}
	return lista;
}

int Archivo::obtenerCapacidad(int j, int e){ //Versión 2.1

	int intCap;
	vector <string> esp (datosDispProfesional(j));
	intCap = capEspecialidad(obtenerEspecialidadProf(j, e));

	return intCap;
}

vector <int> Archivo::capEspecialista(int j){ //Modificación Versión2.1

	vector <int> intCap;
   	int numEP = numEspProfesional(j);

    for(int i=0; i<numEP;i++){
    	intCap.push_back(obtenerCapacidad(j, i));
    }

    return intCap;
}

vector <vector<int> >Archivo::listaCapEspecialistas(){ //Modificación Versión 2.1
	vector <vector <int> > lista;
	for(int i=0; i<numEspecialistas(); i++){
		lista.push_back(capEspecialista(i));
	}
	return lista;
}


vector <string> Archivo::strDispEspecialista(int j){ //Modificación Versión2.1

    vector <string> disponibilidad = datosDispProfesional(j);
    size_t pos;

    int numEP = numEspProfesional(j);

    for(int i=0; i<numEP; i++){
    	pos = disponibilidad[i].find("-");
    	disponibilidad[i] = disponibilidad[i].substr(pos+2);
    }

    return disponibilidad;
}

vector<int> Archivo::dispProfesionalEsp(int j, int e){ //Versión 2.1 ~
	vector <int> dispo;
	dispo = dispEspecialista(j)[e];
	return dispo;
}

vector <vector<int> > Archivo::dispEspecialista(int j){ //Modificación Versión2.1

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
string Archivo::infoPaciente(int i){ //OK
	
	string _infoPaciente = "";

	if(i>=numPacientes() || i<0){
		cout<<"No Existe el Paciente Número ["<<i<<"]"<<endl;
		exit(1);
	}
	else{
		 _infoPaciente = lines[4 + i + numEspecialidades() + numEspecialistas()].c_str();
	     //cout<<_infoPaciente<<endl;		
	}
    
    return _infoPaciente;		
}


int Archivo::idPaciente(int i){ //OK

    int id = 0;
    string pac = infoPaciente(i);
    
    id = getField(pac, 1);
    //cout<<id<<endl;
    
    return id;    
}

vector <int> Archivo::listaIdPacientes(){ //OK
	vector <int> lista;
	for(int i=0; i<numPacientes(); i++){
		lista.push_back(idPaciente(i));
	}
	return lista;
}

//CREAR UN MÉTODO PARA LA LISTA DE TRATAMIENTOS----------------------------------
int Archivo::numTratamientosPac(int i){ //Versión 2.1

	int nTratamientos = 0;
    string infoPac = infoPaciente(i);

    nTratamientos = getField(infoPac, 2);

    return nTratamientos;
}

vector<string> Archivo::datosTratamientosPaciente(int i){ //Versión 2.1

	string pac = "";
	pac = infoPaciente(i);
	int pos = pac.find("-");
	int posIni = itostr(idPaciente(i)).length()+itostr(numTratamientosPac(i)).length()+2;
	pac = pac.substr(posIni, pos-posIni);

	vector <string> cadenas (subcadenasCorchetes(pac));

	return cadenas;

}

int Archivo::obtenerEspecialidadPac(int i, int e){

	int intEsp;
	vector <string> datos (datosTratamientosPaciente(i));
	intEsp = getField(datos[e],1);

	return intEsp;
}

// Especialidad que requiere el Paciente i
vector <int> Archivo::espPaciente(int i){ //Modificación Versión2.1
	
	vector <int> intEsp;
    int numTrat = numTratamientosPac(i);

    for(int n=0; n<numTrat; n++){
		intEsp.push_back(obtenerEspecialidadPac(i, n));
	}

    return intEsp;
}

vector <vector<int> > Archivo::listaEspPacientes(){ //Modificación Versión2.1
	vector <vector<int> > lista;
	int numP = numPacientes();
	for(int i=0; i<numP; i++){
		lista.push_back(espPaciente(i));
	}

	return lista;
}

// Número de Citas que requiere el Paciente i en la especialidad e
int Archivo::numCitas(int i, int e){  //Modificación Versión 2.1
	
	int intCitas;
	vector <string> datos (datosTratamientosPaciente(i));
	intCitas = getField(datos[e],2);

	return intCitas;
}

vector <int> Archivo::numeroCitasPac(int i){
	vector <int> intCitas;
	int numTrat = numTratamientosPac(i);

	for(int n=0; n<numTrat; n++){
		intCitas.push_back(numCitas(i,n));
	}

	return intCitas;
}

// Lista con el numero de citas
vector <vector<int> > Archivo::listaNumCitas(){ //Modificación Versión 2.1
	vector <vector<int> > lista;
	for(int i=0; i<numPacientes(); i++){
		lista.push_back(numeroCitasPac(i));
	}
	return lista;
}


string Archivo::strDispPaciente(int i){ //OK
	string disponibilidad = ""; 
    size_t pos;
    
    disponibilidad = infoPaciente(i);
	pos = disponibilidad.find("-");   
	disponibilidad = disponibilidad.substr(pos+2); 
	
    return disponibilidad;
}

vector <int> Archivo::dispPaciente(int i){ //OK
	
	vector <int> vectorAux;
	string strDisponibilidad = strDispPaciente(i);
	vectorAux = splitStrDisponibilidad(strDisponibilidad);
	 
	return vectorAux;
}

vector <vector<int> > Archivo::listaDispPacientes(){ //OK
	vector <vector<int> > lista;
	for(int i=0; i<numPacientes(); i++){
		lista.push_back(dispPaciente(i));
	}
	return lista;
}


//--------------- MÉTODOS AUXILIARES -----------------//


string Archivo::itostr(int num){
	stringstream ss;
	ss << num;
	return ss.str();
}


int Archivo::getField(string cadena, int field){
	
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


vector <int> Archivo::splitStrDisponibilidad(string cadena){
	
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

string Archivo::subcadenaEspecialidad(int j){ //Versión 2.1

	string esp;
    esp = infoEspecialidad(j);
    //cout<<esp<<endl;

    int pos = 0;
    pos = strIdEspecialidad(j).length()+nomEspecialidad(j).length()+4;

	string cadena;
	cadena = esp.substr(pos);

	return cadena;
}


vector <string> Archivo::subcadenasCorchetes(string cadena){ //Versión2.1 OK

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

int Archivo::get_position(vector <int> vector, int num){ //Versión2.1 OK

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
