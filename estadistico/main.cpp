#include <iostream>
#include <stdlib.h>
#include <vector>
#include <utility>
#include <algorithm> 
#include <math.h>
#include "PatientsData.cpp"
#define ADD_TIME_FIRST 5.0

using namespace std;

float prom_eme;
float prom_pal;
float prom_rad;

float pr_delta(std::vector<std::vector < std::pair< int, int > > > schedul, PatientsData * patData, int eme, int pal, int rad){
	float prom_delta = 0.0;
	prom_eme = 0.0;
	prom_pal = 0.0;
	prom_rad = 0.0;
	for(int i = 0; i < (int)schedul.size(); i++){
	  
		if(schedul[i][0].first > patData[i].finalTreatmentDate){
			float aux = ((float)(schedul[i][0].first - patData[i].finalTreatmentDate));
			
			if(i < eme)
				prom_eme += aux;
			else if(i < eme + pal){
				prom_pal += aux;
				//cout << "ctm: " << aux << endl;
			}
			else if(i < eme + pal + rad)
				prom_rad += aux;
			
			prom_delta += aux;
		}
	}
	
	cout << "Prom urg: " << prom_eme / ((float)eme) << endl;
	cout << "Prom pal: " << prom_pal / ((float)pal) << endl;
	cout << "Prom rad: " << prom_rad / ((float)rad) << endl;
	cout << "Prom: " << (prom_eme + prom_pal + prom_rad) / ((float)schedul.size()) << endl << endl;
	prom_eme = prom_eme / ((float)eme);
	prom_pal = prom_pal / ((float)pal);
	prom_rad = prom_rad / ((float)rad);
	return prom_delta / ((float)schedul.size());
  
}

float desv_delta(std::vector<std::vector < std::pair< int, int > > > schedul, PatientsData * patData, int eme, int pal, int rad){
	float prom_delta = pr_delta(schedul, patData, eme, pal, rad);
	float desv = 0.0;
	float desv_eme = 0.0;
	float desv_pal = 0.0;
	float desv_rad = 0.0;
	for(int i = 0; i < (int)schedul.size(); i++){
	  
		if(schedul[i][0].first > patData[i].finalTreatmentDate){
			float aux = ((float)(schedul[i][0].first - patData[i].finalTreatmentDate)) - prom_delta;
			aux = aux * aux;
			desv += aux;
			if(i < eme){
				float aux_eme = ((float)(schedul[i][0].first - patData[i].finalTreatmentDate));
				aux_eme = aux_eme * aux_eme;
				desv_eme += aux_eme;
			}
			else if(i < eme + pal){
				float aux_pal = ((float)(schedul[i][0].first - patData[i].finalTreatmentDate));
				aux_pal = aux_pal * aux_pal;
				desv_pal += aux_pal;
			}
			else if(i < eme + pal + rad){
				float aux_rad = ((float)(schedul[i][0].first - patData[i].finalTreatmentDate));
				aux_rad = aux_rad * aux_rad;
				desv_rad += aux_rad;
			}
		}
	}

	cout << "Desv urg: " << sqrt(desv_eme / ((float)eme) - prom_eme * prom_eme) << endl;
	cout << "Desv pal: " << sqrt(desv_pal / ((float)pal) - prom_pal * prom_pal) << endl;
	cout << "Desv rad: " << sqrt(desv_rad / ((float)rad) - prom_rad * prom_rad) << endl;
	
	
	desv = desv / (float)schedul.size();
	
	return sqrt(desv);
}

