#include <gecode/int/linear.hh>
#include <gecode/minimodel.hh>

#include <cmath>
#include "ACiTOptions.h"
#include "lectura.h"
#include "heuristica.cpp"
#include "escritura.h"
#include "cita.h"

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
	//Se utiliza para restriccion dia diferente entre dos citas distintas y dia igual para el tiempo de inicio y el tiempo de fi de una cita
	vector<IntVarArray> listaResultDia;
	//Se utiliza unicamente para la restricción de dia igual para el tiempo de inicio y el tiempo de fi de una cita
	vector<IntVarArray> listaResultDia2;

	vector<vector<int> > listaDuracionesPac;

	vector<IntArgs> listaDuracion;
	vector<IntArgs> listaRecursos;
	vector<IntArgs> listaCapacidad;
	vector<IntSet> listaCodigos;

	vector<Especialidad> listaEspecialidades;
	vector<Paciente> listaPacientes;

	int intDia; //intervalos por dia
	int num_semanas;
	int slDia; //slots por dia

	Escritura * e;

public:
	// GETTERS
	vector<Especialidad> lstEspecialidades() {
		return listaEspecialidades;
	}

	vector<Paciente> lstPacientes() {
		return listaPacientes;
	}


	ACiTConstraints(const ACiTOptions& opt)
	: especialistas(*this, opt.totalCitas(), opt.listaCodEspecialistas()),
	  t_inicio(*this, opt.totalCitas(), 0, opt.makespan()),
	  t_fin(*this, opt.totalCitas(), 1, opt.makespan()) {
		/************** Asignaciones Previas **************/
		listaPacientes = opt.listaPacientes();
		listaEspecialidades = opt.listaEspecialidades();

		e = new Escritura("/home/holmes/workspace/output_files");
		e->semanas(opt.semanas());
		e->slotsIntervalo(opt.slotsIntervalo());
		e->slotsDia(opt.slotsDia());
		e->intervalosDia(opt.intervalosDia());
		e->intervalosSemana(opt.intervalosSemana());

		intDia = opt.intervalosDia();
		num_semanas = opt.semanas();
		slDia = opt.slotsDia();

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
			cout << listaEspecialidades[i].nombre() << " citas: " << listaEspecialidades[i].totalCitas() << endl;
			listaVarEspecialistas.push_back(IntVarArray(*this,
					listaEspecialidades[i].totalCitas(),
					listaCodigos[i]));//especialistas
			listaVarTInicio.push_back(IntVarArray(*this,listaEspecialidades[i].totalCitas(),0,opt.makespan()));//tiempos de inicio
			listaVarTFin.push_back(IntVarArray(*this,listaEspecialidades[i].totalCitas(),1,opt.makespan()));//tiempos de fin
			listaSlotsDia.push_back(
					IntVarArray(*this, listaEspecialidades[i].totalCitas(), 0, opt.slotsDia()));
			listaResultDia.push_back(
					IntVarArray(*this, listaEspecialidades[i].totalCitas(), 0, opt.slotsDia()));
			listaResultDia2.push_back(
					IntVarArray(*this, listaEspecialidades[i].totalCitas(), 0, opt.slotsDia()));
		}

