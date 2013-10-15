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

bool order_factor(pair <int, float> a, pair <int, float> b){
      return false;
}

int main(int argc, char *argv[])
{ 
      //Parametros algoritmo
      int seed = atoi(argv[1]);
      int iterations = atoi(argv[2]);
      int restart = atoi(argv[3]);
      float p_insert_eliminate = atof(argv[4]);
      float p_eme = atof(argv[5]);
      float p_pal = atof(argv[6]);
      float p_rad = atof(argv[7]);
      int nDays = 60;
      //Fin parametros algoritmo
      
      //Lectura de archivo de entrada
      
      //Numero de pacientes urgentes
      int nEme;
      cin >> nEme;
      
      //Numero pacientes paliativos
      int nPal;
      cin >> nPal;
      
      //Numero pacientes radicales
      int nRad;
      cin >> nRad;
      
      //Tiempo disponible por dia de planificacion
      float tShift;
      cin >> tShift;
      
      //Numero de maquinas de baja energia
      int low_machine;
      cin >> low_machine;
      
      //Numero de maquinas de alta energia
      int high_machine;
      cin >> high_machine;
      
      //Lectura de informacion de los pacientes
      PatientsData * patientsData = new PatientsData[nPal + nRad + nEme];
      
      //Almacenamiento de información de los pacientes
      for(int i = 0; i < nPal + nRad + nEme; i++){
	      cin >> patientsData[i].initialTreatmentDate;
	      cin >> patientsData[i].finalTreatmentDate;
	      cin >> patientsData[i].days_delay;
	      cin >> patientsData[i].nSessions;
	      cin >> patientsData[i].interruptions;
	      cin >> patientsData[i].machine;
	      cin >> patientsData[i].duration_session;
	      patientsData[i].first_session = patientsData[i].duration_session + FIRST;
	      
		if(i < nEme)
		      patientsData[i].category = 1;
		else if(i < nEme + nPal)
		      patientsData[i].category = 2;
		else
		      patientsData[i].category = 3;
      }
      
      //Fin lectura archivo de entrada
      
      
      
      
      
      
      
      
      
      
      
      
      //Inicializar semilla
      srand(seed);
      //Vector que contiene el tiempo disponible por cada maquina
	vector<int> time_machines(low_machine + high_machine, tShift);
      //Variable que almacenara la mejor solucion global del algoritmo
      Individual global;
      
      //Variable que almacenara el tiempo de ejecucion del greedy y el algoritmo
      unsigned int t_greedy = 0;
      unsigned int t_hc = 0;
      
      //Numero de pacientes que se insertaran-eliminaran con el movimiento
      int n = (float)(nPal + nRad + nEme) * p_insert_eliminate;
      
      for(int res = 0; res < restart; res++){
	    Individual aux;
	    
	    //Iniciar parametros de entrada
	    aux.set_params(tShift, nDays, low_machine, high_machine, patientsData, nEme, nPal, nRad, 1);
	    aux.set_time_machines(time_machines, nDays, low_machine + high_machine);
	    
	    //Tiempo inicial del greedy
	    time_t aux_greedy_beg = time(NULL);
	    
	    vector <pair <int, float> > list_patients;
	    for(int i = 0; i < nPal + nRad + nEme; i++){
		   list_patients.push_back(make_pair(i, 0.0));
	    }
	    
	    //Ordenar los pacientes segun el factor a utilizar
	    std::sort(list_patients.begin(), list_patients.end(), order_factor);
	    
	    for(int i = 0; i < (int)list_patients.size(); i++){
	      
		    std::vector < std::pair< int, int > > s;
		    
		    if(list_patients[i].first <= nEme){
			  float r = random_0_1();
			  if(r < p_eme)
				s = aux.asap_algorithm(list_patients[i].first + 1, patientsData);
			  else
				s = aux.jit_algorithm(list_patients[i].first + 1, patientsData);
		    }
		    else if(list_patients[i].first <= nEme + nPal){
			  float r = random_0_1();
			  if(r < p_pal)
				s = aux.asap_algorithm(list_patients[i].first + 1, patientsData);
			  else
				s = aux.jit_algorithm(list_patients[i].first + 1, patientsData);
		    }
		    else{
			  float r = random_0_1();
			  if(r < p_rad)
				s = aux.asap_algorithm(list_patients[i].first + 1, patientsData);
			  else
				s = aux.jit_algorithm(list_patients[i].first+ 1, patientsData);
		    }
		    
		    if(!s.empty()){
			  aux.insert_schedul(list_patients[i].first + 1, s, patientsData);
		    }
	    }
      }
      
      return 0;
}