float calculate_entropy(std::vector<std::vector < std::pair< int, int > > > schedul, PatientsData * patientsData, int nEme, int nPal, int nRad){
	std::vector<int> list_eme;
	std::vector<int> list_pal;
	std::vector<int> list_rad;
	
	for(int i = 0; i < (int)schedul.size(); i++){
	  
		int delay = schedul[i][0].first - patientsData[i].finalTreatmentDate + patientsData[i].days_delay;
	  
		if(delay > 0 && i < nEme)
		      list_eme.push_back(delay);
		else if(i < nEme && delay <= 0)
		      list_eme.push_back(0);
		else if(delay > 0 && i < nEme + nPal)
		      list_pal.push_back(delay);
		else if(i < nEme + nPal && delay <= 0)
		      list_pal.push_back(0);
		else if(delay > 0 && i < nEme + nPal + nRad)
		      list_rad.push_back(delay);
		else if(i < nEme + nPal + nRad && delay <= 0)
		      list_rad.push_back(0);
	}
	
	//Calculo pacientes urgencia
	//cout << "Eme" << endl;
	std::sort(list_eme.begin(), list_eme.end());
	std::vector <int> copy_eme(list_eme);
	//cout << "Inicial: " << list_pal.size() << endl;
	list_eme.erase(unique(list_eme.begin(), list_eme.end()), list_eme.end());
	//cout << "Final: " << list_pal.size() << endl;
	float entropy_eme = 0.0;
	for(int i = 0; i < (int)list_eme.size(); i++){
		int count = std::count(copy_eme.begin(), copy_eme.end(), list_eme[i]);
		float cal = ((float)count)/((float)copy_eme.size());
		entropy_eme += (cal)*log(cal);
		//cout << (cal)*log(cal) << endl;
	}
	entropy_eme = entropy_eme * -1.0;
	//Fin calculo pacentes urgencia
	
	//Calculo pacientes paliativos
	//cout << "Pal" << endl;
	std::sort(list_pal.begin(), list_pal.end());
	std::vector <int> copy_pal(list_pal);
	//cout << "Inicial: " << list_pal.size() << endl;
	list_pal.erase(unique(list_pal.begin(), list_pal.end()), list_pal.end());
	//cout << "Final: " << list_pal.size() << endl;
	float entropy_pal = 0.0;
	for(int i = 0; i < (int)list_pal.size(); i++){
		int count = std::count(copy_pal.begin(), copy_pal.end(), list_pal[i]);
		float cal = ((float)count)/((float)copy_pal.size());
		entropy_pal += (cal)*log(cal);
		//cout << (cal)*log(cal) << endl;
	}
	entropy_pal = entropy_pal * -1.0;
	//Fin calculo pacientes paliativos
	
	//Calculo pacientes radicales
	//cout << "Rad" << endl;
	std::sort(list_rad.begin(), list_rad.end());
	std::vector <int> copy_rad(list_rad);
	//cout << "Inicial: " << list_pal.size() << endl;
	list_rad.erase(unique(list_rad.begin(), list_rad.end()), list_rad.end());
	//cout << "Final: " << list_pal.size() << endl;
	float entropy_rad = 0.0;
	for(int i = 0; i < (int)list_rad.size(); i++){
		int count = std::count(copy_rad.begin(), copy_rad.end(), list_rad[i]);
		//cout << "Rad: " << count << endl;
		float cal = ((float)count)/((float)copy_rad.size());
		//cout << (cal)*log(cal) << endl;
		entropy_rad += (cal)*log(cal);
		//cout << (cal)*log(cal) << endl;
	}
	entropy_rad = entropy_rad * -1.0;
	//Fin calculo pacientes radicales
	
	//cout << entropy_eme + entropy_pal + entropy_rad << endl;
	cout << "Entropia urgente: " << entropy_eme << endl;
	cout << "Entropia paliativos: " << entropy_pal << endl;
	cout << "Entropia radicales: " << entropy_rad << endl;
	
	return entropy_eme + entropy_pal + entropy_rad;
}

int calculate_delay(std::vector<std::vector < std::pair< int, int > > > schedul, PatientsData * patientsData, int nEme, int nPal, int nRad){
	int delay_eme = 0;
	int delay_pal = 0;
	int delay_rad = 0;
	int delay = 0;
	
	for(int i = 0; i < (int)schedul.size(); i++){
		delay += patientsData[i].days_delay;
		if(i < nEme && schedul[i][0].first - patientsData[i].finalTreatmentDate > 0)
		      delay_eme += schedul[i][0].first - patientsData[i].finalTreatmentDate;
		else if(i < nEme + nPal && schedul[i][0].first - patientsData[i].finalTreatmentDate > 0)
		      delay_pal += schedul[i][0].first - patientsData[i].finalTreatmentDate;
		else if(i < nEme + nPal + nRad && schedul[i][0].first - patientsData[i].finalTreatmentDate > 0)
		      delay_rad += schedul[i][0].first - patientsData[i].finalTreatmentDate;
	}
	
	cout << delay_eme << endl;
	cout << delay_pal << endl;
	cout << delay_rad << endl;
	return delay_eme + delay_pal + delay_rad + delay;
}

