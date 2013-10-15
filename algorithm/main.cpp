#include <iostream>
#include <stdlib.h>
#include "PatientsData.cpp"
#include "Individual.cpp"

#define FIRST 5.0

using namespace std; 

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
      
      //Variable que almacenara la mejor solucion global del algoritmo
      Individual global;
      
      
      
      return 0;
}