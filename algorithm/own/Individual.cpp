#include <list>
#include <vector>
#include "PatientsData.cpp"
#include <iostream>
#include <cstdlib>

using namespace std;

class Individual{

	//Numero de pacientes urgentes
	int emergency;
	//Numero de pacientes paliativos
	int palliative;
	//Numero de pacientes radicales
	int radical;
	//Cantidad de tiempo/bloques por dia
	float shifts;
	//Numero de dias a planificar
	int days;
	//Maquinas de alta energia
	int high_mach;
	//Maquinas de baja energia
	int low_mach;
	//Dia con el que comienza la planificacion
	int first_day_week;

	//Scheduling
	std::vector<std::vector < std::pair< int, int > > > schedul;
	//Interrupciones de pacientes
	vector <int> interruptions_pat;
	//Tiempo disponible por maquina en cada dia <dia, tiempo>
	std::vector < std::list< pair<int, float> > > available_time;
	
	bool friday(int day){
		if((day + first_day_week - 1) % 7 == 5)
			return true;
		return false;
	}
	
	bool monday(int day){
		if((day + first_day_week - 1) % 7 == 1)
			return true;
		
		return false;
	}
	
	bool saturday(int day){
		if((day + first_day_week - 1 )% 7 == 6)
			return true;
		
		return false;
	}
	
	bool sunday(int day){
		if((day + first_day_week - 1) % 7 == 0)
			return true;
		
		return false;
	}
	
	int next_monday(int day){
		do{
			day ++;
			
		}while(!monday(day));
		
		return day;
	}
	
	int next_day(int day, int pat){
		for(int i = day + 1; i < days; i++)
			if(!saturday(i) && !sunday(i))
				return i;
		return 0;
	}
	
	int one_day_before(int day, int pat){
		for(int i = day -1; i >= 0; i--)
			if(!saturday(i) && !sunday(i))
				return i;
		return 0;
	}
	
	//Funcion que dice si se puede asignar a un paciente en una semana con respecto al día
	bool fit_week(int pat, int day, vector<PatientsData> patData){
		
		int i = 0;
		int max_mach = low_mach;
		std::vector<int> available (5, 0);
		
		if(patData[pat - 1].machine == 2){
			i = low_mach;
			max_mach = low_mach + high_mach;
		}
		
		for(; i < max_mach; i++){
			
			int aux_day = day;
			
			for(int j = 0; j < 5; j++)
				available[j] = 0;
			
			while(!monday(aux_day) && aux_day > 0){
				aux_day --;
			}
			
			while(true){
				if(time_machine_i(i, aux_day) >= patData[pat - 1].duration_session){
					available[aux_day % 7 - 1 ] = 1;
				}
			
				if(friday(aux_day))
					break;
				
				aux_day ++;
			}
		}
  
		for(int j = 0; j < 6 - patData[pat-1].nSessions; j++){
			int sum = 0;
			int interruptions = 0;
			for(int k = j; k < j + patData[pat-1].nSessions; k++){
				sum += available[k];
				if(available[k] == 0)
					interruptions++;
				
				if(sum == patData[pat-1].nSessions){
					if(interruptions <= patData[pat -1].interruptions)
						return true;
					break;
				}
			}
		}
		return false;
	}
	
	//Devuelve el tiempo disponible de la maquina en cierto dia
	float time_machine_i(int machine_i, int day){
	  
		for(std::list< std::pair<int, float> >::iterator it = available_time[machine_i].begin(); it != available_time[machine_i].end(); it++){
			if((*it).first == day){
				return (*it).second;
			}
		}
		
		return 0.0;
	}
	
	public:
	
	
	
	void set_params(float nshifts, int ndays, int l_mach, int h_mach, int first_day){
		this->emergency = 0;
		this->palliative = 0;
		this->radical = 0;
		this->shifts = nshifts;
		this->days = ndays;
		this->high_mach = h_mach;
		this->low_mach = l_mach;
		this->first_day_week = first_day;
	}
	
	//Iniciar las maquinas con los tiempos
	void set_time_machines(vector<int> time_machines, int days, int machines){

		//machines corresponde al numero de maquinas disponible
		for(int i = 0; i < machines; i++){
			std::list< pair <int, float> > aux;
			for(int j = 1; j <= days; j++)
				aux.push_back(make_pair(j, time_machines[i]));

			available_time.push_back(aux);
		}
	}
	
	void insert_schedul(int pat, std::vector < std::pair< int, int > > s, vector<PatientsData> patData){
	  
		if(patData[pat - 1].category == 1)
			emergency++;
		else if(patData[pat - 1].category == 2)
			palliative++;
		else
			radical++;
		
		if(pat > (int)schedul.size())
		      schedul.resize(pat);
		
		assign_time_machine(s[0].second, s[0].first, patData[pat - 1].first_session);
		schedul[pat -1].push_back(make_pair(s[0].first, s[0].second));
		for(unsigned int i = 1; i < s.size(); i++){
			//cout << time_machine_i(s[i].second, s[i].first) << endl;
			assign_time_machine(s[i].second, s[i].first, patData[pat - 1].duration_session);
			schedul[pat -1].push_back(make_pair(s[i].first, s[i].second));
		}
	}
	
	//Asignar tiempo a una maquina en cierto dia especifico
	  void assign_time_machine(int machine_i, int day, float time){
		for(std::list< std::pair<int, float> >::iterator it = available_time[machine_i].begin(); it != available_time[machine_i].end(); it++){
			if((*it).first == day){
				//cout << "Desconto: " << time << endl;
				if((*it).second == 0.0){
					available_time[machine_i].erase(it);
				}
				else if((*it).second < 0.0){
					cout << "Error. Se sobrepaso el tiempo disponible de la maquina" << endl;
					exit(0);
				}
				return ;
			}
		}
	}
	
	void show_vector(){
		cout << (int)schedul.size() << endl;
	  
		for(int i = 0; i < (int)schedul.size(); i++){
			cout << (int)schedul[i].size() << " " ;
			for(int j = 0; j < (int)schedul[i].size(); j++){
				cout << schedul[i][j].first << " " ;
				cout << schedul[i][j].second << " " ;
			}
			cout << endl;
		}
	}
	
	int getEmergency(){
		return emergency;
	}
	
	int getPalliative(){
		return palliative;
	}
	
	int getRadical(){
		return radical;
	}
};