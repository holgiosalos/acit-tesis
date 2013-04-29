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

	/**
	 * Constructor. Aqui se definene todas las restricciones.
	 * Se encarga de inicializar las variables del problema: especialistas, t_inicio y t_fin. Y las
	 * variables de optimizacion: preferencias y maximo.
	 */
	ACiTConstraints(const ACiTOptions& opt)
	: especialistas(*this, opt.totalCitas(), opt.listaCodEspecialistas()),
	  t_inicio(*this, opt.totalCitas(), 0, opt.makespan()),
	  t_fin(*this, opt.totalCitas(), 1, opt.makespan()),
	  preferencias(*this, opt.nPacientesPreferencia(), 0, 1),
	  maximo(*this, 0, opt.nPacientesPreferencia()) {

		vector<IntVarArray> listaVarEspecialistas;
		vector<IntVarArray> listaVarTInicio;
		vector<IntVarArray> listaVarTFin;
		//Se utiliza para restriccion dia diferente entre dos citas distintas y dia igual para el tiempo de inicio y el tiempo de fi de una cita
		vector<IntVarArray> listaResultDia;
		//Se utiliza unicamente para la restricción de dia igual para el tiempo de inicio y el tiempo de fi de una cita
		vector<IntVarArray> listaResultDia2;

		vector<IntVarArray> listaSlotsDia;
		vector<IntArgs> listaDuracion;
		vector<IntArgs> listaRecursos;
		vector<IntArgs> listaCapacidad;
		vector<IntSet> listaCodigos;

		/************** Asignaciones Previas **************/
		listaPacientes = opt.listaPacientes();
		listaEspecialidades = opt.listaEspecialidades();

		/*
		 * Inicializamos el objeto de escritura con el directorio donde quedaran consignados los
		 * archivos de salida.
		 */
		writer = new Escritura("/var/www/ACiT/sites/default/files/acit_files/output_files_acit");
		writer->semanas(opt.semanas());
		writer->slotsIntervalo(opt.slotsIntervalo());
		writer->slotsDia(opt.slotsDia());
		writer->intervalosDia(opt.intervalosDia());
		writer->intervalosSemana(opt.intervalosSemana());

		/*
		 * Variables necesarias para realizar los calculos que determinan el dia y la hora a la que
		 * pertenece un slot determinado de tiempo.
		 */
		intDia = opt.intervalosDia();
		intSem = opt.intervalosSemana();
		num_semanas = opt.semanas();
		slDia = opt.slotsDia();
		slInt = opt.slotsIntervalo();
		nPacPref = opt.nPacientesPreferencia();
		makespan = opt.makespan();

		// Inicializacion de algunas variables auxiliares utilizadas en las restricciones.
		for(int i=0; i<(int)listaEspecialidades->size(); i++){
			//IntArgs que representan las capacidades de los profesionales en cada especialidad
//			int arrAuxCap[listaEspecialidades->at(i).nEspecialistas()];
			int arrAuxCap[listaEspecialidades->at(i).totalCitas()];
//			for(int j = 0; j < listaEspecialidades->at(i).nEspecialistas(); j++){
			for(int j = 0; j < listaEspecialidades->at(i).totalCitas(); j++){
				arrAuxCap[j] = listaEspecialidades->at(i).capacidad();
			}
//			listaCapacidad.push_back(IntArgs(listaEspecialidades->at(i).nEspecialistas(), arrAuxCap));
			listaCapacidad.push_back(IntArgs(listaEspecialidades->at(i).totalCitas(), arrAuxCap));

			/*
			 * IntArgs para los recursos que puede consumir un paciene a la vez. Los recursos
			 * pueden traducirse como el numero de puestos simultaneos que un paciente puede ocupar
			 * para un mismo especialista en una misma cita.
			 */
			int arrAux[listaEspecialidades->at(i).totalCitas()];
			for(int j = 0; j < listaEspecialidades->at(i).totalCitas(); j++){
				arrAux[j] = 1;
			}
			listaRecursos.push_back(IntArgs(listaEspecialidades->at(i).totalCitas(), arrAux));

			/*
			 * IntArgs que almacena las duraciones de las citas (en numero de slots) para cada cita
			 * a asignar, en todas las especialidades
			 */
			int arrAuxD[listaEspecialidades->at(i).totalCitas()];
			for(int j = 0; j < listaEspecialidades->at(i).totalCitas(); j++){
				arrAuxD[j] = listaEspecialidades->at(i).duracionCitasSlots();
			}
			listaDuracion.push_back(IntArgs(listaEspecialidades->at(i).totalCitas(), arrAuxD));

			// Lista de los codigos de los especialistas pertenecientes a una especialidad determinada
			listaCodigos.push_back(IntSet(listaEspecialidades->at(i).idEspecialistasArray(),listaEspecialidades->at(i).nEspecialistas()));
		}

		/*
		 * listaVarEspecialistas, listaVarTInicio y listaVarTFin almacenaran el especialista
		 * asignado, el tiempo de inicio y el tiempo de finalizacion de una cita determinada,
		 * en cierta especialidad.
		 */
		for(int i=0; i<(int)listaEspecialidades->size(); i++){
			listaVarEspecialistas.push_back(
				IntVarArray(*this, listaEspecialidades->at(i).totalCitas(), listaCodigos[i]));

			listaVarTInicio.push_back(
				IntVarArray(*this,listaEspecialidades->at(i).totalCitas(),0,opt.makespan()));

			listaVarTFin.push_back(
				IntVarArray(*this,listaEspecialidades->at(i).totalCitas(),1,opt.makespan()));

			// Almacenará el numero de slots por día que hay para cada especialidad
			listaSlotsDia.push_back(
				IntVarArray(*this, listaEspecialidades->at(i).totalCitas(), 0, opt.slotsDia()));

			// Almacenará el dia al que pertenece cada cita
			listaResultDia.push_back(
				IntVarArray(*this, listaEspecialidades->at(i).totalCitas(), 0, opt.slotsDia()));

			// Almacenará el dia al que pertenece cada cita
			listaResultDia2.push_back(
				IntVarArray(*this, listaEspecialidades->at(i).totalCitas(), 0, opt.slotsDia()));
		}

		/*
		 * Asignamos el numero de slots por dia disponibles para cada cita, en el array de variables
		 * diseñado para este propósito.
		 */
		for (int esp = 0; esp < (int)listaEspecialidades->size(); esp++)
		{
			rel(*this, listaSlotsDia[esp], IRT_EQ, opt.slotsDia(), opt.icl());
		}

		/*
		 * Las siguientes restricciones establecen la correspondencia que existe entre el conjunto
		 * de variables especialistas, t_inicio, t_fin con las variables listaVarEspecialistas,
		 * listaVarTInicio y listaVarTFin.
		 */
		int tam = 0;
		for(int i=0; i<(int)listaEspecialidades->size(); i++){
			for(int j=0; j<listaVarEspecialistas[i].size(); j++){
				rel(*this, listaVarEspecialistas[i][j], IRT_EQ, especialistas[tam], opt.icl());
				rel(*this, listaVarTInicio[i][j], IRT_EQ, t_inicio[tam], opt.icl());
				rel(*this, listaVarTFin[i][j], IRT_EQ, t_fin[tam], opt.icl());
				tam++;
			}
		}

		/**************************************  Restricciones *************************************/
		vector<Paciente> pacEsp_i; //Vector auxiliar que contendra los pacientes pertenecientes a la especialidad i
		IntArgs dispPi; //Vector auxiliar que guarda las disponibilidades (dadas en slots) de los pacientes
		IntSet setDom; //IntSet que almacena el conjunto de horarios en los cuales esta disponible el paciente

		//Inicializacion de iteradores
		int citEsp_i=0; //iterador para las citas.
		int esp_i=0; //iterador de las especialidades.
		int itPref=0; //iterador para los pacientes que tiene algun profesional de preferencia.
		while(esp_i < (int) listaEspecialidades->size()){
			pacEsp_i = listaEspecialidades->at(esp_i).pacientes();
			for(int i = 0; i < listaEspecialidades->at(esp_i).nPacientes(); i++){
				//Se obtienen los slots en los cuales el paciente esta disponible.
				dispPi = transformarDisponibilidadEsp(pacEsp_i[i].disponibilidad());
				for(int j = 0; j < pacEsp_i[i].nCitas(listaEspecialidades->at(esp_i).id()); j++){
					//Restriccion Pre-asignacion de Tiempo segun Pacientes
					setDom = IntSet(dispPi);
					dom(*this, listaVarTInicio[esp_i][citEsp_i], setDom, ICL_DOM);

					//Restriccion de preferencia
					if(j == 0 && opt.preferencia()){
						if(pacEsp_i[i].especialistaPref(listaEspecialidades->at(esp_i).id()) != 0) {
							/*
							 * Se hace por medio de una restriccion reificada, si se cumple la restriccion se almacena el resultado
							 * en una posicion del IntVarArray preferencias. Este arreglo es utilizado por la funcion de optimizacion.
							 */
							rel(
								*this,
								(listaVarEspecialistas[esp_i][citEsp_i] == pacEsp_i[i].especialistaPref(listaEspecialidades->at(esp_i).id()))
									== preferencias[itPref],
								opt.icl());
							itPref++;
						}
					}

					if(j < pacEsp_i[i].nCitas(listaEspecialidades->at(esp_i).id())-1){
						/*
						 * Restriccion que me asegura que un paciente sea atendido por un mismo profesional
						 * en todas las citas de un mismo tratamiento
						 */
						rel(*this, listaVarEspecialistas[esp_i][citEsp_i], IRT_EQ, listaVarEspecialistas[esp_i][citEsp_i+1], opt.icl());

						/*
						 * Restriccion Dia differente: Primero se calcula el dia al que pertenece la cita i,
						 * por medio de la division del slot al que va a ser asignado la cita por el numero de
						 * slots por dia, el resultado de esta division es almacenado en la posicion i del vector
						 * listaResultDia. Se hace el mismo proceso para la cita i+1 y se almacena en la posicion
						 * i+1 del mismo vector,
						 */
						div(*this,  listaVarTFin[esp_i][citEsp_i], listaSlotsDia[esp_i][citEsp_i], listaResultDia[esp_i][citEsp_i], opt.icl());
						div(*this,  listaVarTInicio[esp_i][citEsp_i+1], listaSlotsDia[esp_i][citEsp_i+1], listaResultDia[esp_i][citEsp_i+1], opt.icl());
						/*
						 * Luego se establece que el valor almacenado en la posicion i debe ser menor que el
						 * valor almacenado en i+1. Lo que asegura que las citas sean asignadas en dias
						 * distintos.
						 */
						rel(*this, listaResultDia[esp_i][citEsp_i], IRT_LE, listaResultDia[esp_i][citEsp_i+1], opt.icl());

						/*
						 * Los siguientes propagadores tienen un funcionamiento similar al de los propagadores de
						 * la restriccion Día diferente. Pero con la salvedad, que estan asegurando que tanto el tiempo
						 * de incio de la cita i como el tiempo de fin, sean asignados el mismo dia.
						 */
						div(*this,  listaVarTInicio[esp_i][citEsp_i], listaSlotsDia[esp_i][citEsp_i], listaResultDia2[esp_i][citEsp_i], opt.icl());
						rel(*this, listaResultDia[esp_i][citEsp_i], IRT_EQ, listaResultDia2[esp_i][citEsp_i], opt.icl());
						if(j == pacEsp_i[i].nCitas(listaEspecialidades->at(esp_i).id())-2) {
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

		//Restriccion No Solapamiento entre Citas
		vector<vector<int> > infoNS;
		for(int i = 0; i < (int)listaPacientes.size(); i++){
			if(listaPacientes[i].nTratamientos()>1){
				/*
				 * infoNS almacena la siguiente informacion por cada especialidad:
				 *  - Posicion 0: Id de la especialidad en la que el paciente necesita el tratamiento
				 *  - Posicion 1: Posicion de la especialidad en el vector listaEspecialidades
				 *  - Posicion 2: Posicion de inicio de las citas del paciente para esa especialidad
				 *  - Posicion 3: Posicion de fin de las citas del paciente para esa especialidad
				 */
				infoNS = infoNoSolapamiento(listaPacientes[i]);
				if( (int)infoNS.size() == listaPacientes[i].nTratamientos() ) {
					/*
					 * Por cada especialidad en la que el paciente tiene cita, se asegura que:
					 * Cada uno de los slots en los cuales la cita i (nEsp) es asignada, no se asigne
					 * otra cita en cualquiera de las demas especialidades del paciente.
					 */
					int nEsp = 0, cit = infoNS[nEsp][2], sigEsp = 1;
					while(nEsp < int(infoNS.size())- 1){
						for(int j=infoNS[sigEsp][2]; j< infoNS[sigEsp][3]; j++){
							for(int k=0; k<listaPacientes[i].duracionCitTrat(infoNS[nEsp][0]); k++){
								rel(*this, listaVarTInicio[infoNS[sigEsp][1]][j] != listaVarTInicio[infoNS[nEsp][1]][cit] + k, opt.icl());
							}
						}
						cit++;

						/*
						 * cuando se llega al final de las citas de la especialidad i (nEsp) se cambia la especialidad
						 * i+1(sigEsp) a la siguiente (sigEsp++). Si el valor de sigEsp supera el numero de especialidades
						 * en las que tiene que ser atendido el paciente, se pasa a comparar la especialidad i+1 (nEsp++)
						 * con las subsiguientes especialidades.
						 */
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

		//Restriccion Tiempo de Finalizacion
		for(int i=0; i<(int)listaEspecialidades->size(); i++){
			for(int j=0; j<listaVarTInicio[i].size(); j++){
				rel(*this, listaVarTFin[i][j] == listaDuracion[i][j]+listaVarTInicio[i][j], opt.icl());
			}
		}

		/*
		 * La restriccion de capacidad de los especialistas es propagada a traves de cumulatives, que
		 * nos asegura que no se atienda a mas de la cantidad permitida de pacientes en un mismo
		 * slot de tiempo para una especialidad determinada.
		 */
		for (int i=0; i<(int)listaEspecialidades->size(); i++){
			cumulatives(
				*this,
				listaVarEspecialistas[i],
				listaVarTInicio[i],
				listaDuracion[i],
				listaVarTFin[i],
				listaRecursos[i],
				listaCapacidad[i],
				true,
				ICL_VAL);
		}

		/*
		 * El propagador wait, hace que todas las variables en el arreglo especialistas esten asignadas
		 * para poder ejectuar la restriccion de Pre-asignacion de Tiempo segun Profesionales.
		 */
		Gecode::wait(*this, especialistas, &recDispEsp, opt.icl());

		/*
		 * Funcion de optimizacion: Se encarga de maximizar el numero de pacientes a los cuales se les
		 * asigna a su profesional de preferencia.
		 */
		if (opt.preferencia())
		{
			rel(*this, maximo == sum(preferencias), opt.icl());
		}

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

	/**
	 * Esta funcion es activada cuando todos los profesionales han sido asignados a una cita determinada.
	 * Su objetivo es asegurar que las citas van a ser asignadas de acuerdo al horario de atencion que tienen
	 * los profesionales de determinada especialidad.
	 */
	static void recDispEsp(Space& _home){
		ACiTConstraints& home = static_cast<ACiTConstraints&>(_home);
		IntArgs dispEi; // Vector auxiliar que guarda los horarios de atencion (dadas en slots) del profesional
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

	/**
	 * Esta funcion se encarga de transformar las disponibilidades dadas en intervalos de tiempo, a
	 * disponibilidades dadas en slots de tiempo. Recordemos que un intervalo esta compuesto por varios
	 * slots de tiempo.
	 */
	IntArgs transformarDisponibilidadEsp(vector<int> disp){
		int diaSem = int(floor(intSem/intDia)); // dias por semana
		int slSem =  slDia * diaSem;
		vector<int> aux(makespan);

		int pos, sem=0;
		while(sem < num_semanas){
			pos = 0;
			for(int i = (slSem * sem); i < (slSem * (sem+1)); i++){
				int dia_slot = int(floor(i / slDia)) % diaSem;
				int num_slot = int(i % slDia);
				pos = (num_slot / intDia) + (dia_slot * intDia);

				if(num_slot == (slDia - 1))
				{
					pos--;
				}

				if (pos >= (int)disp.size())
				{
					aux[i] = 0;
				}
				else
				{
					aux[i] = disp[pos];
				}
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
		listaEspecialidades = s.listaEspecialidades;
		listaPacientes = s.listaPacientes;

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
			return;
		}

		vector<Especialidad>* lstEspecialidades = listaEspecialidades;
		vector<Paciente> pacientes;
		int esp_i=0;
		int citEsp_i=0;
		int idEsp;
		int contadorPref=0;

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
						if (pacientes[i].especialistaPref(idEsp) == auxEsp.id())
						{
							contadorPref++;
						}
					}
					citEsp_i++;
				}
			}
			esp_i++;
		}

		writer->escribirXml(lstEspecialidades);
		delete writer;
		double porcentaje = contadorPref / (double)nPacPref;
		cout << "Porcentaje de satisfacción: " << porcentaje * 100  << "%" << endl;
	}
}
;
