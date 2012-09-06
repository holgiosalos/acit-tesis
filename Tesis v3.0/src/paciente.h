#ifndef PACIENTE_H  
#define PACIENTE_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "cita.h"

using namespace std;

class Paciente
{
	private:
		int _id;
		int _nTratamientos;
		string _nombre;
		vector<int> _idEspecialidades;
		vector<vector<int> > _infoTratamientos;
		vector<int> _duracionesT;
		vector<int> _disponibilidad;
		vector<Cita> _listaCitas;

	public:
		Paciente(int i, string n, int nT, vector<vector<int> > iT, vector<int> d);
		~Paciente();
		
		void id(int i);
		int id(void) const;

		string nombre(void) const;

		void nTratamientos(int nT);
		int nTratamientos(void);

		int nCitas(int idE) const;
		
		void insertarCita(Cita c);
		void listaCitas(vector<Cita> lc);
		vector<Cita> listaCitas(void) const;

		void disponibilidad(vector<int> d);
		vector<int> disponibilidad(void);

		int duracionCitTrat(int idE); //obtiene la duracion en slots
		vector<int> duracionesT(void);

		bool buscaEspecialidadPac(int id);

};

#endif /* PACIENTE_H */ 
