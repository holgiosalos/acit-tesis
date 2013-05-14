/*
 * restart.h
 *
 *  Created on: 2/05/2013
 *      Author: giovanny
 */

#ifndef RESTART_H_
#define RESTART_H_

#include <cstdio>
#include <math.h>

#include <gecode/driver.hh>
#include <gecode/int.hh>
#include <gecode/minimodel.hh>
#include "ACiTOptions.h"

using namespace Gecode;
using namespace std;

/// Timer interface used for examples
class Timer {
private:
    clock_t t0;
public:
    /// Start timer
    void start(void) {
        t0 = clock();
    }
    /// Stop timer
    double stop(void) {
        return (static_cast<double>(clock()-t0) / CLOCKS_PER_SEC) * 1000.0;
    }
};

class FailTimeStop : public Search::Stop {
private:
    Search::TimeStop *ts; ///< Used time stop object
    Search::FailStop *fs; ///< Used fail stop object
public:
    /// Initialize stop object
    FailTimeStop(int fails, int time) {
        ts = new Search::TimeStop(time);
        fs = new Search::FailStop(fails);
    }
    /// Test whether search must be stopped
    virtual bool stop(const Search::Statistics& s, const Search::Options& o) {
        return fs->stop(s,o) || ts->stop(s,o);
    }
    /// Create appropriate stop-object
    static Search::Stop* create(unsigned int fail, unsigned int time) {
        if ((fail == 0) && (time == 0))
            return NULL;
        if (fail == 0)
            return new Search::TimeStop(time);
        if (time  == 0)
            return new Search::FailStop(fail);
        return new FailTimeStop(fail,time);
    }
};

// Serie de Luby
/* Esta funcion está basada en por un segmento de codigo
 * fuente realizado por:  Henry Kautz
 * y se encuentra disponible en: http://www.satcompetition.org/gorydetails/?p=3
 */
long lubyRestart(int i)
{
    long power;

    int k=1;

    if (i<=0){
        std::cout<<i + " No es un ingreso válido."<<std::endl;
        exit(1);
    }

    power = 2;
    while (power < (i+1)){
        k++;
        power *= 2;
    }

    if (power == (i+1)) return (power/2);
    return (lubyRestart(i - (power/2) + 1));
}

template<class Problem>
void restart(ACiTOptions &opt) {

    MaximizeScript *s = new Problem(opt);
    Search::Options so;
    so.c_d = opt.c_d();
    so.a_d = opt.a_d();
    int i = opt.solutions();

    int failures;  // cutoff
    double geom = opt.factor();   // factor

    if(!opt.isLuby()) {
		failures=opt.cutoff();
	}
	else{
		failures=opt.cutoffLuby();
	}

    double time=opt.time(),tTime=0;
    int tfailures=0, tnodes=0, tprop=0;
    int idRestart=0;
    bool end=false;
    Timer t;

    while(!end) {
        t.start();
        idRestart++;
        std::cout<<"-"; fflush(stdout);
        so.stop = new FailTimeStop(failures, (int)time);
        DFS<MaximizeScript> e(s,so);
        //s->print(std::cout);

        do {
        	MaximizeScript *ex = e.next();
            //std::cout<<"Sol: "<<ex<<std::endl;
            if (ex == NULL) {
                break;
            }
            else {
                std::cout<<std::endl;
                ex->print(std::cout);
                delete ex;
            }
        } while (--i != 0);

		Search::Statistics stat = e.statistics();
        //Actualización de las Estadísticas Globales
        tfailures +=stat.fail;
        tnodes +=stat.node;
        tprop +=stat.propagate;
        double tmpTime=t.stop();
        time -=(static_cast<int>(tmpTime));
        tTime +=tmpTime;

        if(e.stopped()) {
            if(!opt.isLuby()) {
				failures=(int)((double)failures*geom);
			}
			else{
				failures=(int)((double)lubyRestart(idRestart)*opt.cutoffLuby());
			}
        }
        else end=true;
        if(i==0) end=true;
        if(time<=0) end=true;
        if(end==true) {
			// Impresión de las Estadísticas
        	s->print(std::cout);
            std::cout<<std::endl
            <<"Nodes: \t\t"<<tnodes<<std::endl
            <<"Failures: \t"<<tfailures<<std::endl
            <<"Propagation: \t"<<tprop<<std::endl
            <<"Restart No: \t"<<idRestart<<std::endl
            <<"Time: \t\t"<<tTime<<std::endl
            <<"=================="<<std::endl;

            if(!opt.isLuby()){
				std::cout<<"RESTART GEOMETRIC | STATIC"<<std::endl
						 <<"\tInitial Cutoff Failures: \t"<<opt.cutoff()<<std::endl
						 <<"\tInitial Geometric Restart: \t"<<opt.factor()<<std::endl;
			}
			else {
				std::cout<<"RESTART LUBY"<<std::endl
						 <<"\tParametrer: \t\t\t"<<opt.cutoffLuby()<<std::endl;
			}
			std::cout<<"\tFile: \t\t\t\t"<<opt.file()<<std::endl;
        }
        delete so.stop;
    }
    delete s;
}

#endif /* RESTART_H_ */