void show_entropy(std::vector<std::vector < std::pair< int, int > > > schedul, PatientsData * patientsData, int nEme, int nPal, int nRad){
	int delay_eme = 0;
	int delay_pal = 0;
	int delay_rad = 0;
	//int delay = 0;
	
	for(int i = 0; i < (int)schedul.size(); i++){
		//delay += schedul[i][0].first - patientsData[i].initialTreatmentDate;
		if(i < nEme){
		      delay_eme += schedul[i][0].first - patientsData[i].initialTreatmentDate;
		      //if(schedul[i][0].first - patientsData[i].initialTreatmentDate != 0)
			      //cout << schedul[i][0].first << " " << patientsData[i].initialTreatmentDate << " = " << schedul[i][0].first - patientsData[i].initialTreatmentDate << endl;
		}
		else if(i < nEme + nPal){
		      delay_pal += schedul[i][0].first - patientsData[i].initialTreatmentDate;
		     // if(schedul[i][0].first - patientsData[i].initialTreatmentDate != 0)
			    //  cout << schedul[i][0].first << " " << patientsData[i].initialTreatmentDate << " = " << schedul[i][0].first - patientsData[i].initialTreatmentDate << endl;
		}
		else if(i < nEme + nPal + nRad){
		      delay_rad += schedul[i][0].first - patientsData[i].initialTreatmentDate;
		      //if(schedul[i][0].first - patientsData[i].initialTreatmentDate != 0)
			     // cout << schedul[i][0].first << " " << patientsData[i].initialTreatmentDate << " = " << schedul[i][0].first - patientsData[i].initialTreatmentDate << endl;
		}
	}
	
	cout << delay_eme << endl;
	cout << delay_pal << endl;
	cout << delay_rad << endl;
}

void show_vector(std::vector<std::vector < std::pair< int, int > > > schedul){
		  
	for(int i = 0; i < (int)schedul.size(); i++){
		for(int j = 0; j < (int)schedul[i].size(); j++){
			cout << schedul[i][j].first << " " ;
			cout << schedul[i][j].second << " " ;
		}
		cout << endl;
	}
}

int main(int argc, char *argv[])
{  
	std::vector<std::vector < std::pair< int, int > > > schedul;
	
	int n, s, nEme, nPal, nRad, nShifts, first_day_week, low_machine, high_machine;
	
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
	
	//Tiempo total para un día de tratamientos
	cin >> nShifts;
	
	//Día comienzo de semana
	cin >> first_day_week;
	
	if(first_day_week > 5 || first_day_week < 1){
		cout << "Dia inicial erroneo: " << first_day_week << endl;
		exit(0);
	}
	
	//Número de maquinas baja energia
	cin >> low_machine;
	
	//Numero maquinas alta energia
	cin >> high_machine;

	//Vector que contiene el tiempo disponible por cada maquina
	vector<int> time_machines(low_machine + high_machine, nShifts);
	
	PatientsData * patientsData = new PatientsData[nPal + nRad + nEme];
	
	//Almacenamiento de información de los pacientes
	for(int i = 0; i < nPal + nRad + nEme; i++){
		//cin >> patientsData[i].doctor;
		cin >> patientsData[i].initialTreatmentDate;
		cin >> patientsData[i].finalTreatmentDate;
		cin >> patientsData[i].days_delay;
		cin >> patientsData[i].nSessions;
		cin >> patientsData[i].interruptions;
		cin >> patientsData[i].machine;
		cin >> patientsData[i].duration_session;
		patientsData[i].first_session = patientsData[i].duration_session + ADD_TIME_FIRST;
		
		if(i < nEme)
			patientsData[i].category = 1;
		else if(i < nEme + nPal)
			patientsData[i].category = 2;
		else
			patientsData[i].category = 3;
		
	}
	
	//Fin leer entrada
	//show_vector(schedul);
	
// 	for(int i = 0; i < (int)schedul.size(); i++){
// 		      cout << i+1 << " " << schedul[i][0].first - patientsData[i].finalTreatmentDate << endl;
// 	}
	
	cout << "Delay: " << calculate_delay(schedul, patientsData, nEme, nPal, nRad) << endl;
	cout << "Entropia: " << calculate_entropy(schedul, patientsData, nEme, nPal, nRad) << endl;
	//cout << "Promedio: " << pr_delta(schedul, patientsData, nEme, nPal, nRad) << endl;
	cout << "Desviacion: " << desv_delta(schedul, patientsData, nEme, nPal, nRad) << endl;
	
	return 0;
}
