#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <algorithm>
#include <vector>
#include "PatientsData.cpp"

#define MAX_SESSIONS 5

using namespace std;

bool sort_list_patients(PatientsData a, PatientsData b){
	if(a.type < b.type)
		return true;
	return false;
}

bool saturday(int day, int initial_day){
	if(((day + initial_day - 1) % 7 )== 6)
		return true;
	
	return false;
}

bool sunday(int day, int initial_day){
	if((day + initial_day - 1) % 7 == 0)
		return true;
	
	return false;
}

bool friday(int day, int initial_day){
	if((day + initial_day - 1) % 7 == 5)
		return true;
	return false;
}

bool week_day_function(int day, int initial_day){
	if(((day + initial_day - 1) % 7 <= 5) && ((day + initial_day - 1) % 7 >= 1))
		return true;
	return false;
}

int random_generator(int min, int max){
	return min + ((double)(max - min) * (double)rand() / ((double) RAND_MAX));
}

int roullete_doctor(int *sesDoc, int nDoc){
	int total = 0;
	
	for(int i = 0; i < nDoc; i++)
		total += sesDoc[i];
	
	int r = random_generator(0, total);
	int acum = 0;
	for(int i = 0; i < nDoc; i++){
		acum += sesDoc[i];
		if(acum >= r)
			return i + 1;
	}
	
	return 0;
}

int sessionsEme(){
	double r = (double) rand() / ((double) RAND_MAX);
	
	if(r <= 0.2)
		return 4;
	if(r <= 0.6)
		return 2;
	return 1;
}

int sessionsPal(){
	double r = (double) rand() / ((double) RAND_MAX);
	
	if(r <= 0.5)
		return 1;
	if(r <= 0.8)
		return 4;
	return 10;
}

int sessionsRad(){
	double r = (double) rand() / ((double) RAND_MAX);

	if(r <= 0.5)
		return 25;
	return 35;
}

int sessions_pat(float r, int groups, float **prob){
  
	if(groups == 0)
		return 0;
  
	//float r = (double) rand() / ((double) RAND_MAX);
	float sum = 0.0;
	
	for(int i = 0; i < groups; i++){
		sum += prob[0][i];
		if(r < sum)
			return (int)prob[1][i];
	}	
	
	return (int)prob[1][groups - 1];
}

