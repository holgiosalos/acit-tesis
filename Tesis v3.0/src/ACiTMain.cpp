#include "ACiTConstraints.cpp"

using namespace std;

int main(int argc, char* argv[]) {

    ACiTOptions opt("Asignador");
    //Establecimiento de los valores por defecto
    opt.file("test1.txt");

    opt.slotsIntervalo(12); //12 slots por cada intervalo de tiempo, es decir 1 slot equivale a 5 minutos si el intervalo equivale a una hora
    opt.intervalosDia(11); //11 intervalos de tiempo para cada dia
    cout << "sd: " << opt.slotsDia() << endl;
    opt.semanas(1); //1 semana para lograr todas las asignaciones de citas
    opt.intervalosSemana(60); //60 intervalos de tiempo por toda la semana (Lunes a Sabado) el sabado solo tendra 5 intervalos
    cout << "ms: " << opt.makespan() << endl; //El makespan se calcula semanas*intervalosSemana*slotsIntervalo

    opt.icl(ICL_DOM);	// Dominio consistencia
    opt.parse(argc,argv);

    Lectura lector(opt.file());
    opt.reader(lector);
    opt.totalCitas(lector.totCitas());
    cout << "totCit: " << opt.totalCitas() << endl;
    opt.totalEspecialistas(lector.numEspecialistas());
    cout << "nE: " << opt.totalEspecialistas() << endl;

    opt.iniciar();

    Gecode::IntSet codigos(opt.settingCodigos(), lector.numEspecialistas());
    opt.listaCodEspecialistas(codigos);
    cout << "numEsp: " << lector.numEspecialistas() << " tamcod: " << codigos.size() << " - cod: " << codigos << endl;
/*
    //Comprobar que all ha quedado OK:
    vector<Especialidad> _listaEspecialidades = opt.listaEspecialidades();
    for(int i=0; i<(int)_listaEspecialidades.size(); i++){
    		cout << "Nombre: " << _listaEspecialidades[i].id() << " - ID:" <<_listaEspecialidades[i].id() << endl;
    		cout << "	-Citas:		" << _listaEspecialidades[i].totalCitas() << endl;
    		cout << "	-DuraciónM:	" << _listaEspecialidades[i].duracionCitasMinutos() << endl;
    		cout << "	-DuraciónS:	" << _listaEspecialidades[i].duracionCitasSlots() << endl;
    		cout << "	-NPac:		" << _listaEspecialidades[i].nPacientes() << " - " << _listaEspecialidades[i].pacientesString() << endl;
    		cout << "	-NEsp:		" << _listaEspecialidades[i].nEspecialistas() << " - " << _listaEspecialidades[i].especialistasString() << endl;
    		cout << endl;
    }
*/



    //Script::run<ACiTConstraints,DFS,ACiTOptions>(opt);
    return 0;
}
