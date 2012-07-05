#ifndef ESPECIALISTA_H  
#define ESPECIALISTA_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Especialista
{
	private:
		int _id;
		int _nEspecialidades;
		string _nombre;
		vector<int> _idEspecialidades;
		vector<int> _idCitas;
		vector<vector<int> > _horariosAtencion;
		
	public:
		Especialista(int i, string n, int nE, vector<int> iE, vector<vector<int> > hsA);

		void id(int i);
		int id(void);

		string nombre(void);

		void idCitas(vector<int> citas);
		vector<int> idCitas(void);

		void idEspecialidades(vector<int> idE);
		vector<int> idEspecialidades(void);

		void nEspecialidades(int nE);
		int nEspecialidades(void);

		void horariosAtencion(vector<vector<int> > hsA);
		vector<vector<int> > horariosAtencion(void);
		vector<int> horariosAtencionEsp(int idE);

		bool buscaEspecialidadProf(int id);
};

#endif /* ESPECIALISTA_H */ 