//		cout << "inicio restricciones" << endl;

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
				dispPi = transformarDisponibilidad(pacEsp_i[i].disponibilidad(), listaEspecialidades[esp_i].duracionCitasSlots(), opt.slotsIntervalo());
				for(int j = 0; j < pacEsp_i[i].nCitas(listaEspecialidades[esp_i].id()); j++){
					//Restricción 1: Recortar disponibilidades de pacientes (preasignacion de tiempo)
					for (int k = 0; k < (int) dispPi.size(); k++) {
						if (dispPi[k] == 0) {
							rel(*this, listaVarTInicio[esp_i][citEsp_i], IRT_NQ, k, opt.icl());
						}
					}

					if(j == 0){
						//Restricción de preferencia
						if(pacEsp_i[i].especialistaPref(listaEspecialidades[esp_i].id()) != 0){
							cout << "Activada restriccion de preferencia: " << endl;
							cout << "Paciente: " << pacEsp_i[i].id() << endl;
							cout << "Profesional: " << pacEsp_i[i].especialistaPref(listaEspecialidades[esp_i].id()) << endl;
							rel(*this, listaVarEspecialistas[esp_i][citEsp_i], IRT_EQ, pacEsp_i[i].especialistaPref(listaEspecialidades[esp_i].id()), opt.icl());
							cout << endl;
						}
					}

					if(j < pacEsp_i[i].nCitas(listaEspecialidades[esp_i].id())-1){
						//Restricción 2: Mismo especialista para todas las citas de un paciente
						rel(*this, listaVarEspecialistas[esp_i][citEsp_i], IRT_EQ, listaVarEspecialistas[esp_i][citEsp_i+1], opt.icl());
//						rel(*this, listaVarTFin[esp_i][citEsp_i], IRT_LE, listaVarTInicio[esp_i][citEsp_i+1], opt.icl());

						//Restricción 3: Dia diferente para dos citas distintas
						//Asignacion previa para restriccion 3
						rel(*this, listaSlotsDia[esp_i][citEsp_i], IRT_EQ, opt.slotsDia(), opt.icl());
						rel(*this, listaSlotsDia[esp_i][citEsp_i+1], IRT_EQ, opt.slotsDia(), opt.icl());
						//Division de a/(num intervalos dia)=x y a+1/(num intervalos dia)=y para despues asegurarse que x<y
						div(*this,  listaVarTFin[esp_i][citEsp_i], listaSlotsDia[esp_i][citEsp_i], listaResultDia[esp_i][citEsp_i], opt.icl());
						div(*this,  listaVarTInicio[esp_i][citEsp_i+1], listaSlotsDia[esp_i][citEsp_i+1], listaResultDia[esp_i][citEsp_i+1], opt.icl());
						rel(*this, listaResultDia[esp_i][citEsp_i], IRT_LE, listaResultDia[esp_i][citEsp_i+1], opt.icl());

						//Restricción 4: Dia Igual para una misma cita
						//División de b/(num intervalos dia) = z
						div(*this,  listaVarTInicio[esp_i][citEsp_i], listaSlotsDia[esp_i][citEsp_i], listaResultDia2[esp_i][citEsp_i], opt.icl());
						rel(*this, listaResultDia[esp_i][citEsp_i], IRT_EQ, listaResultDia2[esp_i][citEsp_i], opt.icl());
						if(j == pacEsp_i[i].nCitas(listaEspecialidades[esp_i].id())-2){
							div(*this,  listaVarTInicio[esp_i][citEsp_i+1], listaSlotsDia[esp_i][citEsp_i], listaResultDia2[esp_i][citEsp_i+1], opt.icl());
							rel(*this, listaResultDia[esp_i][citEsp_i+1], IRT_EQ, listaResultDia2[esp_i][citEsp_i+1], opt.icl());
						}
					}
					citEsp_i++;
				}
			}
			esp_i++;
			citEsp_i=0;
		}
//		cout << "inicio no solapamiento" << endl;

		//Restriccion No solapamiento entre los tratamientos de un paciente
		for(int i = 0; i < (int)listaPacientes.size(); i++){
			if(listaPacientes[i].nTratamientos()>1){
//				cout << listaPacientes[i].nombre() << endl;
				vector<vector<int> > infT = infoTratamientos(listaPacientes[i]);

				if( (int)infT.size() == listaPacientes[i].nTratamientos() ) {
					int nEsp = 0, cit = infT[nEsp][2], sigEsp = 1;
//					cout << "nT: " << listaPacientes[i].nTratamientos() << endl;
//					cout << "cit== " << tiemposInicio[esp].size() << endl;
					while(nEsp < int(infT.size())- 1){
//						cout << "esp: " << nEsp << " - " << cit << endl;
						// _infoEspecialidad[N][0] obtiene el id de la especialidad numero N
						for(int j=infT[sigEsp][2]; j< infT[sigEsp][3]; j++){
//							cout << "\tesp: " << sigEsp << " - " << j << endl;
							for(int k=0; k<listaPacientes[i].duracionCitTrat(infT[nEsp][0]); k++){
								rel(*this, listaVarTInicio[infT[sigEsp][1]][j] != listaVarTInicio[infT[nEsp][1]][cit] + k, opt.icl());
							}
						}
						cit++;
						if(cit == infT[nEsp][3]){
							cit=0;
							sigEsp++;
							if(sigEsp == (int)infT.size()){
								nEsp++;
								sigEsp=nEsp+1;
							}
						}
					}
				}else {
					cout << "Paciente: " <<  listaPacientes[i].nombre()
						 << "\nError tamaño de _infoTratamientos no coincide con el numero de tratamientos" << endl;
				}
			}
		}
