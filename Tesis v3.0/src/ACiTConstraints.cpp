#include <gecode/int/linear.hh>
#include <gecode/minimodel.hh>

#include <cmath>
#include "ACiTOptions.h"
#include "lectura.h"
#include "heuristica.cpp"

using namespace std;

class ACiTConstraints : public Script {

private:
	IntVarArray especialistas;
	IntVarArray t_inicio;
	IntVarArray t_fin;

	vector<IntVarArray> listaVarEspecialistas;
	vector<IntVarArray> listaVarTInicio;
	vector<IntVarArray> listaVarTFin;
	vector<IntVarArray> listaSlotsDia;
	vector<IntVarArray> listaResultDia;

	vector<IntArgs> listaDuracion;
	vector<IntArgs> listaRecursos;
	vector<IntArgs> listaCapacidad;
	vector<IntSet> listaCodigos;

	vector<Especialidad> listaEspecialidades;
	vector<Paciente> listaPacientes;

public:
	ACiTConstraints(const ACiTOptions& opt)
	: especialistas(*this, opt.totalCitas(), opt.listaCodEspecialistas()),
	  t_inicio(*this, opt.totalCitas(), 0, opt.makespan()),
	  t_fin(*this, opt.totalCitas(), 1, opt.makespan()) {
		/************** Asignaciones Previas **************/

		listaPacientes = opt.listaPacientes();
		listaEspecialidades = opt.listaEspecialidades();

		// Creacion de los IntArgs que representan las capacidades
		for(int i=0; i<(int)listaEspecialidades.size(); i++){

			//IntArgs auxiliares para las capacidades
			int arrAuxCap[listaEspecialidades[i].nEspecialistas()];
//			cout << "Especialidad: " << listaEspecialidades[i].nombre() << endl;
			for(int j=0; j<listaEspecialidades[i].nEspecialistas(); j++){
				arrAuxCap[j] = listaEspecialidades[i].capacidad();
//				cout << "arrAuxCap[" << j <<"]: " << arrAuxCap[j] << endl;
			}
			listaCapacidad.push_back(IntArgs(listaEspecialidades[i].nEspecialistas(), arrAuxCap));

			//IntArgs auxiliar para los recursos
			int arrAux[listaEspecialidades[i].totalCitas()];
			for(int j=0; j<listaEspecialidades[i].totalCitas(); j++){
				arrAux[j] = 1;
			}
			listaRecursos.push_back(IntArgs(listaEspecialidades[i].totalCitas(), arrAux));

			//IntArgs auxiliar para las duraciones
			int arrAuxD[listaEspecialidades[i].totalCitas()];
			for(int j=0; j<listaEspecialidades[i].totalCitas(); j++){
				arrAuxD[j] = listaEspecialidades[i].duracionCitasSlots();
			}
			listaDuracion.push_back(IntArgs(listaEspecialidades[i].totalCitas(), arrAuxD));

			listaCodigos.push_back(IntSet(listaEspecialidades[i].idEspecialistasArray(),listaEspecialidades[i].nEspecialistas()));
		}

		for(int i=0; i<(int)listaEspecialidades.size(); i++){
			listaVarEspecialistas.push_back(IntVarArray(*this,
					listaEspecialidades[i].totalCitas(),
					listaCodigos[i]));//especialistas
			listaVarTInicio.push_back(IntVarArray(*this,listaEspecialidades[i].totalCitas(),0,opt.makespan()));//tiempos de inicio
			listaVarTFin.push_back(IntVarArray(*this,listaEspecialidades[i].totalCitas(),1,opt.makespan()));//tiempos de fin
			listaSlotsDia.push_back(
					IntVarArray(*this, listaEspecialidades[i].totalCitas(), 0, opt.slotsDia()));
			listaResultDia.push_back(
					IntVarArray(*this, listaEspecialidades[i].totalCitas(), 0, opt.slotsDia()));
		}
		/************** Restricciones **************/
		//Preservar coherencia
		int tam = 0;
		for(int i=0; i<(int)listaEspecialidades.size(); i++){
			for(int j=0; j<listaVarEspecialistas[i].size(); j++){
				rel(*this, listaVarEspecialistas[i][j], IRT_EQ, especialistas[tam], opt.icl());
				rel(*this, listaVarTInicio[i][j], IRT_EQ, t_inicio[tam], opt.icl());
				rel(*this, listaVarTFin[i][j], IRT_EQ, t_fin[tam], opt.icl());
				tam++;
			}
		}

		//Restricción 1: Recortar disponibilidades de pacientes (preasignacion de tiempo)
		vector<Paciente> pacEsp_i; //Vector auxiliar que contendra los pacientes pertenecientes a la especialidad i
		vector<int> dispPi; //Vector auxiliar que guarda las disponibilidades (dadas en slots) de los pacientes
		int citEsp_i=0; //Contador que me servira para recorrer las variables de tiempos de inicio de la especialidad i
		int esp_i=0;

		while(esp_i < (int) listaEspecialidades.size()){
//			cout << "Especialidad: " << listaEspecialidades[esp_i].nombre() << endl;
			pacEsp_i = listaEspecialidades[esp_i].pacientes();
			for(int i = 0; i < listaEspecialidades[esp_i].nPacientes(); i++){
				dispPi = transformarDisponibilidad(pacEsp_i[i], listaEspecialidades[esp_i].duracionCitasSlots(), opt.slotsIntervalo());
				for(int j = 0; j < pacEsp_i[i].nCitas(listaEspecialidades[esp_i].id()); j++){
					//Restricción 1: Recortar disponibilidades de pacientes (preasignacion de tiempo)
					for (int k = 0; k < (int) dispPi.size(); k++) {
						if (dispPi[k] == 0) {
							rel(*this, listaVarTInicio[esp_i][citEsp_i], IRT_NQ, k, opt.icl());
						}
					}

					if(j < pacEsp_i[i].nCitas(listaEspecialidades[esp_i].id())-1){
						//Restricción 2: Mismo especialista para todas las citas de un paciente
						rel(*this, listaVarEspecialistas[esp_i][citEsp_i], IRT_EQ, listaVarEspecialistas[esp_i][citEsp_i+1], opt.icl());

						rel(*this, listaVarTFin[esp_i][citEsp_i], IRT_LE, listaVarTInicio[esp_i][citEsp_i+1], opt.icl());
						/*
						//Restricción 3: Dia diferente
						//Asignacion previa para restriccion 3
						rel(*this, listaSlotsDia[esp_i][citEsp_i], IRT_EQ, opt.slotsDia(), opt.icl());
						rel(*this, listaSlotsDia[esp_i][citEsp_i+1], IRT_EQ, opt.slotsDia(), opt.icl());
						//Division de a/(num intervalos dia)=x y a+1/(num intervalos dia)=y para despues asegurarse que x<y
						div(*this,  listaVarTInicio[esp_i][citEsp_i], listaSlotsDia[esp_i][citEsp_i], listaResultDia[esp_i][citEsp_i], opt.icl());
						div(*this,  listaVarTInicio[esp_i][citEsp_i+1], listaSlotsDia[esp_i][citEsp_i+1], listaResultDia[esp_i][citEsp_i+1], opt.icl());
						rel(*this, listaResultDia[esp_i][citEsp_i], IRT_LE, listaResultDia[esp_i][citEsp_i+1], opt.icl());
*/
					}
					citEsp_i++;
				}
			}
			esp_i++;
			citEsp_i=0;
		}

		//Aplicamos la restriccion cumulatives para cada una de las especialidades
		//p+i= e
		for(int i=0; i<(int)listaEspecialidades.size(); i++){
			for(int j=0; j<listaVarTInicio[i].size(); j++){
				rel(*this, listaVarTFin[i][j] == listaDuracion[i][j]+listaVarTInicio[i][j], opt.icl());
			}
		}

		//Gecode::wait(*this, especialistas, &recDispEsp, opt.icl());

		for(int i=0; i<(int)listaEspecialidades.size(); i++){
			cumulatives(*this, listaVarEspecialistas[i], listaVarTInicio[i], listaDuracion[i], listaVarTFin[i], listaRecursos[i], listaCapacidad[i], true, opt.icl());
		}

		/************ Estrategias de Busqueda ************/
		//branch(*this, especialistas, INT_VAR_NONE, INT_VAL_RND, VarBranchOptions::time(),ValBranchOptions::time());
		distribuidor(*this, especialistas, listaEspecialidades);
		branch(*this, t_inicio, INT_VAR_NONE, INT_VAL_MIN);
		branch(*this, t_fin, INT_VAR_NONE, INT_VAL_MIN);
	}

