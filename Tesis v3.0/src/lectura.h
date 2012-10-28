#ifndef LECTURA_H
#define LECTURA_H

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
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
		//CONSTRUCTORES
		Lectura();
		~Lectura();
		Lectura(string n); //Constructor cuyo parámetro es el nombre de archivo
		//MÉTODOS GENERALES
		void nombreArchivo(string n); //Fija el nombre del archivo
		string nombreArchivo();	//Retorna el nombre del archivo
		void saveLines(); //Almacena las lineas del archivo en el vector "lines"
		vector <string> getLines(); //Retorna el vector "lines"
		int numEspecialistas();	//Devuelve el número de Especialistas (Profesionales)
		int numEspecialidades(); //Devuelve el número de Especialidades
		int numPacientes();	//Devuelve el número de Pacientes
		int totCitasPaciente(int i); //Obtiene el número total de citas del Paciente i
		int totCitas();	//Obtiene el número total de citas de todos los Pacientes
		int numIntervalos(); //Retorna el número de Intervalos referentes a la disponibilidad
		int numIntervalosDia(void); //Retorna el número de Intervalos por día

		//MÉTODOS RELACIONADOS CON LAS ESPECIALIDADES
		string infoEspecialidad(int e); //Obtiene la información de la Especialidad que se encuentra en la posición e
            string strIdEspecialidad(int e); //Retorna el identificador de tipo string de la Especialidad que se encuentra en la posición e
            int idEspecialidad(int e); //Retorna el identificador de tipo int de la Especialidad que se encuentra en la posición e
            string nomEspecialidad(int e); //Retorna el nombre de la Especialidad que se encuentra en la posición e
            string nombreEspecialidadID(int id); //Retorna el nombre de la Especialidad dependiendo del identificador proporcionado
            int capEspecialidad(int e);	//Retorna el límite de pacientes (capacidad) de la Especialidad que se encuentra en la posición e
            int duracionCita(int e); //Retorna la duración de la cita de la Especialidad que se encuentra en la posición e
            //Métodos que retornan listas
            vector <int> listaIdEspecialidades(); //Devuelve la lista de identificadores de todas las Especialidades
			vector <string> listaNomEspecialidades(); //Devuelve la lista de nombres de todas las Especialidades
			vector <int> listaCapEspecialidad(); //Devuelve la lista de límites de pacientes (capacidad) de todas las Especialidades
			vector <int> listaDuracionesCitas(); //Devuelve la lista de duraciones de las citas de todas las Especialidades
			//Métodos Anexos
			int numCitasEsp(int id); //Obtiene el número de citas que deben atenderse en la Especialidad que posee el identificador "id"
			int numPacEsp(int id); //Obtiene el número de citas que requiere un paciente en la Especialidad que posee el identificador "id"
			string subcadenaEspecialidad(int e); //Crea una subcadena de la información que se encuentra después del nombre de la Especialidad, esto es:  capacidad y duración

		//MÉTODOS RELACIONADOS CON LOS PROFESIONALES
		//CAMBIO OCT O2
		string infoEspecialista(int j); //Obtiene la información del Profesional que se encuentra en la posición j
            int idEspecialista(int j); //Retorna el identificador del Profesional que se encuentra en la posición j
            string nomEspecialista(int j); //Retorna el nombre del Profesional que se encuentra en la posición j
            int numEspProfesional(int j); //Retorna el número de Especialidades del Profesional que se encuentra en la posición j
            int obtenerEspecialidadProf(int j, int e); //Retorna el Identificador de la Especialidad e del Profesional que se encuentra en la posición j
            int obtenerCapacidad(int j, int e); //Retorna el límite de pacientes (capacidad) del Profesional j en la Especialidad e
            //Métodos referentes a la disponibilidad
            //CAMBIO OCT 02
            vector <string> datosDispProfesional(int j); //Retorna el vector con los datos de las disponibilidades (IdEspecialidad - Disponibilidad) del Profesional j en las distintas Especialidades
            string datosDispProfEspecialidad(int j, int e); //Retorna el vector con los datos de la disponibilidad (IdEspecialidad - Disponibilidad) del Profesional j en la Especialiad e
            vector <string> strDispEspecialista(int j); //Retorna el vector de disponibilidades (de tipo string) del Profesional j en las distintas Especialidades
            vector <vector<int> > dispEspecialista(int j); //Retorna la lista de todos los Horarios de Atención en todas las Especialidades del Profesional j
            vector <int> dispProfesionalEsp(int j, int e); //Retorna los Horarios de Atención del Profesional j en la Especialidad e
            //Métodos que retornan listas
            vector <int> listaIdEspecialistas(); //Devuelve la lista de Identificadores de todos los Profesionales
            vector <string> listaNomEspecialistas(); //Devuelve la lista de Nombres de todos los Profesionales
            vector <int> especialidadesProf(int j); //Devuelve la lista de Identificadores de las Especialidades del Profesional j
            vector <int> capEspecialista(int j); //Devuelve la lista con las capacidades (límites de pacientes) de todas las Especialidades del Profesional j
            vector <vector<int> > listaEspecialidadesProfs(); //Devuelve la matriz con los Identificadores de todas las Especialidades de todos los Profesionales
			vector <vector<int> >listaCapEspecialistas(); //Devuelve la matriz con las capacidades (límites de pacientes) de todas las Especialidades de todos los Profesionales
			vector <vector<int> > listaDispEspecialistas(); //Devuelve la matriz con los Horarios de Atención de todos los Profesionales
			//ELIMINÉ MÉTODO SUBCADENAPROFESIONAL(int j)

		//MÉTODOS RELACIONADOS CON LOS PACIENTES
		string infoPaciente(int i); //Obtiene la información del Paciente que se encuentra en la posición i
            int idPaciente(int i); //Retorna el Identificador del Paciente que se encuentra en la posición i
            string nomPaciente(int j); //Retorna el Nombre del Paciente que se encuentra en la posición i
            int numTratamientosPac(int i); //Retorna el número de tratamientos (Especialidades que requiere) del Paciente que se encuentra en la posición i
            int numCitas(int i, int e); //Retorna el número de citas en las que el Paciente i requiere atención en la Especialidad e
            int obtenerEspecialidadPac(int i, int e); //Retorna el Identificador de la Especialidad e que requiere el Paciente i
            int profPrefPac(int i, int e); //Retorna el Identificador del Profesional que requiere el Paciente i en la Especialidad e
            string strDispPaciente(int i); //Retorna los horarios en los cuales está disponible el Paciente i (en formato string)
            int duracionCitTrat(int i, int e);
            //Métodos que retornan listas
            vector<int>listaIdPacientes(); //Devuelve la lista de Identificadores de todos los Pacientes
            vector <string> listaNomPacientes(); //Devuelve la lista de Nombres de todos los Pacientes
            vector <int> espPaciente(int i);  //Devuelve la lista de Especialidades que requiere el Paciente i
            vector <int> numeroCitasPac(int i); //Devuelve el número de citas que requiere el Paciente i en todas sus Especialidades
            vector <vector<int> > listaEspPacientes(); //Devuelve la matriz con todas las Especialidades que requieren todos los Pacientes
            vector <vector<int> > listaNumCitas(); //Devuelve el número de citas de todos los Pacientes
            vector <string> datosTratamientosPaciente(int i); //Devuelve la información de todos los tratamientos que requiere el Paciente i
            												  //(IdEspecialidad, número de citas y profesional de preferencia)
            vector <int> listaProfPrefPac(int i); //Devuelve la lista de Profesionales de Preferencia del Paciente i
            vector <int> dispPaciente(int i); //Devuelve la disponibilidad del Paciente i
            vector <vector<int> > listaDispPacientes(); //Devuelve la disponibilidad de todos los Pacientes (en formato entero)

            //Métodos anexos
            string subcadenaPaciente(int i); //Crea una subcadena de la información que se encuentra después del nombre del Paciente

        //MÁS MÉTODOS ANEXOS
		string itostr(int num); //Convierte un número entero a tipo string
		int getField(string cadena, int field); //Obtiene el campo que se encuentra en la posición "field" de una cadena
		int get_position(vector <int> vector, int num); //Obtiene la posición del elemento "num" dentro del vector "vector"
		vector <string> subcadenasCorchetes(string cadena); //Separa la información contenida en los corchetes
		vector <int> splitStrDisponibilidad(string cadena); //Convierte la cadena de disponibilidades en un vector de enteros
		//CREACIÓN OCT 02
		int get_pos_info_prof(int j); //Devuelve la línea en la cual se encuentra la información del Profesional j
		//CREACIÓN OCT 02
		int get_nro_total_EP(); //Obtiene la suma del número de Especialidades de todos los Profesionales
};

#endif
