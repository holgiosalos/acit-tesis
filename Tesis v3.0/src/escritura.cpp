/*
 * escritura.cpp
 *
 *  Created on: 8/07/2012
 *      Author: giovanny
 */

#include "escritura.h"
/************************** Metodos privados **************************/
string Escritura::itostr(int n) const{
	stringstream ss;
	ss << n;
	return ss.str();
}

/************************** Metodos publicos **************************/

Escritura::Escritura(string directorio) {
	_directorioSalida 	= directorio;
	calcularNombreArchivo();
	_fechaGnrlInicio = calculadora.fechaInicioCitas();
}

Escritura::~Escritura() { }

void Escritura::calcularNombreArchivo(void){
	time_t tSac 	= time(NULL);       // instante actual
	struct tm* tmP 	= localtime(&tSac);

	string year_mes_dia = itostr(tmP->tm_year+1900) + itostr(tmP->tm_mon+1) + itostr(tmP->tm_mday);
	string hh_mm_ss 	= itostr(tmP->tm_hour) + itostr(tmP->tm_min) + itostr(tmP->tm_sec);

	_nombreArchivo = "ACiT-"+year_mes_dia+"-"+hh_mm_ss+".xml";
}

void Escritura::semanas(int s) {
	_semanas = s;
}

void Escritura::slotsIntervalo(int sI) {
	_slotsIntervalo = sI;
}

void Escritura::slotsDia(int sD) {
	_slotsDia = sD;
}

void Escritura::intervalosDia(int iD) {
	_intervalosDia = iD;
}

void Escritura::intervalosSemana(int iS) {
	_intervalosSemana = iS;
}

string Escritura::determinarHora(int slot) const{

	string hora = itostr((int)floor((slot % _slotsDia) / _slotsIntervalo) + 7);
	string minutos = itostr((int)floor((slot % _slotsDia) % _slotsIntervalo) * 5);

	if(hora.size()==1){
		hora = "0"+hora;
	}
	if(minutos.size()==1){
		minutos = "0"+minutos;
	}

	return hora + ":" + minutos;
}

string Escritura::determinarDia(int slot) const{
	return itostr(int(slot/_slotsDia)+1);
}

int Escritura::determinarDiaInt(int slot) const{
	return int(slot/_slotsDia)+1;
}

vector<vector<string> > Escritura::getDatosDisponibilidad(Especialista esp, int idE) const{
	vector<vector<string> > res;
	vector<string> aux(3);
	vector<int> disponibilidad = esp.horariosAtencionEsp(idE);
	bool ini,fin;
	for(int i=0; i<_intervalosSemana; i+=_intervalosDia){
		ini = true; fin=false;
		for(int j=i; j<i+_intervalosDia; j++){
			if((disponibilidad[j] == 1)&&(ini==true)){
				aux[0] = itostr(int(floor(j/_intervalosDia))+1);
				aux[1] = determinarHora((j%_intervalosDia)*_slotsIntervalo);
				ini=false;
				if(j==(i+_intervalosDia)-1){
					aux[2] = determinarHora( ((j%_intervalosDia)+1)*_slotsIntervalo );
					fin=true;
				}
			}else if((disponibilidad[j] == 0)&&(ini==false)){
				aux[2] = determinarHora((j%_intervalosDia)*_slotsIntervalo);
				fin = true;
			}else if((disponibilidad[j] == 1) && (j==(i+_intervalosDia)-1) && (ini==false)){
				aux[2] = determinarHora( ((j%_intervalosDia)+1)*_slotsIntervalo );
				fin=true;
			}
			if((ini==false)&&(fin==true)){
				res.push_back(aux);
				break;
			}
		}
	}
	return res;
}

