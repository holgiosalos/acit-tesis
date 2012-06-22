#include <gecode/int.hh>
#include <limits>
#include "especialidad.h"


using namespace Gecode;
using namespace std;


class Distribuidor : public Brancher {
protected:
	ViewArray<Int::IntView> x;
	vector<Especialidad> lstEspecialidades;
	vector<vector<int> > prioridades;
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
		int esp, pr;
		for (int i=0; i<x.size(); i++){
			if (!x[i].assigned()){
				esp = determinarEspecialidad(i);
				//cout << "esp[" << i << "]: " << esp << endl;
				int* codigos = lstEspecialidades[esp].idEspecialistasArray();
				pr = determinarEspecialista(prioridades[esp]);
				//cout << pr << endl;
				prioridades[esp][pr] +=1;
				return new PosVal(*this, i, codigos[pr]);
			}
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

	//Determinar los especialidad
	int determinarEspecialidad(int i) {
		int acumulado=0;
		int nEsp;
		for(int a=0; a<(int)lstEspecialidades.size(); a++){
			if(a>0){
				if( (i >= acumulado) && (i < acumulado+lstEspecialidades[a].totalCitas()) ){
					nEsp = a;
					break;
				}
			}else {
				if(i < lstEspecialidades[a].totalCitas()){
					nEsp = a;
					break;
				}
			}
			acumulado += lstEspecialidades[a].totalCitas();
		}
		return nEsp;
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
};
