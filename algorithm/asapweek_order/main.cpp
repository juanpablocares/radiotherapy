#include <iostream>
#include <stdlib.h>
#include "Individual.cpp"
#include <algorithm>

#define FIRST 5.0

using namespace std; 

//Generador de numeros aleatorios entre 0 y 1
float random_0_1(){
	return ((double)rand() / ((double) RAND_MAX));
}

bool order_factor(pair<float, PatientsData> a, pair<float, PatientsData> b){
      return (a.first > b.first);
}

vector <PatientsData> order_patients(vector <PatientsData> list, int day){
      
      vector<pair<float, PatientsData> > factor;
  
      for(int i = 0; i < (int)list.size(); i++){
	      pair <float, PatientsData> aux_pair;
	      
	      int delay = 0;
	      if(list[i].category == 1)
		    delay = 2;
	      else if(list[i].category == 2)
		    delay = 14;
	      else
		    delay = 28;
	      if(day < list[i].initialTreatmentDate)
		    aux_pair.first = -1.0;
	      else
		    aux_pair.first = ((float)(day - list[i].initialTreatmentDate + 1))/((float)delay);
	      
	      aux_pair.second = list[i];
	      factor.push_back(aux_pair);
      }
      
      std::sort(factor.begin(), factor.end(), order_factor);
      
      list.clear();
      for(int i = 0; i < (int)factor.size(); i++){
	      PatientsData temporal_pat = factor[i].second;
	      list.push_back(temporal_pat);
      }
      
      return list;
}

bool sort_category_day(PatientsData a, PatientsData b){
	if(a.initialTreatmentDate == b.initialTreatmentDate){
		return a.category < b.category;
	}
	
	else
		return (a.initialTreatmentDate < b.initialTreatmentDate);
}

bool sort_category(PatientsData a, PatientsData b){
      return (a.category < b.category);
}

bool sort_waiting(PatientsData a, PatientsData b){
      
      return (a.initialTreatmentDate < b.initialTreatmentDate);
  
      //return false;
}

vector <PatientsData> erase_patient(int id, vector <PatientsData> list){
      
      for(int i = 0; i < (int)list.size(); i++){
	      if(list[i].id == id){
		      list.erase(list.begin() + i);
		      return list;
	      }
      }
      
      return list;
}


int main(int argc, char *argv[])
{ 
      //Parametros algoritmo
      int seed = atoi(argv[1]);
      
      int nDays;
      //Fin parametros algoritmo
      
      //Lectura de archivo de entrada
      
      //Numero de dias a planificar
      cin >> nDays;
      
      //Tiempo disponible por dia de planificacion
      float tShift;
      cin >> tShift;
      
      //Numero de maquinas de baja energia
      int low_machine;
      cin >> low_machine;
      
      //Numero de maquinas de alta energia
      int high_machine;
      cin >> high_machine;
      
      //Fin lectura archivo de entrada
      
      
      
      
      
      
      
      

      
      
      //Inicializar semilla
      srand(seed);

      //Variable que almacenara la mejor solucion global del algoritmo
      Individual global;
      
      //Lectura de informacion de los pacientes
      vector <PatientsData> patientsData;
      //Pacientes que estan en lista de espera por insuficiencia de maquinas en el dia especificado
      vector <PatientsData> patients_waiting;
      //REVISAR MEMORIA ACA, YA QUE PIDO MAS DE LO NECESARIO
      vector <int> quan_pat(600, 0);
      
      //Inicializar parametros del individuo
      global.set_params(tShift, 600, low_machine, high_machine, 1);
      //Vector que contiene el tiempo disponible por cada maquina
      vector<int> time_machines(low_machine + high_machine, tShift);
      //Inicializar parametros de las maquinas
      global.set_time_machines(time_machines, 600, low_machine + high_machine);
      
      int id = 0;
      bool week = false;
      //int sum = 0;
      for(int d = 1; d <= nDays || !patients_waiting.empty(); d++){
	      int total_pat;
	      //cout << d << endl;
	      if(d <= nDays){
		    cin >> total_pat;
		   // cout << total_pat << endl;
	      }

	      if(d % 7 == 0)
			week = true;
	      else
			week = false;
	      
	      //Analizar los pacientes que se deben atener en el dia d
			for(int i = 0; week && (int)patients_waiting.size(); i++){
				//cout << "Inicial: " << patients_waiting[i].initialTreatmentDate << " actual " << d << endl;
				if(!(patients_waiting[i].initialTreatmentDate > d)){

					  std::vector < std::pair< int, int > > s;
					  
					  int local_machine = patients_waiting[i].machine;
					  if(patients_waiting[i].machine == 3){
						float r = random_0_1();
						if(r < 0.5)
						      local_machine = 1;
						else
						      local_machine = 2;
					  }
					  
					  s = global.asap_algorithm(patients_waiting[i].id, local_machine, patientsData);
					  
					  if(s.size() == 0){
						  //global.show_vector();
						  cout << "PROBLEMA " << patients_waiting[i].id << endl;
						  exit(0);
						  //cout << global.time_machine_i(0, d) << endl;
						  //Update lista
						  std::sort(patients_waiting.begin(), patients_waiting.end(), sort_waiting);
						  patients_waiting = order_patients(patients_waiting, d);
						  quan_pat[d]++;
						  //exit(0);
					  }
					  else{
						  //Se puede insertar en la planificacion
						  global.insert_schedul(patients_waiting[i].id, s, patientsData);
						  patients_waiting = erase_patient(patients_waiting[i].id, patients_waiting);
					  }
				}
			}
	      
	      //Almacenamiento de información de los pacientes
	      for(int i = 0; i < total_pat && d <= nDays; i++){
		      PatientsData pat_aux;
		      id++;
		      pat_aux.id = id;
		      cin >> pat_aux.category;
		      cin >> pat_aux.initialTreatmentDate;
		      cin >> pat_aux.finalTreatmentDate;
		      pat_aux.days_delay = 0;
		      cin >> pat_aux.nSessions;
		      cin >> pat_aux.interruptions;
		      cin >> pat_aux.machine;
		      cin >> pat_aux.duration_session;
		      pat_aux.first_session = pat_aux.duration_session + FIRST;
		      patientsData.push_back(pat_aux);
		      patients_waiting.push_back(pat_aux);
		      quan_pat[pat_aux.initialTreatmentDate - 1]++;
	      }

	      if(d <= nDays){
		      std::sort(patients_waiting.begin(), patients_waiting.end(), sort_category_day);
		      //patients_waiting = order_patients(patients_waiting, d);
	      }

      }
      //cout << endl << "Solucion" << endl;
      //cout << "Urgentes: " << global.getEmergency() << endl;
      //cout << "Paliativos: " << global.getPalliative() << endl;
      //cout << "Radicales: " << global.getRadical() << endl;
      global.show_vector(global.getEmergency(), global.getPalliative(), global.getRadical(), patientsData);
      return 0;
}
