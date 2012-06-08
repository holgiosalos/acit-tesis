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
		int _idEspecialidad;
		int _nCitas;
		vector<int> _disponibilidad;
		vector<int> _idCitas; 
		vector<int> _idEspecialista; //Id del especialista asignado a la cita

	public:
		Paciente(int i, int nC);
		Paciente(int i, int nC, int iE);
		Paciente(int i, int nC, int iE, vector<int> d);
		
		void idCitas(vector<int> citas);
		vector<int> idCitas();
		
		void idEspecialista(vector<int> idEsp);
		vector<int> idEspecialista();
		
		void idEspecialidad(int idE);
		int idEspecialidad(void) const;
		
		void nCitas(int c);
		int nCitas(void) const;
		
		void id(int i);
		int id();
		
		void disponibilidad(int d[], int size);
		vector<int> disponibilidad();

};

#endif /* PACIENTE_H */ 
