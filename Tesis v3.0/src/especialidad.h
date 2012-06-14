#ifndef ESPECIALIDAD_H  
#define ESPECIALIDAD_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "especialista.h"
#include "paciente.h"

using namespace std;

class Especialidad {
private:
	int _id;
	int _totalCitas; // Numero de citas de la especialidad
	int _nEspecialistas; // Numero de especialistas en la especialidad
	int _nPacientes; // Numero de pacientes en la especialidad
	string _nombre;
	vector<int> _idEspecialistas;
	vector<int> _idPacientes;
	vector<Especialista> _especialistas; //Lista de especialistas pertenecientes a la especialidad
	vector<Paciente> _pacientes; //Lista de pacientes que necesitan ser atendidos en dicha especialidad

public:
	Especialidad(int i, string n, int nC, int nE, int nP,
			vector<Especialista> esp, vector<Paciente> pac,
			vector<int> idsE, vector<int> idsP);

	int id(void) const;

	void totalCitas(int nC);
	int totalCitas(void);

	void nEspecialistas(int nE);
	int nEspecialistas(void);

	void nPacientes(int nP);
	int nPacientes(void);

	void especialistas(vector<Especialista> esp);
	vector<Especialista> especialistas();

	void idEspecialistas(vector<int> ids);
	int* idEspecialistasArray(void);
	vector<int> idEspecialistasVector(void);

	void idPacientes(vector<int> ids);
	int* idPacientesArray(void);
	vector<int> idPacientesVector(void);

	void pacientes(vector<Paciente> esp);
	vector<Paciente> pacientes(void) const;
};

#endif /* ESPECIALIDAD_H */ 