//		cout << "fin no solapamiento" << endl;

		//Aplicamos la restriccion cumulatives para cada una de las especialidades
		//p+i= e
		for(int i=0; i<(int)listaEspecialidades.size(); i++){
			for(int j=0; j<listaVarTInicio[i].size(); j++){
				rel(*this, listaVarTFin[i][j] == listaDuracion[i][j]+listaVarTInicio[i][j], opt.icl());
			}
		}

		Gecode::wait(*this, especialistas, &recDispEsp, opt.icl());

		for(int i=0; i<(int)listaEspecialidades.size(); i++){
			cumulatives(*this, listaVarEspecialistas[i], listaVarTInicio[i], listaDuracion[i], listaVarTFin[i], listaRecursos[i], listaCapacidad[i], true, opt.icl());
		}
//		cout << "fin restricciones" << endl;
		/************ Estrategias de Busqueda ************/
		//branch(*this, especialistas, INT_VAR_NONE, INT_VAL_RND, VarBranchOptions::time(),ValBranchOptions::time());
		distribuidor(*this, especialistas, listaEspecialidades);
		branch(*this, t_inicio, INT_VAR_NONE, INT_VAL_MIN);
		branch(*this, t_fin, INT_VAR_NONE, INT_VAL_MIN);
//		branch(*this, t_inicio, INT_VAR_NONE, INT_VAL_MAX);
//		branch(*this, t_fin, INT_VAR_NONE, INT_VAL_MAX);
//		branch(*this, t_inicio, INT_VAR_NONE, INT_VAL_RND, VarBranchOptions::time(),ValBranchOptions::time());
//		branch(*this, t_fin, INT_VAR_NONE, INT_VAL_RND, VarBranchOptions::time(),ValBranchOptions::time());
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
		vector<Especialidad> lstEspecialidades = listaEspecialidades;

