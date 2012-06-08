#ifndef ESPECIALISTA_H  
#define ESPECIALISTA_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Especialista
{
	private:
		int _id;
		int _capacidad;
		int _idEspecialidad;
		vector<int> _idCitas;
		vector<int> _disponibilidad;
		
	public:
		Especialista(int i, int c, int iE);
		Especialista(int i, int c, int iE, vector<int> d);

		void idCitas(vector<int> citas);
		vector<int> idCitas();

		int idEspecialidad();
		void idEspecialidad(int idE);

		void id(int i);
		int id();

		int capacidad();
		void capacidad(int c);

		void disponibilidad(int d[], int size);
		vector<int> disponibilidad();
};

#endif /* ESPECIALISTA_H */ 

