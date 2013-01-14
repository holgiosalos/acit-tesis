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
      pre_time=tv.tv_sec*1000000+tv.tv_usec;
    }
  else
    {
      gettimeofday(&tv, NULL);
      return tv.tv_sec*1000000+tv.tv_usec - pre_time;
    }
    return 0;
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
	return result;
}

int main(int argc, char* argv[]) {
    ACiTOptions opt("ACiT");
    opt.solutions(0);
    //Establecimiento de los valores por defecto
    opt.file("test4.txt");
    //opt.file("test_files/dist0_10pac.txt");
    opt.preferencia(true);

    opt.slotsIntervalo(12); //12 slots por cada intervalo de tiempo, es decir 1 slot equivale a 5 minutos si el intervalo equivale a una hora
    opt.intervalosDia(11); //11 intervalos de tiempo para cada dia
//    cout << "sd: " << opt.slotsDia() << endl;
    opt.semanas(1); //1 semana para lograr todas las asignaciones de citas
    opt.intervalosSemana(60); //60 intervalos de tiempo por toda la semana (Lunes a Sabado) el sabado solo tendra 5 intervalos
//    cout << "ms: " << opt.makespan() << endl;

    opt.icl(ICL_BND);
    opt.parse(argc,argv);

    Lectura lector(opt.file());
    opt.reader(lector);
    opt.totalCitas(lector.totCitas());
//    cout << "totCit: " << opt.totalCitas() << endl;
    opt.totalEspecialistas(lector.numEspecialistas());
//    cout << "nE: " << opt.totalEspecialistas() << endl;

    opt.iniciar();

//    cout << "nPacPref: " << opt.nPacientesPreferencia() << endl;
    Gecode::IntSet codigos(opt.settingCodigos(), lector.numEspecialistas());
    opt.listaCodEspecialistas(codigos);

    /*
    //Comprobar que all ha quedado OK:
    vector<Especialidad> _listaEspecialidades = opt.listaEspecialidades();
    for(int i=0; i<(int)_listaEspecialidades.size(); i++){
    		cout << "Nombre: " << _listaEspecialidades[i].nombre() << " - ID:" <<_listaEspecialidades[i].id() << endl;
    		cout << "	-Citas:		" << _listaEspecialidades[i].totalCitas() << endl;
    		cout << "	-DuraciónM:	" << _listaEspecialidades[i].duracionCitasMinutos() << endl;
    		cout << "	-DuraciónS:	" << _listaEspecialidades[i].duracionCitasSlots() << endl;
    		cout << "	-NPac:		" << _listaEspecialidades[i].nPacientes() << " - " << _listaEspecialidades[i].pacientesString() << endl;
    		cout << "	-NEsp:		" << _listaEspecialidades[i].nEspecialistas() << " - " << _listaEspecialidades[i].especialistasString() << endl;
    		cout << endl;
    }
    */

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

    	int contador = -1;
    	do {
    		ultimaSolucion = acit;
    		contador++;
    	} while ((acit = search.next()));
    	unsigned long long tiempo = cronousec(0);

    	// Prints the statistics
    	cout << opt.name() << endl;
    	ultimaSolucion->print(opt.listaEspecialidades(), opt.nPacientesPreferencia());
    	Search::Statistics stat = search.statistics();

    	cout << "Initial:" << endl;
    	cout << "\tpropagators: " << np << endl;
    	cout << "\tbranchers:   " << nb << endl;
    	cout << endl;
    	cout << "Summary:" << endl;
    	cout << "\truntime:      " << convertir(tiempo, false) << " (" << convertir(tiempo, true) << " ms)" << endl;
    	cout << "\tsoulutions:   " << contador << endl;
    	cout << "\tpropagations: " << stat.propagate << endl;
    	cout << "\tnodes:        " << stat.node << endl;
    	cout << "\tfailures:     " << stat.fail << endl;
    	cout << "\tpeak depth:   " << stat.depth << endl;
    	cout << "\tpeak memory:  " << (stat.memory/1024) << " KB" << endl;
    }

    return 0;
}
