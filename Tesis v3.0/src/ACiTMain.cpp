#include <gecode/search.hh>
#include <ctime>
#include <string>

#include "ACiTConstraints.cpp"

using namespace std;

long long cronousec(int startstop)
{
  static long long pre_time;
  struct timeval tv;

  if (startstop)
    {
      gettimeofday(&tv, NULL);
      pre_time = tv.tv_sec*1000000+tv.tv_usec;
    }
  else
    {
      gettimeofday(&tv, NULL);
      return (tv.tv_sec*1000000+tv.tv_usec - pre_time);
    }
    return (0);
}

// Funcion que convierte de microsegundos a mili segundos (si opc=true) o a segundos (si opc=false)
double convertir(unsigned long long microsecs, bool opc)
{
	double result;
	if (opc)
	{
		result = (double)(microsecs/1000.0);
	} else {
		result = (double)(microsecs/1000000.0);
	}
	return (result);
}

int main(int argc, char* argv[]) {
    ACiTOptions opt("ACiT");
    opt.solutions(0);
    //Establecimiento de los valores por defecto
//    opt.file("test4.txt");
    opt.file("test_files/dist1_10pac.txt");
    opt.c_d(10);
    opt.a_d(20);
    opt.preferencia(true);

    opt.slotsIntervalo(12); //12 slots por cada intervalo de tiempo, es decir 1 slot equivale a 5 minutos si el intervalo equivale a una hora
    opt.intervalosDia(12); //12 intervalos de tiempo para cada dia
    opt.semanas(4); //1 semana para lograr todas las asignaciones de citas
    opt.intervalosSemana(65); //65 intervalos de tiempo por toda la semana (Lunes a Sabado) el sabado solo tendra 5 intervalos

    opt.icl(ICL_BND);

    // Opciones de branching
    opt.branching(ACiTConstraints::BRANCH_CITAS);
    opt.branching(ACiTConstraints::BRANCH_DEFAULT,"default","= sel-var: dom-wdeg, sel-val: random");
    opt.branching(ACiTConstraints::BRANCH_PACIENTES, "pacientes", "= sel-val: min-num-pacientes");
    opt.branching(ACiTConstraints::BRANCH_CITAS, "citas", "= sel-val: min-num-citas");

    opt.parse(argc,argv);

    Lectura lector(opt.file());
    opt.reader(lector);
    opt.totalCitas(lector.totCitas());
    opt.totalEspecialistas(lector.numEspecialistas());

    opt.iniciar();

    Gecode::IntSet codigos(opt.settingCodigos(), lector.numEspecialistas());
    opt.listaCodEspecialistas(codigos);

    if (!opt.preferencia())
    {
    	opt.solutions(1);
    	Script::run<ACiTConstraints,BAB,ACiTOptions>(opt);
    }
    else
    {
    	cronousec(1);
    	ACiTConstraints* acit = new ACiTConstraints(opt);
    	int np = acit->propagators();
    	int nb = acit->branchers();
    	BAB<ACiTConstraints> search(acit);
    	ACiTConstraints* ultimaSolucion;

    	int contador = 0;
    	while (ACiTConstraints* s = search.next()) {
    		ultimaSolucion = s;
    		contador++;
    	}
    	unsigned long long tiempo = cronousec(0);

    	// Prints the statistics
    	cout << opt.name() << endl;
    	ultimaSolucion->print(opt.listaEspecialidades());
    	Search::Statistics stat = search.statistics();

    	double memoriaKB = (stat.memory/1024);
    	int memoriaMB = (memoriaKB/1024);

    	cout << "Initial:" << endl;
    	cout << "\tpropagators: " << np << endl;
    	cout << "\tbranchers:   " << nb << endl;
    	cout << endl;
    	cout << "Summary:" << endl;
    	cout << "\truntime:      " << convertir(tiempo, false) << " s (" << convertir(tiempo, true) << " ms)" << endl;
    	cout << "\tsoulutions:   " << contador << endl;
    	cout << "\tpropagations: " << stat.propagate << endl;
    	cout << "\tnodes:        " << stat.node << endl;
    	cout << "\tfailures:     " << stat.fail << endl;
    	cout << "\tpeak depth:   " << stat.depth << endl;
    	cout << "\tpeak memory:  " << memoriaMB << " MB (" << memoriaKB << " KB)" << endl;
    }

    return (0);
}