	/// Constructor for cloning \a s
	ACiTConstraints(bool share, ACiTConstraints& s) : Script(share, s) {
		especialistas.update(*this, share, s.especialistas);
		t_inicio.update(*this, share, s.t_inicio);
		t_fin.update(*this, share, s.t_fin);
	}

	/// Perform copying during cloning
	virtual Space* copy(bool share) {
		return new ACiTConstraints(share,*this);
	}

	/// Print solution
	virtual void
	print(std::ostream& os) const{
		for(int i=0; i<(int)listaEspecialidades.size(); i++){
			Especialidad aux = listaEspecialidades[i];
			os << "\nEspecialidad: " << aux.id() << "-> ";
			for(int j=0; j<aux.nEspecialistas(); j++){
				vector<Especialista> aux2 = aux.especialistas();
				if(aux2[j].buscaEspecialidadProf(aux.id())){
					os << "Especialista " << aux2[j].id() << " ";
				}
			}
		}

		os << "\n------- CITAS ASIGNADAS POR ESPECIALIDAD -------" << endl;
		vector<Paciente> pacientes;
		int esp_i=0;
		int citEsp_i=0;
		while(esp_i < (int) listaEspecialidades.size()) {
			os << "\nEspecialidad " << listaEspecialidades[esp_i].id() << endl;
			pacientes = listaEspecialidades[esp_i].pacientes();
			for(int i = 0; i < (int) pacientes.size(); i++){
				os << "\nPaciente: " << pacientes[i].id() << "\tCitas: " << pacientes[i].nCitas(listaEspecialidades[esp_i].id()) << endl;
				for(int j = 0; j < pacientes[i].nCitas(listaEspecialidades[esp_i].id()); j++) {
					os << "Cita " << j+1 << endl;
					os << "\tInicio: " << listaVarTInicio[esp_i][citEsp_i].val()
										   << " Fin: " << listaVarTFin[esp_i][citEsp_i].val()
										   << " Especialista: " << listaVarEspecialistas[esp_i][citEsp_i].val() << endl;
					citEsp_i++;
				}
			}
			esp_i++;
			citEsp_i=0;
		}
	}

