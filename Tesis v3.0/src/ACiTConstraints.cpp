#include <gecode/int/linear.hh>
#include <gecode/minimodel.hh>
#include <cmath>

#include "ACiTOptions.h"
#include "lectura.h"
#include "escritura.h"
#include "heuristica.cpp"
#include "cita.h"

using namespace std;
using namespace Gecode;

class ACiTConstraints : public MaximizeScript {

private:
	IntVarArray especialistas;
	IntVarArray t_inicio;
	IntVarArray t_fin;
	BoolVarArray preferencias;
	IntVar maximo;

	vector<IntVarArray> listaVarEspecialistas;
	vector<IntVarArray> listaVarTInicio;
	vector<IntVarArray> listaVarTFin;
	//Se utiliza para restriccion dia diferente entre dos citas distintas y dia igual para el tiempo de inicio y el tiempo de fi de una cita
	vector<IntVarArray> listaResultDia;
	//Se utiliza unicamente para la restricción de dia igual para el tiempo de inicio y el tiempo de fi de una cita
	vector<IntVarArray> listaResultDia2;

	vector<vector<int> > listaDuracionesPac;

	vector<IntVarArray> listaSlotsDia;
	vector<IntArgs> listaDuracion;
	vector<IntArgs> listaRecursos;
	vector<IntArgs> listaCapacidad;
	vector<IntSet> listaCodigos;

	vector<Especialidad> * listaEspecialidades;
	vector<Paciente> listaPacientes;

	int intDia; //intervalos por dia
	int intSem;
	int num_semanas;
	int slInt;
	int slDia; //slots por dia
	int nPacPref;
	int makespan;

	Escritura * writer;

public:
	// Distintas opciones de búsqueda i.e "Branching variants"
	enum {					// NOMBRE BÚSQUEDA | DESCRIPCIÓN
		BRANCH_DEFAULT, 	//   0 = Default   | sel-var: dom-wdeg, sel-val: random
		BRANCH_PACIENTES,	//   1 = branch1   | sel-val: menor-num-pacientes
		BRANCH_CITAS,		//   2 = branch2   | sel-val: menor-num-citas
	};

	vector<Especialidad>* getListaEspecialidades() const
	{
	    return listaEspecialidades;
	}

	void setListaEspecialidades(vector<Especialidad>* listaEspecialidades)
	{
	    this->listaEspecialidades = listaEspecialidades;
	}

