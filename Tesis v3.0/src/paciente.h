#ifndef PACIENTE_H  
#define PACIENTE_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Paciente
{
	private:
		int _id;
		int _nTratamientos;
		vector<int> _idEspecialidades;
		vector<vector<int> > _infoTratamientos;
		vector<int> _disponibilidad;

	public:
		Paciente(int i, int nT, vector<vector<int> > iT, vector<int> d);
		
		int nCitas(int idE);

		void nTratamientos(int nT);
		int nTratamientos(void);
		
		void id(int i);
		int id(void);
		
		void disponibilidad(vector<int> d);
		vector<int> disponibilidad(void);

		bool buscaEspecialidadPac(int id);

};

#endif /* PACIENTE_H */ 