void Escritura::escribirXml(vector<Especialidad>* listaEspecialidades) const{

	TiXmlDocument outputfile;

	TiXmlDeclaration * decl = new TiXmlDeclaration("1.0","utf-8","");
	outputfile.LinkEndChild( decl );

	TiXmlElement * root = new TiXmlElement( "acit" );
	outputfile.LinkEndChild( root );

	//Semanas
	TiXmlElement * num_semanas = new TiXmlElement( "num_semanas" );
	num_semanas->LinkEndChild(new TiXmlText( itostr(_semanas) ));
	root->LinkEndChild(num_semanas);

	TiXmlElement * especialidad;
	TiXmlElement * profesional;
	TiXmlElement * disponibilidad;
	TiXmlElement * paciente;
	TiXmlElement * cita;

	for(int i=0; i<(int)listaEspecialidades->size(); i++){
		//Especialidades
		especialidad = new TiXmlElement( "especialidad");
		root->LinkEndChild( especialidad );
		especialidad->SetAttribute("id", listaEspecialidades->at(i).id());
		especialidad->SetAttribute("nombre", listaEspecialidades->at(i).nombre());
		especialidad->SetAttribute("duracion", listaEspecialidades->at(i).duracionCitasMinutos());

		vector<Especialista>* lstProfesionales = listaEspecialidades->at(i).especialistas();

		for(int j=0; j<(int)lstProfesionales->size(); j++){
			//especialista
			profesional = new TiXmlElement( "profesional");
			especialidad->LinkEndChild( profesional );
			profesional->SetAttribute("id", lstProfesionales->at(j).id());
			profesional->SetAttribute("nombre", lstProfesionales->at(j).nombre());

			//disponibilidad
			vector<vector<string> > infoDisponibilidad = getDatosDisponibilidad(lstProfesionales->at(j), listaEspecialidades->at(i).id());
			for(int d=0; d<(int)infoDisponibilidad.size(); d++){
				disponibilidad = new TiXmlElement( "disponible");
				profesional->LinkEndChild(disponibilidad);
				disponibilidad->SetAttribute("dia", infoDisponibilidad[d][0]);
				disponibilidad->SetAttribute("hora_ini", infoDisponibilidad[d][1]);
				disponibilidad->SetAttribute("hora_fin", infoDisponibilidad[d][2]);
			}

			//pacientes
			vector<Paciente> lstPacientes = lstProfesionales->at(j).pacientes();
			for(int k=0; k<(int)lstPacientes.size(); k++){
				paciente = new TiXmlElement( "paciente");
				profesional->LinkEndChild(paciente);
				paciente->SetAttribute("id", lstPacientes[k].id());
				paciente->SetAttribute("nombre", lstPacientes[k].nombre());
				paciente->SetAttribute("num_citas", lstPacientes[k].nCitas(listaEspecialidades->at(i).id()));

				vector<Cita> lstCita = lstPacientes[k].listaCitas();
				//antes de agregar una cita, hay que setear el numero de dias por semana
				for(int c=0; c<int(lstCita.size()); c++){
					lstCita[c].setDiasSemana((int)ceil(_intervalosSemana/_intervalosDia));
					tm_fecha fC = calculadora.sumarDias(_fechaGnrlInicio, lstCita[c].transformarDia());
					string year = itostr(fC.anio);
					string mes = itostr(fC.mes);
					if(mes.size() == 1){
						mes = "0"+mes;
					}
					string dia = itostr(fC.dia);
					if(dia.size() == 1){
						dia = "0"+dia;
					}

					cita = new TiXmlElement("cita");
					paciente->LinkEndChild(cita);
					cita->SetAttribute("fecha", year+"-"+mes+"-"+dia);
					cita->SetAttribute("hora_ini", determinarHora(lstCita[c].inicio()));
					cita->SetAttribute("hora_fin", determinarHora(lstCita[c].fin()));
				}
			}
		}
	}

	if(outputfile.SaveFile( _directorioSalida+"/"+_nombreArchivo )){
		cout << _nombreArchivo << endl;
	}else{
		cout << "error al guardar el archivo" << endl;
	}
}
