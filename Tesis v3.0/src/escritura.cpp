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

Escritura::Escritura(vector<Especialidad> l0, vector<Especialista> l1, vector<Paciente> l2) {
	listaEspecialidades = l0;
	listaProfesionales	= l1;
	listaPacientes		= l2;
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
	string hora = itostr((int)floor((slot / _slotsIntervalo) % _intervalosDia) + 7);

	string minutos = itostr((slot % _slotsIntervalo) * 5);

	if(hora.size()==1){
		string cero = "0";
		cero += hora;
		hora = cero;
	}
	if(minutos == "0"){
		minutos += "0";
	}

	return hora + ":" + minutos;
}

string Escritura::determinarDia(int slot) const{
	return itostr(int(slot/_slotsDia)+1);
}

vector<vector<string> > Escritura::getDatosDisponibilidad(Especialista esp, int idE) const{
	vector<vector<string> > res;
	vector<string> aux(3);
	vector<int> disponibilidad = esp.horariosAtencionEsp(idE);
	int i=0;
	bool ini,fin;
	while(i < int(disponibilidad.size())){
		ini=true, fin=false;
		aux[0] = determinarDia(i*_slotsIntervalo); //dia
		for(int j=i; j<(i+_intervalosDia); j++){
			if( (disponibilidad[j]==1) && ini ){
				aux[1] = determinarHora(j*_slotsIntervalo); //hora_ini
				ini = false;
				if(j == (i+_intervalosDia)-1){
					aux[2] = determinarHora((j*_slotsIntervalo)-1); //hora_fin
					fin = true;
				}
			}
			if((disponibilidad[j]==0)&& !ini && !fin){
				aux[2] = determinarHora(j*_slotsIntervalo); //hora_fin
				fin=true;
				break;
			}
		}
		res.push_back(aux);
		i += _intervalosDia;
	}
	return res;
}

void Escritura::escribirXml() const{

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
//	TiXmlElement * paciente;
//	TiXmlElement * cita;

	for(int i=0; i<(int)listaEspecialidades.size(); i++){
		//Especialidades
		especialidad = new TiXmlElement( "especialidad");
		root->LinkEndChild( especialidad );
		especialidad->SetAttribute("id", listaEspecialidades[i].id());
		especialidad->SetAttribute("nombre", listaEspecialidades[i].nombre());
		especialidad->SetAttribute("duracion", listaEspecialidades[i].duracionCitasMinutos());

		vector<Especialista> lstProfesionales = listaEspecialidades[i].especialistas();

		for(int j=0; j<(int)lstProfesionales.size(); j++){
			//especialista
			profesional = new TiXmlElement( "profesional");
			especialidad->LinkEndChild( profesional );
			profesional->SetAttribute("id", lstProfesionales[j].id());
			profesional->SetAttribute("nombre", lstProfesionales[j].nombre());

			//disponibilidad
			vector<vector<string> > infoDisponibilidad = getDatosDisponibilidad(lstProfesionales[j], listaEspecialidades[i].id());
			for(int d=0; d<(int)infoDisponibilidad.size(); d++){
				disponibilidad = new TiXmlElement( "disponibilidad");
				profesional->LinkEndChild(disponibilidad);
				disponibilidad->SetAttribute("dia", infoDisponibilidad[d][0]);
				disponibilidad->SetAttribute("hora_ini", infoDisponibilidad[d][1]);
				disponibilidad->SetAttribute("hora_fin", infoDisponibilidad[d][2]);
			}
		}
	}


			//pacientes
				//cita

	outputfile.SaveFile( "madeByHand.xml" );
}
