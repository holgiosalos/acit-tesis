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
		vector<int> _idEspecialidades;
		vector<int> _idCitas;
		vector<vector<int> > _disponibilidades;
		
	public:
		Especialista(int i, int nE, vector<int> iE, vector<vector<int> > d);

		void id(int i);
		int id(void);

		void idCitas(vector<int> citas);
		vector<int> idCitas(void);

		void idEspecialidades(vector<int> idE);
		vector<int> idEspecialidades(void);

		void nEspecialidades(int nE);
		int nEspecialidades(void);

		void disponibilidades(vector<vector<int> > d);
		vector<vector<int> > disponibilidades(void);

		bool buscaEspecialidadProf(int id);
};

#endif /* ESPECIALISTA_H */ 

