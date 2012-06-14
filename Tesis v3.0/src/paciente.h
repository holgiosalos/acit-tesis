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
		vector<int> _nCitas;
		vector<int> _disponibilidad;
		vector<int> _idCitas; 
		vector<int> _idEspecialista; //Id del especialista asignado a la cita

	public:
		Paciente(int i, int nT, vector<int> nC, vector<int> iE, vector<int> d);
		
		void idCitas(vector<int> citas);
		vector<int> idCitas(void);
		
		void idEspecialista(vector<int> idEsp);
		vector<int> idEspecialista(void);
		
		void idEspecialidades(vector<int> idE);
		vector<int> idEspecialidades(void) const;
		
		void nTratamientos(int nT);
		int nTratamientos(void);

		void nCitas(vector<int> c);
		vector<int> nCitas(void) const;
		
		void id(int i);
		int id(void);
		
		void disponibilidad(vector<int> d);
		vector<int> disponibilidad(void);

		bool buscaEspecialidadPac(int id);

};

#endif /* PACIENTE_H */ 