	ACiTConstraints(const ACiTOptions& opt)
	: especialistas(*this, opt.totalCitas(), opt.listaCodEspecialistas()),
	  t_inicio(*this, opt.totalCitas(), 0, opt.makespan()),
	  t_fin(*this, opt.totalCitas(), 1, opt.makespan()),
	  preferencias(*this, opt.nPacientesPreferencia(), 0, 1),
	  maximo(*this, 0, opt.nPacientesPreferencia()) {
//		cout << "init constraints" << endl;

		/************** Asignaciones Previas **************/
		listaPacientes = opt.listaPacientes();
		setListaEspecialidades(opt.listaEspecialidades());

		writer = new Escritura("/var/www/ACiT/sites/default/files/acit_files/output_files_acit");
		writer->semanas(opt.semanas());
		writer->slotsIntervalo(opt.slotsIntervalo());
		writer->slotsDia(opt.slotsDia());
		writer->intervalosDia(opt.intervalosDia());
		writer->intervalosSemana(opt.intervalosSemana());

		intDia = opt.intervalosDia();
		intSem = opt.intervalosSemana();
		num_semanas = opt.semanas();
		slDia = opt.slotsDia();
		slInt = opt.slotsIntervalo();
		nPacPref = opt.nPacientesPreferencia();
		makespan = opt.makespan();

		// Creacion de los IntArgs que representan las capacidades
		for(int i=0; i<(int)listaEspecialidades->size(); i++){
			//IntArgs auxiliares para las capacidades
			int arrAuxCap[listaEspecialidades->at(i).nEspecialistas()];
			for(int j=0; j<listaEspecialidades->at(i).nEspecialistas(); j++){
				arrAuxCap[j] = listaEspecialidades->at(i).capacidad();
			}
			listaCapacidad.push_back(IntArgs(listaEspecialidades->at(i).nEspecialistas(), arrAuxCap));

			//IntArgs auxiliar para los recursos
			int arrAux[listaEspecialidades->at(i).totalCitas()];
			for(int j=0; j<listaEspecialidades->at(i).totalCitas(); j++){
				arrAux[j] = 1;
			}
			listaRecursos.push_back(IntArgs(listaEspecialidades->at(i).totalCitas(), arrAux));

			//IntArgs auxiliar para las duraciones
			int arrAuxD[listaEspecialidades->at(i).totalCitas()];
			for(int j=0; j<listaEspecialidades->at(i).totalCitas(); j++){
				arrAuxD[j] = listaEspecialidades->at(i).duracionCitasSlots();
			}
			listaDuracion.push_back(IntArgs(listaEspecialidades->at(i).totalCitas(), arrAuxD));

			listaCodigos.push_back(IntSet(listaEspecialidades->at(i).idEspecialistasArray(),listaEspecialidades->at(i).nEspecialistas()));
		}

		for(int i=0; i<(int)listaEspecialidades->size(); i++){
			listaVarEspecialistas.push_back(
				IntVarArray(*this, listaEspecialidades->at(i).totalCitas(), listaCodigos[i]));

			listaVarTInicio.push_back(
				IntVarArray(*this,listaEspecialidades->at(i).totalCitas(),0,opt.makespan()));

			listaVarTFin.push_back(
				IntVarArray(*this,listaEspecialidades->at(i).totalCitas(),1,opt.makespan()));

			listaSlotsDia.push_back(
				IntVarArray(*this, listaEspecialidades->at(i).totalCitas(), 0, opt.slotsDia()));
			listaResultDia.push_back(
				IntVarArray(*this, listaEspecialidades->at(i).totalCitas(), 0, opt.slotsDia()));
			listaResultDia2.push_back(
				IntVarArray(*this, listaEspecialidades->at(i).totalCitas(), 0, opt.slotsDia()));
		}

		for (int esp = 0; esp < (int)listaEspecialidades->size(); esp++)
		{
			rel(*this, listaSlotsDia[esp], IRT_EQ, opt.slotsDia(), opt.icl());
		}

		/************** Restricciones **************/
		//Preservar coherencia
		int tam = 0;
		for(int i=0; i<(int)listaEspecialidades->size(); i++){
			for(int j=0; j<listaVarEspecialistas[i].size(); j++){
				rel(*this, listaVarEspecialistas[i][j], IRT_EQ, especialistas[tam], opt.icl());
				rel(*this, listaVarTInicio[i][j], IRT_EQ, t_inicio[tam], opt.icl());
				rel(*this, listaVarTFin[i][j], IRT_EQ, t_fin[tam], opt.icl());
				tam++;
			}
		}

		//Restricción 1: Recortar disponibilidades de pacientes (preasignacion de tiempo)
		vector<Paciente> pacEsp_i; //Vector auxiliar que contendra los pacientes pertenecientes a la especialidad i
//		vector<int> dispPi; //Vector auxiliar que guarda las disponibilidades (dadas en slots) de los pacientes
		IntArgs dispPi; //Vector auxiliar que guarda las disponibilidades (dadas en slots) de los pacientes
		IntSet setDom;
		int citEsp_i=0; //Contador que me servira para recorrer las variables de tiempos de inicio de la especialidad i
		int esp_i=0;
		int itPref=0; //iterador del IntVarArray preferencias

		while(esp_i < (int) listaEspecialidades->size()){
			pacEsp_i = listaEspecialidades->at(esp_i).pacientes();
			for(int i = 0; i < listaEspecialidades->at(esp_i).nPacientes(); i++){
				dispPi = transformarDisponibilidadEsp(pacEsp_i[i].disponibilidad());
				for(int j = 0; j < pacEsp_i[i].nCitas(listaEspecialidades->at(esp_i).id()); j++){
					//Restricción 1: Recortar disponibilidades de pacientes (preasignacion de tiempo)
					setDom = IntSet(dispPi);
					dom(*this, listaVarTInicio[esp_i][citEsp_i], setDom, ICL_DOM);

					//Restricción de preferencia
					if(j == 0 && opt.preferencia()){
						if(pacEsp_i[i].especialistaPref(listaEspecialidades->at(esp_i).id()) != 0) {
							rel(
								*this,
								(listaVarEspecialistas[esp_i][citEsp_i] == pacEsp_i[i].especialistaPref(listaEspecialidades->at(esp_i).id()))
									== preferencias[itPref],
								opt.icl());
							itPref++;
						}
					}

					if(j < pacEsp_i[i].nCitas(listaEspecialidades->at(esp_i).id())-1){
						//Restricción 2: Mismo especialista para todas las citas de un paciente
						rel(*this, listaVarEspecialistas[esp_i][citEsp_i], IRT_EQ, listaVarEspecialistas[esp_i][citEsp_i+1], opt.icl());
//						rel(*this, listaVarTFin[esp_i][citEsp_i], IRT_LE, listaVarTInicio[esp_i][citEsp_i+1], opt.icl());

						//Restricción 3: Dia diferente para dos citas distintas
						//Division de a/(num intervalos dia)=x y a+1/(num intervalos dia)=y para despues asegurarse que x<y
						div(*this,  listaVarTFin[esp_i][citEsp_i], listaSlotsDia[esp_i][citEsp_i], listaResultDia[esp_i][citEsp_i], opt.icl());
						div(*this,  listaVarTInicio[esp_i][citEsp_i+1], listaSlotsDia[esp_i][citEsp_i+1], listaResultDia[esp_i][citEsp_i+1], opt.icl());
						rel(*this, listaResultDia[esp_i][citEsp_i], IRT_LE, listaResultDia[esp_i][citEsp_i+1], opt.icl());

						//Restricción 4: Dia Igual para una misma cita
						//División de b/(num intervalos dia) = z
						div(*this,  listaVarTInicio[esp_i][citEsp_i], listaSlotsDia[esp_i][citEsp_i], listaResultDia2[esp_i][citEsp_i], opt.icl());
						rel(*this, listaResultDia[esp_i][citEsp_i], IRT_EQ, listaResultDia2[esp_i][citEsp_i], opt.icl());
						if(j == pacEsp_i[i].nCitas(listaEspecialidades->at(esp_i).id())-2){
							div(*this, listaVarTInicio[esp_i][citEsp_i+1], listaSlotsDia[esp_i][citEsp_i], listaResultDia2[esp_i][citEsp_i+1], opt.icl());
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
		vector<vector<int> > infoNS;
		for(int i = 0; i < (int)listaPacientes.size(); i++){
			if(listaPacientes[i].nTratamientos()>1){
				infoNS = infoNoSolapamiento(listaPacientes[i]);

				if( (int)infoNS.size() == listaPacientes[i].nTratamientos() ) {
					int nEsp = 0, cit = infoNS[nEsp][2], sigEsp = 1;
					while(nEsp < int(infoNS.size())- 1){
						for(int j=infoNS[sigEsp][2]; j< infoNS[sigEsp][3]; j++){
							for(int k=0; k<listaPacientes[i].duracionCitTrat(infoNS[nEsp][0]); k++){
								rel(*this, listaVarTInicio[infoNS[sigEsp][1]][j] != listaVarTInicio[infoNS[nEsp][1]][cit] + k, opt.icl());
							}
						}
						cit++;
						if(cit == infoNS[nEsp][3]){
							cit=0;
							sigEsp++;
							if(sigEsp == (int)infoNS.size()){
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
			infoNS.clear();
		}
		infoNS.clear();

		//Aplicamos la restriccion cumulatives para cada una de las especialidades
		//p+i= e
		for(int i=0; i<(int)listaEspecialidades->size(); i++){
			for(int j=0; j<listaVarTInicio[i].size(); j++){
				rel(*this, listaVarTFin[i][j] == listaDuracion[i][j]+listaVarTInicio[i][j], opt.icl());
			}
		}

		Gecode::wait(*this, especialistas, &recDispEsp, opt.icl());

		for(int i=0; i<(int)listaEspecialidades->size(); i++){
			cumulatives(*this, listaVarEspecialistas[i], listaVarTInicio[i], listaDuracion[i], listaVarTFin[i], listaRecursos[i], listaCapacidad[i], true, opt.icl());
		}

		if (opt.preferencia())
		{
			rel(*this, maximo == sum(preferencias), opt.icl());
		}

//		cout << "end constraints" << endl;

		/************ Estrategias de Busqueda ************/
		switch(opt.branching())
		{
			case BRANCH_DEFAULT:
				branch(*this, especialistas, INT_VAR_SIZE_DEGREE_MIN, INT_VAL_RND, VarBranchOptions::time(),ValBranchOptions::time());
				break;

			case BRANCH_PACIENTES:
				distribuidor(*this, especialistas, listaEspecialidades, PACIENTES);
				break;
			case BRANCH_CITAS:
				distribuidor(*this, especialistas, listaEspecialidades, CITAS);
				break;
		}
		branch(*this, t_inicio, INT_VAR_NONE, INT_VAL_MIN);
		branch(*this, t_fin, INT_VAR_NONE, INT_VAL_MIN);
	}

	static void recDispEsp(Space& _home){
		ACiTConstraints& home = static_cast<ACiTConstraints&>(_home);
//		vector<int> dispEi; //Vector auxiliar que guarda las disponibilidades de los profesionales
		IntArgs dispEi; //Vector auxiliar que guarda las disponibilidades de los profesionales
		IntSet setDom;
		int id;

		int esp_i=0;
		while (esp_i < (int)home.listaEspecialidades->size()) {
			for(int i=0; i<(int)home.listaEspecialidades->at(esp_i).especialistas()->size(); i++){
				dispEi = home.transformarDisponibilidadEsp(
					home.listaEspecialidades->at(esp_i).especialistas()->at(i).horariosAtencionEsp(
						home.listaEspecialidades->at(esp_i).id()));
				id = home.listaEspecialidades->at(esp_i).especialistas()->at(i).id();
				setDom = IntSet(dispEi);
				for(int j=0; j<home.especialistas.size(); j++){
					if(home.especialistas[j].val() == id){
//						cout << setDom << endl;
						dom(home, home.t_inicio[j], setDom, ICL_DOM);
					}
				}
			}
			esp_i++;
		}
	}

	vector<vector<int> > infoNoSolapamiento(Paciente p){
		int inicio, citas;
		vector<vector<int> > infoNS;
		vector<int> auxInfo(4);
		vector<Paciente> aux;
		for(int i = 0; i < (int)listaEspecialidades->size(); i++){
			if(p.buscaEspecialidadPac(listaEspecialidades->at(i).id())){
				aux = listaEspecialidades->at(i).pacientes();
				inicio = 0; citas = 0;
				for(int j = 0; j < (int)aux.size(); j++){
					citas = aux[j].nCitas(listaEspecialidades->at(i).id());
					if(aux[j].id() == p.id()){
						auxInfo[0] = listaEspecialidades->at(i).id(); //Id de la especialidad
						auxInfo[1] = i;				//posicion de la especialidad en el vector listaEspecialidades
						auxInfo[2] = inicio;		//posicion de inicio de las citas para esa especialidad
						auxInfo[3] = inicio+citas;	//posicion de fin de las citas para esa especialidad
						infoNS.push_back(auxInfo);
					}
					inicio += citas;
					auxInfo.clear();
					auxInfo.resize(4);
				}
			}
		}
		auxInfo.clear();
		aux.clear();
		return infoNS;
	}

	vector<int> transformarDisponibilidad(vector<int> disp){
		int diaSem = int(floor(intDia/intSem)); // dias por semana
		int slSem = slInt * intSem + diaSem + 1;
		vector<int> aux(makespan);

		/*
		 * Primero se duplica cada valor en el vector de disponibilidades, tantas veces como
		 * numero de slots hayan por cada intervalo, debido a que el vector de disponibilidades
		 * esta dado por intervalos que representan 60 minutos cada uno, pero no todas las citas
		 * consumen ese intervalo, por ende se debe hacer esta conversion
		 */
		int pos, sem=0;
		while(sem < num_semanas){
			pos = 0;
			for(int i = (slSem * sem); i < (slSem * (sem+1)); i++){
				pos = (
					(int)floor((i % slDia) / slInt) +
					(intDia*((int)floor(i/slDia))) ) % intSem; // i/slDia = numero de dia del slot
				aux[i] = disp[pos];
			}
			sem++;
		}
		return aux;
	}

	IntArgs transformarDisponibilidadEsp(vector<int> disp){
//		int diaSem = int(floor(intDia/intSem)); // dias por semana
		int slSem =  785; //(slInt * intSem) + diaSem + 1;
		vector<int> aux(makespan);

		/*
		 * Primero se duplica cada valor en el vector de disponibilidades, tantas veces como
		 * numero de slots hayan por cada intervalo, debido a que el vector de disponibilidades
		 * esta dado por intervalos que representan 60 minutos cada uno, pero no todas las citas
		 * consumen ese intervalo, por ende se debe hacer esta conversion
		 */
		int pos, sem=0;
		while(sem < num_semanas){
			pos = 0;
			for(int i = (slSem * sem); i < (slSem * (sem+1)); i++){
				pos = (
						(int)floor((i % slDia) / slInt) +
						(intDia*((int)floor(i/slDia))) ) % intSem; // i/slDia = numero de dia del slot
				aux[i] = disp[pos];
			}
			sem++;
		}
		disp.clear();
		for (int i = 0; i < (int)aux.size(); i++)
		{
			if (aux[i] == 1) {
				disp.push_back(i);
			}
		}
		aux.clear();
		IntArgs result(disp);

		return result;
	}

	// Brancher
	void distribuidor(Home home, const IntVarArgs& x, vector<Especialidad>* lstEsp, int enfoque){
		if(home.failed()) return;

		ViewArray<Int::IntView> y(home, x);
		Distribuidor::post(home, y, lstEsp, enfoque); // numPacientes
	}

	/// Constructor for cloning \a s
	ACiTConstraints(bool share, ACiTConstraints& s) : MaximizeScript(share, s) {
		especialistas.update(*this, share, s.especialistas);
		t_inicio.update(*this, share, s.t_inicio);
		t_fin.update(*this, share, s.t_fin);
		preferencias.update(*this, share, s.preferencias);
		maximo.update(*this, share, s.maximo);
		writer = s.writer;
		listaVarTInicio = s.listaVarTInicio;
		listaVarTFin = s.listaVarTFin;
		listaVarEspecialistas = s.listaVarEspecialistas;

		listaEspecialidades = s.listaEspecialidades;
		for (int i=0; i < (int)listaEspecialidades->size(); i++)
		{
			listaVarTInicio[i].update(*this, share, s.listaVarTInicio[i]);
			listaVarTFin[i].update(*this, share, s.listaVarTFin[i]);
			listaVarEspecialistas[i].update(*this, share, s.listaVarEspecialistas[i]);
		}

		nPacPref = s.nPacPref;
		intDia = s.intDia;
		intSem = s.intSem;
		num_semanas = s.num_semanas;
		slDia = s.slDia;
		slInt = s.slInt;
		makespan = s.makespan;
	}

	/// Perform copying during cloning
	virtual Space* copy(bool share) {
		return new ACiTConstraints(share,*this);
	}

	// Cost function
	virtual IntVar cost(void) const {
		return maximo;
	}

	/// Print solution when the preference constraint is activated
	virtual void
	print(vector<Especialidad>* lstEspecialidades) const{
		if (this->failed())
		{
			cout << "Failed" << endl;
			return;
		}

		vector<Paciente> pacientes;
		int esp_i=0;
		int citEsp_i=0;
		int idEsp=-1;

		while(esp_i < (int) lstEspecialidades->size()) {
			pacientes = lstEspecialidades->at(esp_i).pacientes();
			idEsp = lstEspecialidades->at(esp_i).id();
			for(int i = 0; i < (int) pacientes.size(); i++){
				for(int j = 0; j < pacientes[i].nCitas(idEsp); j++) {
					pacientes[i].insertarCita(Cita(idEsp,
						t_inicio[citEsp_i].val(),
						t_fin[citEsp_i].val(),
						writer->determinarDiaInt(t_inicio[citEsp_i].val())
					));

					if(j == pacientes[i].nCitas(idEsp)-1) {
						Especialista auxEsp = lstEspecialidades->at(esp_i).buscarEspecialista(especialistas[citEsp_i].val());
						auxEsp.insertarPaciente(pacientes[i]);
						lstEspecialidades->at(esp_i).actualizarEspecialista(auxEsp);
					}
					citEsp_i++;
				}
			}
			esp_i++;
		}
		cout << citEsp_i << " == " << t_inicio.size() << endl;

		writer->escribirXml(lstEspecialidades);
		double porcentaje = maximo.val() / (double)nPacPref;
		cout << "Porcentaje de satisfacción: " << porcentaje * 100  << "%" << endl;
		delete writer;
	}

	/// Print solution when the preference constraint is inactive
	virtual void
	print(std::ostream& os) const {
		if (this->failed())
		{
			os << "Failed" << endl;
//			return;
		}

		vector<Especialidad>* lstEspecialidades = listaEspecialidades;
		vector<Paciente> pacientes;
		int esp_i=0;
		int citEsp_i=0;
		int idEsp;

//		int contadorPref=0;
		while(esp_i < (int) lstEspecialidades->size()) {
			pacientes = lstEspecialidades->at(esp_i).pacientes();
			idEsp = lstEspecialidades->at(esp_i).id();
			os << "Especialidad: " << idEsp << " - " << lstEspecialidades->at(esp_i).nombre() << endl;
			for(int i = 0; i < (int) pacientes.size(); i++){
				for(int j = 0; j < pacientes[i].nCitas(idEsp); j++) {
					if (j == 0)
					{
						os << "Paciente: " << pacientes[i].id() << endl;
					}
					os << "\tCita: " << (j+1) << " (" << t_inicio[citEsp_i] << " - "
					   << t_fin[citEsp_i] << ") Especialista: " << especialistas[citEsp_i] << endl;
					citEsp_i++;
				}
			}
			esp_i++;
		}
//		while(esp_i < (int) lstEspecialidades->size()) {
//			pacientes = lstEspecialidades->at(esp_i).pacientes();
//			idEsp = lstEspecialidades->at(esp_i).id();
//			for(int i = 0; i < (int) pacientes.size(); i++){
//				for(int j = 0; j < pacientes[i].nCitas(idEsp); j++) {
//					pacientes[i].insertarCita(Cita(idEsp,
//							t_inicio[citEsp_i].val(),
//							t_fin[citEsp_i].val(),
//							writer->determinarDiaInt(t_inicio[citEsp_i].val())
//					));
//					if(j == pacientes[i].nCitas(idEsp)-1) {
//						Especialista auxEsp = lstEspecialidades->at(esp_i).buscarEspecialista(especialistas[citEsp_i].val());
//						auxEsp.insertarPaciente(pacientes[i]);
//						lstEspecialidades->at(esp_i).actualizarEspecialista(auxEsp);
//						if (pacientes[i].especialistaPref(idEsp) == auxEsp.id())
//						{
//							contadorPref++;
//						}
//					}
//					citEsp_i++;
//				}
//			}
//			esp_i++;
//		}
////		cout << citEsp_i << " == " << t_inicio.size() << endl;
//
//		writer->escribirXml(lstEspecialidades);
//		delete writer;
//		double porcentaje = contadorPref / (double)nPacPref;
//		cout << "Porcentaje de satisfacción: " << porcentaje * 100  << "%" << endl;
	}
}
;