int main(int argc, char *argv[])
{
	/*Datos por entrada*/
	int groupEme, groupPal, groupRad, total_pat, nDays, nMach_high, nMach_low, seed;
	int laboralDays, initial_day;
	initial_day = 1;
	float nShifts, nEme, nPal, nRad;
	
	/*Inicializar semilla*/
	seed = atoi(argv[1]);
	srand(seed);
	nDays = 30;
	/*******************Leer entrada*******************/
	
	//Numero total pacientes
	cin >> total_pat;
	
	//Proporcion de pacientes Urgentes
	cin >> nEme;
	
	//Numero de grupos de sesiones
	cin >> groupEme;
	
	float **prob_ses_eme;
	
	prob_ses_eme = (float **)malloc(2 * sizeof(float **));
	
	for(int i = 0; i < 2; i++)
		prob_ses_eme[i] = (float *)malloc(sizeof(float *) * groupEme);

	for(int i = 0; i < groupEme; i++){
		cin >> prob_ses_eme[0][i];
		cin >> prob_ses_eme[1][i];
		
		prob_ses_eme[0][i] = prob_ses_eme[0][i] * nEme;
	}
	
	//Proporcion de pacientes Radicales
	cin >> nPal;
	
	//Numero de grupos de sesiones
	cin >> groupPal;
	
	float **prob_ses_pal = (float **)malloc(2 * sizeof(float **));;
	
	for(int i = 0; i < 2; i++)
		prob_ses_pal[i] = (float *)malloc(sizeof(float *) * groupPal) ;
	
	for(int i = 0; i < groupPal; i++){
		cin >> prob_ses_pal[0][i];
		cin >> prob_ses_pal[1][i];
		
		prob_ses_pal[0][i] = prob_ses_pal[0][i] * nPal;
	}
	
	prob_ses_pal[0][0] = prob_ses_pal[0][0] + nEme;
	
	//Proporcion de pacientes Paliativos
	cin >> nRad;
	
	//Numero de grupos de sesiones
	cin >> groupRad;
	
	float **prob_ses_rad = (float **)malloc(2 * sizeof(float **));
	
	for(int i = 0; i < 2; i++)
		prob_ses_rad[i] = (float *)malloc(sizeof(float *) * groupRad) ;

	for(int i = 0; i < groupRad; i++){
		cin >> prob_ses_rad[0][i];
		cin >> prob_ses_rad[1][i];
		
		prob_ses_rad[0][i] = prob_ses_rad[0][i] * nRad;
	}
	
	prob_ses_rad[0][0] = prob_ses_rad[0][0] + nEme + nPal;
	
	//Numero de interrupciones
	//0 significa sin interrupciones para el paciente
	float prob_int;
	cin >> prob_int;
	
	//Numero total para un día de tratamientos
	cin >> nShifts;
	
	//Número de maquinas
	cin >> nMach_low;
	cin >> nMach_high;

	//Maquinas Urgente
	float mach_eme;
	cin >> mach_eme;
	
	//Maquinas Paliativos
	float mach_pal;
	cin >> mach_pal;
	
	//Maquinas Radicales
	float mach_rad;
	cin >> mach_rad;
	
	/*******************Fin Leer entrada*******************/
	
	int work_days = nDays - nDays/7 * (7 - 5);
	int work_days_eme = nDays - nDays/7 * (7 - laboralDays);
	
	if(initial_day == 6 && laboralDays == 5)
		work_days -= 2;
	else if(initial_day == 7 && laboralDays == 5)
		work_days -= 1;
	else if((initial_day == 6 || initial_day == 7) && laboralDays == 6)
		work_days -= 1;
	

	//int total = nEme + nPal + nRad;
	//float aux_f = (float)nEme / total;
	//float aux = ((float)total_pat * aux_f);
	//nEme = (int)aux;
	//cout << nEme  << " " ;
	
	//aux_f = (float)nPal / total;
	//aux = ((float)total_pat * aux_f);
	//nPal = (int)aux;
	//cout << nPal  << " ";
	
	//aux_f = (float)nRad / total;
	//aux = ((float)total_pat * aux_f);
	//nRad = (int)aux;
	//cout << nRad  << " " << endl ;
	
	//cout << nShifts << " " ;
	
	//cout << endl;
	
	
	//cout << initial_day << " ";
	
	//cout << nMach_low << " " << nMach_high << endl << endl;
	
	std::vector<PatientsData> list_patients;
	
	for(int i = 1; i <= total_pat; i++){
		
		PatientsData aux;
		int type = 0;
		int sessions;
		
		double aleatorio = ((double)rand() / ((double) RAND_MAX));
		
		if(aleatorio < nEme){
			type = 1;
			aux.type = 1;
		}
		else if(aleatorio < nPal + nEme){
			type = 2;
			aux.type = 2;
		}
		else{
			type = 3;
			aux.type = 3;
		}
		
		if(type == 1)
		    sessions = sessions_pat(aleatorio, groupEme, prob_ses_eme);
		else if(type == 2)
		    sessions = sessions_pat(aleatorio, groupPal, prob_ses_pal);
		else
		    sessions = sessions_pat(aleatorio, groupRad, prob_ses_rad);	
		
		if(sessions > work_days)
			sessions = work_days;
		
		if(sessions > work_days && i > nEme)
			sessions = work_days;
		else if(sessions > work_days_eme && i <= nEme)
			sessions = work_days_eme;
		
		int r = 0;
		int beg = 0;
		
		r = 1 + ((double)(nDays - 1) * (rand() / ((double) RAND_MAX)));
			
		if(type == 1)
			beg = r + 1;
		else if(type == 2)
			beg = r + 13;
		else
			beg = r + 27;
		
		int ran = random_generator(1, sessions - 1);
		//int ran = 0;
		
		
		beg = beg + ran;
		r = r + ran;
		
		if(beg < 1)
			beg = 1;
		if(r < 1)
			r = 1;
		
		if(beg < r){
			//scout << beg << " " << r << " " ;
			aux.initialTreatmentDate = beg;
			aux.finalTreatmentDate = r;
		}
		else{
			//cout << r << " " << beg << " " ;
			aux.initialTreatmentDate = r;
			aux.finalTreatmentDate = beg;
		}
		
		//Numero de dias atrasado
		//cout << ran << " ";
		aux.delay = ran;
		
		//Numero sesiones
		//cout << sessions << " ";
		aux.nSessions = sessions;
		
		//Numero de interrupciones
		int interruptions = random_generator(0,ceil(prob_int * (float)sessions));
		
		//cout << interruptions << " ";
		aux.interruptions = interruptions;
		
		double ra = ((double)rand() / ((double) RAND_MAX));
		
		if(type == 1){
			if(ra < 1){
				//cout << "1 ";
				aux.machine = 1;
			}
			else{
				//cout << "2 ";
				aux.machine = 2;
			}
		}
		else if(type == 2){
			if(ra < mach_pal){
				//cout << "1 ";
				aux.machine = 1;
			}
			else{
				//cout << "2 ";
				aux.machine = 2;
			}
		}
		else{
			if(ra < mach_rad){
				//cout << "1 ";
				aux.machine = 1;
			}
			else{
				//cout << "2 ";
				aux.machine = 2;
			}
		}
		
		//Tiempo por sesion
		//cout << "10.0" << endl;
		aux.time_ses = 10.0;
		
		list_patients.push_back(aux);
	}
	
	std::sort(list_patients.begin(), list_patients.end(), sort_list_patients);
	
	int total_eme = 0;
	int total_pal = 0;
	int total_rad = 0;
	
	int total_ses_eme = 0;
	int total_ses_pal = 0;
	int total_ses_rad = 0;
	for(int i = 0; i <total_pat; i++){
		if(list_patients[i].type == 1){
			total_eme++;
			total_ses_eme += list_patients[i].nSessions;
		}
		else if(list_patients[i].type == 2){
			total_pal++;
			total_ses_pal += list_patients[i].nSessions;
		}
		else if(list_patients[i].type == 3){
			total_rad++;
			total_ses_rad += list_patients[i].nSessions;
		}
	}
	
	cout << "Total eme: " << total_eme << endl;
	cout << "Ses eme: " << total_ses_eme << endl << endl;
	cout << "Total pal: " << total_pal << endl;
	cout << "Ses pal: " << total_ses_pal << endl << endl;
	cout << "Total rad: " << total_rad << endl;
	cout << "Ses rad: " << total_ses_rad << endl << endl;
	
	for(int i = 0; i <total_pat; i++){
		cout << list_patients[i].type << " " ;
		cout << list_patients[i].initialTreatmentDate << " " ;
		cout << list_patients[i].finalTreatmentDate << " " ;
		cout << list_patients[i].delay << " " ;
		cout << list_patients[i].nSessions << " " ;
		cout << list_patients[i].interruptions << " " ;
		cout << list_patients[i].machine << " " ;
		cout << list_patients[i].time_ses << endl ;
	}
	
	return 0;
}


