#ifndef ARCHIVO_H  
#define ARCHIVO_H

#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

class Archivo
{
	private:
		string nombre;
		vector <string> lines;		
	public:
		Archivo();
		Archivo(string n);
		void nombreArchivo(string n);
		string nombreArchivo();	
		void getInfoCita(int i);	
		
		void saveLines();
		vector <string> getLines();
		
		int numEspecialistas();
		int numEspecialidades();
		int numPacientes();
		int numSemanas(); //Versión 2.1
		int totCitas();
		int totCitasPaciente(int i);
		int numIntervalos();
		int numIntervalosDia(void);
		
		string infoEspecialidad(int x); 
            string strIdEspecialidad(int x);
            int idEspecialidad(int x);
            string nomEspecialidad(int id);
			vector<string> listaNomEspecialidades();
		    vector<int> listaIdEspecialidades();
		    string subcadenaEspecialidad(int j); //Versión 2.1
		    int capEspecialidad(int j); //Versión 2.1
		    vector <int>listaCapEspecialidad(); //Versión 2.1
		    int duracionCita(int j); //Versión 2.1
		    vector<int> listaDuracionesCitas(); //Versión 2.1
			int numCitasEsp(int i);
			int numPacEsp(int i);
		
		string infoEspecialista(int j);
            int idEspecialista(int j);
            int numEspProfesional(int j); //Versión 2.1
            string datosDispProfEspecialidad(int j, int e); //Versión 2.1
            vector <string> datosDispProfesional(int j); //Versión 2.1
			int obtenerEspecialidadProf(int j, int e);
			vector <int> profEspecialista(int j);
			vector<int> listaIdEspecialistas();
			vector <vector<int> > listaProfEspecialistas();
			int obtenerCapacidad(int j, int e);
			vector <int> capEspecialista(int j);
			string nombreEspecialidadID(int id);
			vector <vector<int> >listaCapEspecialistas();
			vector <string> strDispEspecialista(int j);
			vector <int> dispProfesionalEsp(int j, int e);
			vector <vector<int> > dispEspecialista(int j);
			vector <vector<int> > listaDispEspecialistas(); //Delete en Versión2.1
					
		string infoPaciente(int i);
            int idPaciente(int i);
            int obtenerEspecialidadPac(int i, int e);
			vector <int> espPaciente(int i);
			vector<int>listaIdPacientes();
			vector <string> datosTratamientosPaciente(int i);
			int numTratamientosPac(int i); //Versión 2.1
			vector <vector<int> > listaEspPacientes();
			int numCitas(int i, int e); //Modificación Versión 2.1
			vector <int> numeroCitasPac(int i); //Versión 2.1
			vector <vector<int> > listaNumCitas(); //Modificación Versión 2.1
			string strDispPaciente(int i); 
			vector <int> dispPaciente(int i); 
			vector <vector<int> > listaDispPacientes();
		
		string itostr(int num);
		int getField(string cadena, int field);
		vector<int> splitStrDisponibilidad(string cadena);
		vector<string> subcadenasCorchetes(string cadena);
		int get_position(vector <int> vector, int pos);
};

#endif 
