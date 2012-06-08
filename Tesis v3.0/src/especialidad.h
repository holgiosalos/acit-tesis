#ifndef ESPECIALIDAD_H  
#define ESPECIALIDAD_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "especialista.h"
#include "paciente.h"

using namespace std;

class Especialidad
{
	private:
		int _id;
		int _totalCitas; // Numero de citas de la especialidad
		int _nEspecialistas; // Numero de especialistas en la especialidad
		int _nPacientes; // Numero de pacientes en la especialidad
		vector<int> _idEspecialistas;
		vector<Especialista> _especialistas; //Lista de especialistas pertenecientes a la especialidad
		vector<Paciente> _pacientes; //Lista de pacientes que necesitan ser atendidos en dicha especialidad
		
	public:
		Especialidad(int i, int nC, int nE, int nP);
		Especialidad(int i, int nC, int nE, int nP, vector<Especialista> esp, vector<Paciente> pac, vector<int> ids);
		int id() const;
		void totalCitas(int nC);
		int totalCitas();
		void nEspecialistas(int nE) ;
		int nEspecialistas();
		void nPacientes(int nP);
		int nPacientes();
		void especialistas(vector<Especialista> esp);
		vector<Especialista> especialistas();
		void idEspecialistas(vector<int> ids);
		int* idEspecialistas();
		void pacientes(vector<Paciente> esp);
		vector<Paciente> pacientes() const;
};

#endif /* ESPECIALIDAD_H */ 