//		os << "\n------- CITAS ASIGNADAS POR ESPECIALIDAD -------" << endl;
		vector<Paciente> pacientes;
		int esp_i=0;
		int citEsp_i=0;
		while(esp_i < (int) lstEspecialidades.size()) {
//			os << "\nEspecialidad " << lstEspecialidades[esp_i].id() << endl;
			pacientes = lstEspecialidades[esp_i].pacientes();
			for(int i = 0; i < (int) pacientes.size(); i++){
//				os << "\nPaciente: " << pacientes[i].id() << "\tCitas: " << pacientes[i].nCitas(listaEspecialidades[esp_i].id()) << endl;
				for(int j = 0; j < pacientes[i].nCitas(lstEspecialidades[esp_i].id()); j++) {
					pacientes[i].insertarCita(Cita(lstEspecialidades[esp_i].id(),
							listaVarTInicio[esp_i][citEsp_i].val(),
							listaVarTFin[esp_i][citEsp_i].val(),
							e->determinarDiaInt(listaVarTInicio[esp_i][citEsp_i].val())
					));
//					os << "Cita " << j+1 << endl;
//					os << "\tInicio: " << listaVarTInicio[esp_i][citEsp_i].val() << " -> " << e->determinarDia(listaVarTInicio[esp_i][citEsp_i].val()) << "|" << e->determinarHora(listaVarTInicio[esp_i][citEsp_i].val())
//																	   << " Fin: " << listaVarTFin[esp_i][citEsp_i].val() << " -> " << e->determinarDia(listaVarTFin[esp_i][citEsp_i].val()) << "|" << e->determinarHora(listaVarTFin[esp_i][citEsp_i].val())
//																	   << " Especialista: " << listaVarEspecialistas[esp_i][citEsp_i].val() << endl;

					if(j == (pacientes[i].nCitas(listaEspecialidades[esp_i].id()) - 1)){
						Especialista auxEsp = lstEspecialidades[esp_i].buscarEspecialista(listaVarEspecialistas[esp_i][citEsp_i].val());
						auxEsp.insertarPaciente(pacientes[i]);
						lstEspecialidades[esp_i].actualizarEspecialista(auxEsp);
					}
					citEsp_i++;
				}
			}
			esp_i++;
			citEsp_i=0;
		}

		e->escribirXml(lstEspecialidades);
		delete e;
	}

	vector<vector<int> > infoTratamientos(Paciente p){
		vector<vector<int> > infoT;
		vector<int> auxInfo(4);
		for(int i = 0; i < (int)listaEspecialidades.size(); i++){
			if(p.buscaEspecialidadPac(listaEspecialidades[i].id())){
				vector<Paciente> aux = listaEspecialidades[i].pacientes();
				int inicio = 0, citas = 0;
				for(int j = 0; j < (int)aux.size(); j++){
					citas = aux[j].nCitas(listaEspecialidades[i].id());
					if(aux[j].id() == p.id()){
						auxInfo[0] = listaEspecialidades[i].id(); //Id de la especialidad
						auxInfo[1] = i;				//posicion de la especialidad en el vector listaEspecialidades
						auxInfo[2] = inicio;		//posicion de inicio de las citas para esa especialidad
						auxInfo[3] = inicio+citas;	//posicion de fin de las citas para esa especialidad
						infoT.push_back(auxInfo);
					}
					inicio += citas;
				}
			}
		}
		return infoT;
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

	vector<int> transformarDisponibilidad(vector<int> disp, int durCitEspSlot, int slotsIntervalo){
		int slSem = slDia*(disp.size()/intDia);
		int tam = slSem*num_semanas;
		vector<int> aux(tam);

//		cout << "tam: " << tam << endl;

		/*
		 * Primero se duplica cada valor en el vector de disponibilidades, tantas veces como
		 * numero de slots hayan por cada intervalo, debido a que el vector de disponibilidades
		 * esta dado por intervalos que representan 60 minutos cada uno, pero no todas las citas
		 * consumen ese intervalo, por ende se debe hacer esta conversion
		 */
		int pos, sem=0;
		while(sem < num_semanas){
			pos = 0;
			for(int i=(slSem*sem); i <(slSem*(sem+1)); i++){
				if((i % slDia) != 48){
					pos = (
						  (int)floor((i % slDia) / slotsIntervalo) +
						  (intDia*((int)floor(i/slDia)))
						  ) % (int)disp.size(); // i/slDia = numero de dia del slot
				}
				aux[i] = disp[pos];
			}
			sem++;
		}

		disp.clear(); disp.resize(aux.size());

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
				disp[i] = 1;
			}else{
				disp[i] = 0;
			}
//			cout << dispP[i] << " ";
			suma = 0;
		}
//		cout << endl;
		return aux;
	}

	static void recDispEsp(Space& _home){
		ACiTConstraints& home = static_cast<ACiTConstraints&>(_home);
		vector<Especialista> profEsp_i; //Vector auxiliar que contendra los profesionales pertenecientes a la especialidad i
		vector<int> dispEi; //Vector auxiliar que guarda las disponibilidades de los profesionales

		int esp_i=0;
		while (esp_i < (int)home.listaEspecialidades.size()) {
			profEsp_i = home.listaEspecialidades[esp_i].especialistas();
//			cout << home.listaEspecialidades[esp_i].nombre() << " - " << home.listaEspecialidades[esp_i].nEspecialistas() << endl;
			for(int i=0; i<(int)profEsp_i.size();i++){
//				cout << profEsp_i[i].horariosAtencionEsp(home.listaEspecialidades[esp_i].id())[3] << endl;
				dispEi = home.transformarDisponibilidad(
									profEsp_i[i].horariosAtencionEsp(home.listaEspecialidades[esp_i].id()),
									home.listaEspecialidades[esp_i].duracionCitasSlots(),
									12);
//				cout << "dispEi: " << dispEi.size() << endl;
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
