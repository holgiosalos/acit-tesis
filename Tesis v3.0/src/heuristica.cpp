#include <gecode/int.hh>
#include <limits>
#include <map>
#include "paciente.h"
#include "especialidad.h"

enum EnfoquesHeuristica { PACIENTES, CITAS };

using namespace Gecode;
using namespace std;

class Distribuidor : public Brancher {
protected:
	ViewArray<Int::IntView> x;
	vector<Especialidad> lstEspecialidades;
	vector<map<int, int> > contador;
	int proxima;
	int enfoqueHeuristica;
	//Choice Definition
	class PosVal : public Choice {
		public:
			int pos; int val;
			PosVal(const Distribuidor& b, int p, int v)
				: Choice(b,2), pos(p), val(v) {}
			virtual size_t size(void) const {
				return sizeof(*this);
			}
			virtual void archive(Archive& e) const {
				Choice::archive(e);
				e << pos << val;
			}
	};
public:
	Distribuidor(Home home, ViewArray<Int::IntView>& x0, vector<Especialidad> lstEsp, int enfoque) //vector<map<int, int> > nP
		: Brancher(home), x(x0) {
		lstEspecialidades = lstEsp;
		enfoqueHeuristica = enfoque;
		contador.resize(lstEspecialidades.size());
		for (int i = 0; i < (int)contador.size(); i ++) {
			vector<int> ids = lstEspecialidades[i].idEspecialistasVector();
			for (int j = 0; j < (int)ids.size(); j++) {
				contador[i][ids[j]] = 0;
			}
		}
		proxima=0; //proxima variable a asignar
	}

	static void post(Home home, ViewArray<Int::IntView>& x, vector<Especialidad> lstEsp, int enfoque){
		(void) new (home) Distribuidor(home, x, lstEsp, enfoque);
	}

	virtual size_t dispose(Space& home) {
		(void) Brancher::dispose(home);
		return sizeof(*this);
	}

	Distribuidor(Space& home, bool share, Distribuidor& b)
		: Brancher(home,share,b) {
		x.update(home,share,b.x);
		lstEspecialidades = b.lstEspecialidades;
		contador = b.contador;
		proxima = b.proxima;
		enfoqueHeuristica = b.enfoqueHeuristica;
	}

	virtual Brancher* copy(Space& home, bool share) {
		return new (home) Distribuidor(home,share,*this);
	}

	//Status
	virtual bool status(const Space& home) const {
		for (int i=0; i<x.size(); i++)
			if (!x[i].assigned())
				return true;
		return false;
	}

	//Choice
	virtual Choice* choice(Space& home) {
		vector<int> infoVar(2);
		int id;
		vector<int> codigos;
		for (int i=0; i<x.size(); i++){
			infoVar = infoVariable(i);
			if (!x[i].assigned())
			{
				proxima = i + infoVar[1];
				codigos = lstEspecialidades[infoVar[0]].idEspecialistasVector();
				id = determinarEspecialista(contador[infoVar[0]], codigos);
				if (enfoqueHeuristica == PACIENTES) {
					contador[infoVar[0]][id] += 1;
				}
				else {
					contador[infoVar[0]][id] += infoVar[1];
				}
				return new PosVal(*this, i, id);
			}
			infoVar.clear();
			infoVar.resize(2);
		}
		GECODE_NEVER;
		return NULL;
	}

	virtual Choice* choice(const Space&, Archive& e) {
		int pos, val;
		e >> pos >> val;
		return new PosVal(*this, pos, val);
	}

	//Commit
	virtual ExecStatus commit(Space& home, const Choice& c, unsigned int a) {
		const PosVal& pv = static_cast<const PosVal&>(c);
		int pos=pv.pos, val=pv.val;
	    if (a == 0)
	    {
	    	return me_failed(x[pos].eq(home,val)) ? ES_FAILED : ES_OK;
	    }
	    else
	    {
	    	return me_failed(x[pos].nq(home,val)) ? ES_FAILED : ES_OK;
	    }
	}

	//Busca que especialistas ya poseen pacientes/citas asignadas
	void actualizarNumPacientes() {
		vector<int> infoVar(2);
		vector<int> codigos;
		for (int i=0; i<x.size(); i++){
			if ((x[i].assigned()) && (i >= proxima)) {
				infoVar = infoVariable(i);
				proxima = i + infoVar[1];
				if (enfoqueHeuristica == PACIENTES) {
					contador[infoVar[0]][x[i].val()] += 1;
				}
				else {
					contador[infoVar[0]][x[i].val()] += infoVar[1];
				}
			}
			infoVar.clear();
			infoVar.resize(2);
		}
	}

	//Determinar la especialidad y las citas faltantes para cambio de paciente
	vector<int> infoVariable(int i) {
		vector<int> _infoVariable(2);
		int acumulado = 0;
		for (int nEsp = 0; nEsp < (int)lstEspecialidades.size(); nEsp++){
			if (nEsp > 0){
				if( (i >= acumulado) && (i < acumulado+lstEspecialidades[nEsp].totalCitas()) ){
					_infoVariable[0] = nEsp;
					_infoVariable[1] = determinarCitasPaciente(i, nEsp, acumulado);
					break;
				}
			}else {
				if(i < lstEspecialidades[nEsp].totalCitas()){
					_infoVariable[0] = nEsp;
					_infoVariable[1] = determinarCitasPaciente(i, nEsp, acumulado);
					break;
				}
			}
			acumulado += lstEspecialidades[nEsp].totalCitas();
		}
		return _infoVariable;
	}

	//Determinar la prioridad de asignación para un especialista
	int determinarEspecialista(map<int, int> numPac, vector<int> ids) {
		int idProf = 0;
		int menor = numeric_limits<int>::max();
		for(int j=0; j<(int)numPac.size(); j++){
			if(numPac[ids[j]] < menor){
				menor = numPac[ids[j]];
				idProf = ids[j];
			}
		}
		return idProf;
	}

	int determinarCitasPaciente(int i, int esp, int acumulado){
		vector<Paciente> pacientes = lstEspecialidades[esp].pacientes();
		int idEsp = lstEspecialidades[esp].id();
		int citasP=0;
		int posIni = i - acumulado; //indica la posicion de la primera cita de esa especialidad
		int acumuladoCitas = 0;
		for (int b = 0; b < (int)pacientes.size(); b++){
			if (b > 0){
				if ((posIni >= acumuladoCitas) && (posIni < acumuladoCitas+pacientes[b].nCitas(idEsp)))
				{
					citasP = pacientes[b].nCitas(idEsp);
					break;
				}
			}
			else if (posIni < pacientes[b].nCitas(idEsp))
			{
					citasP = pacientes[b].nCitas(idEsp);
					break;
			}
			acumuladoCitas += pacientes[b].nCitas(idEsp);
		}
		return citasP;
	}

	int determinarPosicion(int esp, int idE, int* cods){
		int pos = -1;
		for(int i=0; i<lstEspecialidades[esp].nEspecialistas(); i++){
			if(cods[i] == idE){
				pos = i;
				break;
			}
		}
		return pos;
	}
};
