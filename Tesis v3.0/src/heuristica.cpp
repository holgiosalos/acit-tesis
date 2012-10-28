#include <gecode/int.hh>
#include <limits>
#include "paciente.h"
#include "especialidad.h"

using namespace Gecode;
using namespace std;

class Distribuidor : public Brancher {
protected:
	ViewArray<Int::IntView> x;
	vector<Especialidad> lstEspecialidades;
	vector<vector<int> > prioridades;
	int proxima;
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
	Distribuidor(Home home, ViewArray<Int::IntView>& x0, vector<Especialidad> lstEsp, vector<vector<int> > p)
		: Brancher(home), x(x0) {
		lstEspecialidades = lstEsp;
		prioridades = p;
		proxima=0; //proximo variable a asignar
	}

	static void post(Home home, ViewArray<Int::IntView>& x, vector<Especialidad> lstEsp, vector<vector<int> > p){
		(void) new (home) Distribuidor(home, x, lstEsp, p);
	}

	virtual size_t dispose(Space& home) {
		(void) Brancher::dispose(home);
		return sizeof(*this);
	}

	Distribuidor(Space& home, bool share, Distribuidor& b)
		: Brancher(home,share,b) {
		x.update(home,share,b.x);
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
		int pos;
		for (int i=0; i<x.size(); i++){
			infoVar = infoVariable(i);
			if (!x[i].assigned())
			{
				//ultimo + citas
				proxima = i + infoVar[1];
				//cout << "i: " << i << " -> " << proxima << endl;
//				cout << "esp[" << i << "]: " << esp << endl;
				int* codigos = lstEspecialidades[infoVar[0]].idEspecialistasArray();
				pos = determinarEspecialista(prioridades[infoVar[0]]);
//				cout << pr << endl;
				prioridades[infoVar[0]][pos] +=1;
				cout << "prioridades[" << infoVar[0] << "][" << pos << "]: " << prioridades[infoVar[0]][pos] << endl;
				cout << "seleccionado codigos[" << pos << "]: " << codigos[pos] << endl;
				if (i == x.size()-2)
				{
					cout << "valores finales: " << endl;
					for (int j=0; j<(int)prioridades.size(); j++)
					{
						for (int k=0; k<(int)prioridades[j].size(); k++)
						{
							cout << "prioridades[" << j << "][" << k << "]: " << prioridades[j][k] << endl;
						}
					}
				}
				return new PosVal(*this, i, codigos[pos]);
			}
			else
			{
				//cout << "i': " << i << " -> " << proxima << endl;
/*
				if(i >= proxima) {
					// sumar paciente al especialista que corresponde
					int* codigos = lstEspecialidades[infoVar[0]].idEspecialistasArray();
					pos = determinarEspecialista(prioridades[infoVar[0]]);
					prioridades[infoVar[0]][pos] +=1 ;
					//actualizar proxima
					proxima = i + infoVar[1];
					cout << "+ prioridades[" << infoVar[0] << "][" << pos << "]: " << prioridades[infoVar[0]][pos] << endl;
					cout << "sumado codigos[" << pos << "]: " << codigos[pos] << endl;
				}
*/
				if (i == x.size()-2)
				{
					cout << "valores finales: " << endl;
					for (int j=0; j<(int)prioridades.size(); j++)
					{
						for (int k=0; k<(int)prioridades[j].size(); k++)
						{
							cout << "prioridades[" << j << "][" << k << "]: " << prioridades[j][k] << endl;
						}
					}
				}
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
	    	return me_failed(x[pos].eq(home,val)) ? ES_FAILED : ES_OK;
	    else
	    	return me_failed(x[pos].nq(home,val)) ? ES_FAILED : ES_OK;
	}

	//Determinar la especialidad y las citas faltantes para cambio de paciente
	vector<int> infoVariable(int i) {
		vector<int> _infoVariable(2);
		int acumulado = 0;
		for (int nEsp = 0; nEsp < (int)lstEspecialidades.size(); nEsp++){
			if (nEsp > 0){
				if( (i >= acumulado) && (i < acumulado+lstEspecialidades[nEsp].totalCitas()) ){
					_infoVariable[0] = nEsp;
					_infoVariable[1] = determinarPaciente(i, nEsp, acumulado);
					break;
				}
			}else {
				if(i < lstEspecialidades[nEsp].totalCitas()){
					_infoVariable[0] = nEsp;
					_infoVariable[1] = determinarPaciente(i, nEsp, acumulado);
					break;
				}
			}
			acumulado += lstEspecialidades[nEsp].totalCitas();
		}
		return _infoVariable;
	}

	//Determinar la prioridad de asignación para un especialista
	int determinarEspecialista(vector<int> pEsp) {
		int posEsp = 0;
		int menor = numeric_limits<int>::max();
		for(int j=0; j<(int)pEsp.size(); j++){
			if(pEsp[j] < menor){
				menor = pEsp[j];
				posEsp = j;
			}
		}
		return posEsp;
	}

	int determinarPaciente(int i, int esp, int acumulado){
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
		int pos;
		for(int i=0; i<lstEspecialidades[esp].nEspecialistas(); i++){
			if(cods[i] == idE){
				pos = i;
				break;
			}
		}
		return pos;
	}
};
