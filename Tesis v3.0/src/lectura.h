#ifndef LECTURA_H
#define LECTURA_H

#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

class Lectura
{
	private:
		string nombre;
		vector <string> lines;		
	public:
		Lectura();
		Lectura(string n);
		void nombreArchivo(string n);
		string nombreArchivo();	
		
		void saveLines();
		vector <string> getLines();
		
		int numEspecialistas();
		int numEspecialidades();
		int numPacientes();
		int totCitas();
		int totCitasPaciente(int i);
		
		string infoEspecialidad(int x); 
            string strIdEspecialidad(int x);
            int idEspecialidad(int x);
            string nombreEspecialidadID(int id); //Retorna el nombre de la especialidad con el identificador proporcionado
            string nomEspecialidad(int j);
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
            string nomEspecialista(int j); //Versión Lectura
            vector <string> listaNomEspecialistas(); //Versión Lectura
            string subcadenaProfesional(int j); //Versión Lectura
            int numEspProfesional(int j); //Obtner el numero de especialidades del profesional numero j
            string datosDispProfEspecialidad(int j, int e); //Versión 2.1
            vector <string> datosDispProfesional(int j); //Versión 2.1
			int obtenerEspecialidadProf(int j, int e); //Obtener la especialidad numero e del profesional j
			vector <int> especialidadesProf(int j); //Obtener el total de especialidades del profesional j
			vector<int> listaIdEspecialistas(); //Obtener el id de todos los profesionales
			vector <vector<int> > listaEspecialidadesProfs(); //Obtener el id de todas las especialidades de todos los profesionales
			int obtenerCapacidad(int j, int e); //Obtener la capacidad del profesional j en la especialidad e
			vector <int> capEspecialista(int j); //Lista con las capacidades de todas las especialidades del profesional j
			vector <vector<int> >listaCapEspecialistas(); //Obtener las capacidades de todos los profesionales
			vector <string> strDispEspecialista(int j);
			vector <int> dispProfesionalEsp(int j, int e); //Horarios de atencion del profesional j en la especialidad e
			vector <vector<int> > dispEspecialista(int j); //Lista de todos los horarios de atencion en todas las especialidades del profesional j
			vector <vector<int> > listaDispEspecialistas(); //Lista de los horarios de atencion de todos los profesionales.
					
		string infoPaciente(int i);
            int idPaciente(int i); //Obtener el ID del paciente numero i
            string nomPaciente(int j); // Versión Lectura
            vector <string> listaNomPacientes(); // Versión Lectura
            string subcadenaPaciente(int j); //Versión Lectura
            int obtenerEspecialidadPac(int i, int e); //Obtener el ID de la especialidad numero e del paciente numero i
			vector <int> espPaciente(int i);  //Obtener el conjunto de especialidades que requiere el paciente i
			vector<int>listaIdPacientes(); //Obtener el conjunto de IDs de los pacientes.
			vector <string> datosTratamientosPaciente(int i); //Obtiene id y numero de citas de todos los tratamientos
			int numTratamientosPac(int i); //Obtener el total de tratamientos del paciente numero i
			vector <vector<int> > listaEspPacientes(); //Matriz que retorna todas las especialidades por todos los pacientes
			int numCitas(int i, int e); //Numero de citas del paciente numero i en la especialidad numero e
			vector <int> numeroCitasPac(int i); //Obtener el numero de citas del paciente numero i en todas sus especialidades
			vector <vector<int> > listaNumCitas(); //Obtener todas las citas de todos los pacientes
			int duracionCitTrat(int i, int e); //NUEVO METODO SE NECESITÓ PARA RESTRICCION DE NO SOLAPAMIENTO ENTRE TRATAM.

			int profPrefPac(int i, int e);  //Versión Lectura
			vector <int> listaProfPrefPac(int i); //Versión Lectura

			string strDispPaciente(int i); 
			vector <int> dispPaciente(int i); //Obtener la disponibilidad del paciente numero i
			vector <vector<int> > listaDispPacientes();
		
		string itostr(int num);
		int getField(string cadena, int field);
		vector<int> splitStrDisponibilidad(string cadena);
		vector<string> subcadenasCorchetes(string cadena);
		int get_position(vector <int> vector, int pos);
};

#endif