	void distribuidor(Home home, const IntVarArgs& x, vector<Especialidad> lstEsp){
		if(home.failed()) return;
		ViewArray<Int::IntView> y(home, x);
		//inicializacion del vector de prioridades
		vector<vector<int> > p(lstEsp.size());
		//cout << "tamaño p: " << p.size() << endl;
		for(int i=0; i<(int)p.size(); i++){
			p[i].resize(lstEsp[i].nEspecialistas());
			//cout << "p[" << i << "]: " << p[i].size() << endl;
		}
		Distribuidor::post(home, y, lstEsp, p);
	}

	vector<int> transformarDisponibilidad(Paciente p, int durCitEspSlot, int slotsIntervalo){
		vector<int> dispP = p.disponibilidad();
		vector<int> aux(dispP.size() * slotsIntervalo);

		/*
		 * Primero se duplica cada valor en el vector de disponibilidades, tantas veces como
		 * numero de slots hayan por cada intervalo, debido a que el vector de disponibilidades
		 * esta dado por intervalos que representan 60 minutos cada uno, pero no todas las citas
		 * consumen ese intervalo, por ende se debe hacer esta conversion
		 */
		for(int i=0; i < (int) dispP.size(); i++){
			for(int j = (i * slotsIntervalo); j< (i+1) * slotsIntervalo; j++){
				aux[j] = dispP[i];
			}
		}

		dispP.clear(); dispP.resize(aux.size());

		/*
		 * Luego vamos a calcular que slots de tiempo son los candidatos como tiempos de inicio
		 * para las citas, si un numero de slot es candidato, el valor que toma es 1, de lo
		 * contrario el valor es 0. Se determina que un slot es candidato cuando el numero de
		 * slots siguientes en 1 es igual a durCitEspSlot
		 */
		int suma=0;
//		cout << p.nombre() << ": ";
		for(int i=0; i< ((int)aux.size()) - durCitEspSlot; i++){
			for(int j=i; j<i+durCitEspSlot; j++){
				suma += aux[j];
			}
			if(suma==durCitEspSlot){
				dispP[i] = 1;
			}else{
				dispP[i] = 0;
			}
//			cout << dispP[i] << " ";
			suma = 0;
		}
//		cout << endl;
		return dispP;
	}

	static void recDispEsp(Space& _home){
		ACiTConstraints& home = static_cast<ACiTConstraints&>(_home);
		vector<Especialista> profEsp_i; //Vector auxiliar que contendra los profesionales pertenecientes a la especialidad i
		vector<int> dispEi; //Vector auxiliar que guarda las disponibilidades de los profesionales

		int esp_i=0;
		while (esp_i < (int)home.listaEspecialidades.size()) {
			profEsp_i = home.listaEspecialidades[esp_i].especialistas();
			for(int i=0; i<(int)profEsp_i.size();i++){
				dispEi = profEsp_i[i].horariosAtencionEsp(home.listaEspecialidades[esp_i].id());
				for(int j=0; j<home.especialistas.size(); j++){
					if(home.especialistas[j].val() == profEsp_i[i].id()){
						for (int k = 0; k < (int) dispEi.size(); k++) {
							if (dispEi[k] == 0) {
								rel(home, home.t_inicio[j], IRT_NQ, k);
							}
						}
					}
				}
			}
			esp_i++;
		}
	}
};
