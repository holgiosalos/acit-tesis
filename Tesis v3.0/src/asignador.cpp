#include <gecode/int/linear.hh>
#include <gecode/minimodel.hh>

#include <cmath>
#include "AsignadorOptions.h"
#include "archivo.h"
#include "heuristica.cpp"

using namespace std;

class Asignador : public Script {

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
		
		Asignador(const AsignadorOptions& opt)
		: especialistas(*this, opt.totalCitas(), opt.listaCodEspecialistas()),
		  t_inicio(*this, opt.totalCitas(), 0, opt.totalSlots()),
		  t_fin(*this, opt.totalCitas(), 1, opt.totalSlots()) {

			/************** Asignaciones Previas **************/

			listaPacientes = opt.listaPacientes();
			listaEspecialidades = opt.listaEspecialidades();

			// Creacion de los IntArgs que representan las capacidades
			for(int i=0; i<(int)listaEspecialidades.size(); i++){

				//IntArgs auxiliares para las capacidades
				int arrAuxCap[listaEspecialidades[i].nEspecialistas()];
				vector<Especialista> lstEsp = listaEspecialidades[i].especialistas();
				for(int j=0; j<listaEspecialidades[i].nEspecialistas(); j++){
					arrAuxCap[j] = lstEsp[j].capacidad();
				}
				listaCapacidad.push_back(IntArgs(listaEspecialidades[i].nEspecialistas(), arrAuxCap));

				//IntArgs auxiliares para los recursos y las duraciones
				int arrAux[listaEspecialidades[i].totalCitas()];
				for(int j=0; j<listaEspecialidades[i].totalCitas(); j++){
					arrAux[j] = 1;
				}
				listaRecursos.push_back(IntArgs(listaEspecialidades[i].totalCitas(), arrAux));

				int arrAuxD[listaEspecialidades[i].totalCitas()];
				for(int j=0; j<listaEspecialidades[i].totalCitas(); j++){
					arrAuxD[j] = 1;
				}
				listaDuracion.push_back(IntArgs(listaEspecialidades[i].totalCitas(), arrAuxD));

				listaCodigos.push_back(IntSet(listaEspecialidades[i].idEspecialistas(),listaEspecialidades[i].nEspecialistas()));
			}

			for(int i=0; i<(int)listaEspecialidades.size(); i++){
				listaVarEspecialistas.push_back(IntVarArray(*this,
										listaEspecialidades[i].totalCitas(),
										listaCodigos[i]));//especialistas
				listaVarTInicio.push_back(IntVarArray(*this,listaEspecialidades[i].totalCitas(),0,opt.totalSlots()));//tiempos de inicio
				listaVarTFin.push_back(IntVarArray(*this,listaEspecialidades[i].totalCitas(),1,opt.totalSlots()));//tiempos de fin
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
			vector<int> dispPi; //Vector auxiliar que guarda las disponibilidades de los pacientes
			int citEsp_i=0; //Contador que me servira para recorrer las variables de tiempos de inicio de la especialidad i
			int esp_i=0;

			while(esp_i < (int) listaEspecialidades.size()){
				pacEsp_i = listaEspecialidades[esp_i].pacientes();
				for(int i = 0; i < listaEspecialidades[esp_i].nPacientes(); i++){
					dispPi = pacEsp_i[i].disponibilidad();
					for(int j = 0; j < pacEsp_i[i].nCitas(); j++){
						//Restricción 1: Recortar disponibilidades de pacientes (preasignacion de tiempo)
						for (int k = 0; k < (int) dispPi.size(); k++) {
							if (dispPi[k] == 0) {
								rel(*this, listaVarTInicio[esp_i][citEsp_i], IRT_NQ, k, opt.icl());
							}
						}

						if(j<pacEsp_i[i].nCitas()-1){
							//Restricción 2: Mismo especialista para todas las citas de un paciente
							rel(*this, listaVarEspecialistas[esp_i][citEsp_i], IRT_EQ, listaVarEspecialistas[esp_i][citEsp_i+1], opt.icl());

							//Restricción 3: Dia diferente
							//Asignacion previa para restriccion 3
							rel(*this, listaSlotsDia[esp_i][citEsp_i], IRT_EQ, opt.slotsDia(), opt.icl());
							rel(*this, listaSlotsDia[esp_i][citEsp_i+1], IRT_EQ, opt.slotsDia(), opt.icl());
							//Division de a/(num intervalos dia)=x y a+1/(num intervalos dia)=y para despues asegurarse que x<y
							div(*this,  listaVarTInicio[esp_i][citEsp_i], listaSlotsDia[esp_i][citEsp_i], listaResultDia[esp_i][citEsp_i], opt.icl());
							div(*this,  listaVarTInicio[esp_i][citEsp_i+1], listaSlotsDia[esp_i][citEsp_i+1], listaResultDia[esp_i][citEsp_i+1], opt.icl());
							rel(*this, listaResultDia[esp_i][citEsp_i], IRT_LE, listaResultDia[esp_i][citEsp_i+1], opt.icl());

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

			Gecode::wait(*this, especialistas, &recDispEsp, opt.icl());

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
		Asignador(bool share, Asignador& s) : Script(share, s) {
			especialistas.update(*this, share, s.especialistas);
			t_inicio.update(*this, share, s.t_inicio);
			t_fin.update(*this, share, s.t_fin);
		}

		/// Perform copying during cloning
		virtual Space* copy(bool share) {
			return new Asignador(share,*this);
		}

		/// Print solution
		virtual void
		print(std::ostream& os) const{
			for(int i=0; i<(int)listaEspecialidades.size(); i++){
				Especialidad aux = listaEspecialidades[i];
				os << "\nEspecialidad: " << aux.id() << "-> ";
				for(int j=0; j<aux.nEspecialistas(); j++){
					vector<Especialista> aux2 = aux.especialistas();
					if(aux2[j].idEspecialidad() == (i+1)){
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
					os << "\nPaciente: " << pacientes[i].id() << "\tCitas: " << pacientes[i].nCitas() << "\tEspecialidad: " << pacientes[i].idEspecialidad() << endl;
					for(int j = 0; j < pacientes[i].nCitas(); j++) {
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

		static void recDispEsp(Space& _home){
			Asignador& home = static_cast<Asignador&>(_home);
			vector<Especialista> profEsp_i; //Vector auxiliar que contendra los profesionales pertenecientes a la especialidad i
			vector<int> dispEi; //Vector auxiliar que guarda las disponibilidades de los profesionales

			int esp_i=0;
			while (esp_i < (int)home.listaEspecialidades.size()) {
				profEsp_i = home.listaEspecialidades[esp_i].especialistas();
				for(int i=0; i<(int)profEsp_i.size();i++){
					dispEi = profEsp_i[i].disponibilidad();
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

int main(int argc, char* argv[]) {

	Archivo lector;
	int i=0;
	if(argc>1){
		while (++i < argc){
			if (strcmp(argv[i],"-file")==0){
				lector.nombreArchivo(argv[++i]);
				lector.saveLines();
			}else{
				lector.nombreArchivo("test1.txt");
				lector.saveLines();
			}
		}
	}else{
		lector.nombreArchivo("test1.txt");
		lector.saveLines();
	}


    AsignadorOptions opt("Asignador");
    opt.reader(lector);
    opt.iniciar();
    opt.totalCitas(lector.totCitas());
    opt.totalEspecialistas(lector.numEspecialistas());
    opt.totalSlots(lector.numIntervalos());
    opt.slotsDia(lector.numIntervalosDia());
    Gecode::IntSet codigos(opt.settingCodigos(), lector.numEspecialistas());
    opt.listaCodEspecialistas(codigos);
    opt.icl(ICL_DOM);	// Dominio consistencia

    Script::run<Asignador,DFS,AsignadorOptions>(opt);
    
    return 0;
}
