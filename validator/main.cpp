#include <iostream>
#include <stdlib.h>
#include <vector>
#include <utility>
#include <algorithm> 
#include <math.h>
#include "PatientsData.cpp"
#define FIRST 5.0

using namespace std;

bool feasible(std::vector<std::vector < std::pair< int, int > > > schedul, PatientsData p){
	
	//Valida si no comenzo antes de la fecha definida
	if(p.initialTreatmentDate - schedul[p.id - 1][0].first > 0){
		cout << "Paciente comenzo antes de la fecha: " << endl;
		cout << "Deberia comenzar en: " << p.initialTreatmentDate << endl;
		cout << "Comenzo en: " << schedul[p.id - 1][0].first << endl;
		return false;
	}
	
	//El paciente está asignado a una única máquina
	int m = schedul[p.id - 1][0].second;
	for(int i = 1; i < (int)schedul[p.id - 1].size(); i++){
		if(m != schedul[p.id - 1][i].second){
			cout << "Cambio de maquina" << endl;
			return false;
		}
	}
	
	//Numero de sesiones
	if((int)schedul[p.id - 1].size() != p.nSessions){
		cout << "Numero de sesiones distinto al necesario" << endl;
		return false;
	}
	
	return true;
}

int main(int argc, char *argv[])
{  
	std::vector<std::vector < std::pair< int, int > > > schedul;
	
	int n, s, nEme, nPal, nRad;
	
	cin >> n;
	schedul.resize(n);
	
	for(int i = 0; i < n; i++){
		cin >> s;
		
		for(int j = 0; j < s; j++){
			int day;
			int mach;
			
			cin >> day;
			cin >> mach;
			
			schedul[i].push_back(make_pair(day, mach));
		}
	}
	//Leer entrada
	
	//Número de pacientes Urgentes
	cin >> nEme;

	//Número de pacientes Paliativos
	cin >> nPal;

	//Número de pacientes Radicales
	cin >> nRad;
	
	vector<PatientsData> patientsData;
	
	//Almacenamiento de información de los pacientes
	for(int i = 0; i < nPal + nRad + nEme; i++){
		PatientsData pat_aux;
		cin >> pat_aux.id;
		cin >> pat_aux.category;
		cin >> pat_aux.initialTreatmentDate;
		cin >> pat_aux.finalTreatmentDate;
		cin >> pat_aux.days_delay;
		cin >> pat_aux.nSessions;
		cin >> pat_aux.interruptions;
		cin >> pat_aux.machine;
		cin >> pat_aux.duration_session;
		pat_aux.first_session = pat_aux.duration_session + FIRST;
		
		patientsData.push_back(pat_aux);
		
		if(!feasible(schedul, pat_aux)){
			cout << "Solucion infactible" << endl;
			cout << "Paciente " << i+1 << endl;
			exit(0);
		}
	}
	
	//Fin leer entrada
	
	return 0;
}
