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
	float fitness;
	
	//Scheduling
	std::vector<std::vector < std::pair< int, int > > > schedul;
	//Interrupciones de pacientes
	vector <int> interruptions_pat;
	//Tiempo disponible por maquina en cada dia <dia, tiempo>
	std::vector < std::list< pair<int, float> > > available_time;
	
	bool friday(int day){
		if((day + first_day_week - 1) % 5 == 4)
			return true;
		return false;
	}
	
	bool monday(int day){
		if((day + first_day_week - 1) % 5 == 1)
			return true;
		
		return false;
	}
	
	bool saturday(int day){
	  
		return false;
	}
	
	bool sunday(int day){
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
					available[aux_day % 5 - 1 ] = 1;
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
	
	void copy(Individual in){
		
		emergency = in.emergency;
		palliative = in.palliative;
		radical = in.radical;
		shifts = in.shifts;
		days = in.days;
		high_mach = in.high_mach;
		low_mach = in.low_mach;
		first_day_week = in.first_day_week;
		
		schedul.resize(in.schedul.size());
		for(int i = 0; i < (int)in.schedul.size(); i++){
			schedul[i].resize(in.schedul[i].size());
			for(int j = 0 ; j < (int)in.schedul[i].size(); j++){
				schedul[i][j].first = (in.schedul[i][j]).first;
				schedul[i][j].second = (in.schedul[i][j]).second;
			}
		}
		
		for(int i = 0; i < (int)in.interruptions_pat.size(); i++)
			interruptions_pat.push_back(in.interruptions_pat[i]);
		
		available_time.resize(in.available_time.size());
		for(int i = 0; i < (int)in.available_time.size(); i++)
			for(list<pair<int, float> >::iterator it = in.available_time[i].begin(); it != in.available_time[i].end(); ++it)
				available_time[i].push_back(make_pair((*it).first, (*it).second));
			
		fitness = in.fitness;
	}

	//Funcion para ordenar los tiempos de las maquinas
	static bool sort_time_machine(pair <int, float> p1, pair <int, float> p2){
		if(p1.first > p2.first)
			return true;
		return false;
	}
	  
	//Funcion que agrega tiempo a las maquinas
	//Principalmente utilizado para liberar un recurso
	void add_time_machine(int machine_i, int day, float time){
		for(std::list< std::pair<int, float> >::iterator it = available_time[machine_i].begin(); it != available_time[machine_i].end(); it++){
			if((*it).first == day){
				(*it).second += time;
				return ;
			}
		}
		available_time[machine_i].push_back(make_pair(day, time));
		available_time[machine_i].sort(sort_time_machine);
	}
	  
	//Retorna un vector de largo cero en caso de no poder insertar al paciente
        std::vector < std::pair< int, int > > try_insert(int pat, int machine, int initial_day, vector <PatientsData> patData){
		
                int count_days = 0;
                int day = initial_day;
                int category = patData[pat - 1].category;
                int sessions = patData[pat - 1].nSessions;
                bool max_interruptions;

                std::vector < std::pair< int, int > > assignments;
		
                do{
                        if(category != 1){
                                
                                if(sessions <= 5 && !fit_week(pat, day, patData)){
                                        day = next_monday(day);
                                }

                                if(category == 3 && friday(day)){
                                        day = next_monday(day);
                                }
                        }

                        //Probar si se puede asignar las sesiones
                        int interruptions = 0;
                        
                        int mach = 0;
                        int max_mach = low_mach;
                        
                        if(machine == 2){
                                mach = low_mach;
                                max_mach = low_mach + high_mach;
                        }
                        
                        do{
                                int aux_day = day;
                                interruptions = 0;
                                assignments.clear();
				
                                do{
                                        //Si la capacidad de la maquina es mayor a la necesaria para el tratamiento
                                        //Agregado para considerar mas tiempo en la primera sesion
                                        if(assignments.size() == 0){
                                                if(time_machine_i(mach, aux_day) >= patData[pat-1].first_session){
                                                        assignments.push_back(make_pair(aux_day, mach));
                                                        //cout << day << " " << mach << endl;
                                                        aux_day = next_day(aux_day, pat);
                                                }
                                                else{
                                                        interruptions++;
                                                        //cout << "Int: " << interruptions << " " << patData[pat - 1].interruptions << endl; 
							  if(patData[pat - 1].interruptions == 0){
								assignments.clear();
                                                                return assignments;
							  }
                                                        if(interruptions <= patData[pat - 1].interruptions){
                                                                assignments.clear();
                                                                return assignments;
                                                        }
                                                }
                                        }
                                        else{
                                                if(time_machine_i(mach, aux_day) >= patData[pat-1].duration_session){
                                                        assignments.push_back(make_pair(aux_day, mach));
                                                        //cout << day << " " << mach << endl;
                                                        aux_day = next_day(aux_day, pat);
                                                }
                                                else{
                                                        interruptions++;
							if(patData[pat - 1].interruptions == 0){
								assignments.clear();
                                                                return assignments;
							  }
                                                        //cout << "Int: " << interruptions << " " << patData[pat - 1].interruptions << endl; 
                                                        if(interruptions <= patData[pat - 1].interruptions){
                                                                assignments.clear();
                                                                return assignments;
                                                        }
                                                }
					}
					//cout << day << " " << pat << " " << patData.size() << " " << assignments.size() << " " << patData[pat - 1].nSessions << endl;
                               }while(assignments.size() < (unsigned int)patData[pat - 1].nSessions);
                                
                                mach ++;
                                
                        }while(mach < max_mach && !(assignments.size() == (unsigned int)patData[pat - 1].nSessions));
                        
                        //Se supero el numero maximo de interrupciones
                        max_interruptions = false;
                        count_days++;
                        /*if(count_days + patData[pat - 1].initialTreatmentDate + patData[pat - 1].nSessions - 1 > days){
                                assignments.clear();
                                cout << "entro aca"  << count_days << endl;
                                return assignments;
                                //cout << "Se supero el numero maximo de dias en el asap para el paciente " << pat << endl;
                                //cout << day << endl;
                                //show_info_patient(pat, patData);
                                //show_scheduling();
                                exit(0);
                        }
                        else */if(assignments.size() != (unsigned int)patData[pat - 1].nSessions){
                                assignments.clear();
                                return assignments;
                                day = next_day(count_days + patData[pat - 1].initialTreatmentDate, pat);
                                max_interruptions = true;
                        }
                        
                }while(max_interruptions);
                
                //showSchedulPatience(assignments);
                //cout << endl;
                
                //Asignar los tiempos calculados a las maquinas
                //Agregar el primer tiempo de ser necesario
                //for(unsigned int i = 0; i < assignments.size(); i++){
                //        assign_time_machine(assignments[i].second, assignments[i].first, patData[pat -1].duration_session);
                //}

                return assignments;
        }
	
	void set_params(float nshifts, int ndays, int l_mach, int h_mach, int first_day){
		this->emergency = 0;
		this->palliative = 0;
		this->radical = 0;
		this->shifts = nshifts;
		this->days = ndays;
		this->high_mach = h_mach;
		this->low_mach = l_mach;
		this->first_day_week = first_day;
		this->fitness = 0.0;
	}
	
	float get_fitness(){
		return fitness;
	}
	
	void update_fitness(vector<PatientsData> list_pat){
		fitness = 0.0;
		
		for(int i = 0; i < (int)schedul.size(); i++){
			if((int)schedul[i].size() > 0){
				if(schedul[i][0].first > list_pat[i].initialTreatmentDate){
					fitness += schedul[i][0].first - list_pat[i].initialTreatmentDate;
				}
				else if(schedul[i][0].first < list_pat[i].initialTreatmentDate){
					cout << "PROBLEMA" << endl;
				}
			}
		}
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
				(*it).second -= time;
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
	
	void show_vector(int e, int p, int r, vector<PatientsData> patData){
		cout << (int)schedul.size() << endl;
	  
		for(int i = 0; i < (int)schedul.size(); i++){
			cout << (int)schedul[i].size() << " " ;
			for(int j = 0; j < (int)schedul[i].size(); j++){
				cout << schedul[i][j].first << " " ;
				cout << schedul[i][j].second << " " ;
			}
			cout << endl;
		}
		
		cout << e << " " << p << " " << r << endl;
		
		for(int i = 0; i < e + p + r; i++){
		      cout << patData[i].id << " ";
		      cout << patData[i].category << " ";
		      cout << patData[i].initialTreatmentDate << " ";
		      cout << patData[i].finalTreatmentDate << " ";
		      cout << patData[i].days_delay << " ";
		      cout << patData[i].nSessions << " ";
		      cout << patData[i].interruptions << " ";
		      cout << patData[i].machine << " ";
		      cout << patData[i].duration_session << endl;
		}
	}
	
	bool shift_patient(int start, int end, int pat, vector<PatientsData> patData){
	  
		if(patData[pat - 1].initialTreatmentDate == schedul[patData[pat -1].id - 1][0].first)
			return false;
		
		//Descuento el número de pacientes agendados
		if(patData[pat - 1].category == 1)
			emergency--;
		else if(patData[pat - 1].category == 2)
			palliative--;
		else
			radical--;
		
		//Libero recursos
		for(int i = 0; i < (int)schedul[pat - 1].size(); i++){
			if(i == 0)
				add_time_machine(schedul[pat - 1][i].second, schedul[pat - 1][i].first, patData[pat - 1].first_session);
			else
				add_time_machine(schedul[pat - 1][i].second, schedul[pat - 1][i].first, patData[pat - 1].duration_session);
		}
		
		schedul[pat - 1].clear();
		
		int d = 0;
		if(start < patData[pat - 1].initialTreatmentDate)
		    d = patData[pat - 1].initialTreatmentDate;
		else
		    d = start;
		
		std::vector < std::pair< int, int > > new_schedul;
		for( ; d < patData[pat - 1].finalTreatmentDate && d < end  ; d++){
		      new_schedul = try_insert(pat, patData[pat - 1].machine, d, patData);
		      
		      if(new_schedul.size() != 0){
			      //Se puede insertar en la planificacion
			      insert_schedul(pat, new_schedul, patData);
			      //patData = erase_patient(patData[pat - 1].id, patData);
			      return true;
		      }
		      
		      new_schedul.clear();
		      
		 }
		
		return false;
	}
	
	bool swap_schedul_schedul(int start, int end, int pat_schedul1, int pat_schedul2, vector<PatientsData> patData){
	  
		//Descuento el número de pacientes agendados
		if(patData[pat_schedul1 - 1].category == 1)
			emergency--;
		else if(patData[pat_schedul1 - 1].category == 2)
			palliative--;
		else
			radical--;
		
		if(patData[pat_schedul2 - 1].category == 1)
			emergency--;
		else if(patData[pat_schedul2 - 1].category == 2)
			palliative--;
		else
			radical--;
	  
		//Libero recursos
		//Paciente 1
		
		if(patData[pat_schedul1-1].initialTreatmentDate > end)
			return false;
		if(patData[pat_schedul2-1].initialTreatmentDate > end)
			return false;
		if(pat_schedul1 == pat_schedul2)
			return false;
		
		for(int i = 0; i < (int)schedul[pat_schedul1 - 1].size(); i++){
			if(i == 0)
				add_time_machine(schedul[pat_schedul1 - 1][i].second, schedul[pat_schedul1 - 1][i].first, patData[pat_schedul1 - 1].first_session);
			else
				add_time_machine(schedul[pat_schedul1 - 1][i].second, schedul[pat_schedul1 - 1][i].first, patData[pat_schedul1 - 1].duration_session);
		}
		schedul[pat_schedul1 - 1].clear();
		
		//Paciente 2
		for(int i = 0; i < (int)schedul[pat_schedul2 - 1].size(); i++){
			if(i == 0)
				add_time_machine(schedul[pat_schedul2 - 1][i].second, schedul[pat_schedul2 - 1][i].first, patData[pat_schedul2 - 1].first_session);
			else
				add_time_machine(schedul[pat_schedul2 - 1][i].second, schedul[pat_schedul2 - 1][i].first, patData[pat_schedul2 - 1].duration_session);
		}
		schedul[pat_schedul2 - 1].clear();
		
		bool pat2 = false;
		int d2 = 0;
		if(patData[pat_schedul2 - 1].initialTreatmentDate < start)
			d2 = start;
		else
			d2 = patData[pat_schedul2 - 1].initialTreatmentDate;
		
		for( ; d2 < patData[pat_schedul2 - 1].finalTreatmentDate && d2 < end  ; d2++){
		  
		      std::vector < std::pair< int, int > > new_schedul = try_insert(pat_schedul2, patData[pat_schedul2 - 1].machine, d2, patData);
		      
		      if(new_schedul.size() != 0){
			      //Se puede insertar en la planificacion
			      insert_schedul(pat_schedul2, new_schedul, patData);
			      pat2 = true;
			      break;
		      }
		 }
		 
		 if(pat2 == false)
			return false;
		 
		 bool pat1 = false;
		 int d1 = 0;
		 
		if(patData[pat_schedul1 - 1].initialTreatmentDate < start)
			d1 = start;
		else
			d1 = patData[pat_schedul1 - 1].initialTreatmentDate;
		
		  for( ; d1 < patData[pat_schedul1 - 1].finalTreatmentDate && d1 < end  ; d1++){
		      std::vector < std::pair< int, int > > new_schedul = try_insert(pat_schedul1, patData[pat_schedul1 - 1].machine, d1, patData);
		      if(new_schedul.size() != 0){
			      //Se puede insertar en la planificacion
			      insert_schedul(pat_schedul1, new_schedul, patData);
			      pat1 = true;
			      break;
		      }
		 }
		 
		return (pat1 && pat2);
	}
	
	bool insert_list_to_schedul(int start, int end, int pat_list, vector<PatientsData> &patients_waiting, vector <PatientsData> &patients_list, vector <PatientsData> patData){
	  
		//int firstday_aux = schedul[pat_schedul - 1][0].first;
		
		int d;
		if(patData[pat_list - 1].initialTreatmentDate < start)
			d = start;
		else
			d = patData[pat_list - 1].initialTreatmentDate;
		
		 std::vector < std::pair< int, int > > new_schedul;
		 for( ; d < patData[pat_list - 1].finalTreatmentDate && d < end  ; d++){
		      new_schedul = try_insert(pat_list, patData[pat_list - 1].machine, d, patData);
		      
		      if(new_schedul.size() != 0){
			      //Se puede insertar en la planificacion
			      insert_schedul(pat_list, new_schedul, patData);
			      patients_waiting = erase_patient(pat_list, patients_waiting);
			      patients_list.push_back(patData[pat_list - 1]);
			      return true;
		      }
		      
		      new_schedul.clear();
		      
		 }
		 
		 return false;
	}
	
	bool swap_list_schedul(int start, int end, int pat_waiting, vector<PatientsData> &patients_waiting, int pat_schedul, vector <PatientsData> &patients_scheduled, vector <PatientsData> patData){
		
		//int firstday_aux = schedul[pat_schedul - 1][0].first;
		//Descuento el número de pacientes agendados
		if(patData[pat_schedul - 1].category == 1)
			emergency--;
		else if(patData[pat_schedul - 1].category == 2)
			palliative--;
		else
			radical--;
		
		//Libero recursos
		for(int i = 0; i < (int)schedul[pat_schedul - 1].size(); i++){
			if(i == 0)
				add_time_machine(schedul[pat_schedul - 1][i].second, schedul[pat_schedul - 1][i].first, patData[pat_schedul - 1].first_session);
			else
				add_time_machine(schedul[pat_schedul - 1][i].second, schedul[pat_schedul - 1][i].first, patData[pat_schedul - 1].duration_session);
		}
		schedul[pat_schedul - 1].clear();
		
		int d = 0;
		if(patData[pat_waiting - 1].initialTreatmentDate < start)
			d = start;
		else
			d = patData[pat_waiting - 1].initialTreatmentDate;
		 
		 std::vector < std::pair< int, int > > new_schedul;
		 for( ; d <= patData[pat_waiting - 1].finalTreatmentDate && d <= end  ; d++){
		   
		      new_schedul = try_insert(pat_waiting, patData[pat_waiting - 1].machine, d, patData);
		      
		      if(new_schedul.size() != 0){
			      //Se puede insertar en la planificacion
			      insert_schedul(pat_waiting, new_schedul, patData);
			      
			      PatientsData aux_waiting = patData[pat_waiting - 1];
			      PatientsData aux_scheduled = patData[pat_schedul - 1];
			      
			      patients_waiting = erase_patient(pat_waiting, patients_waiting);
			      patients_waiting.push_back(aux_scheduled);
			      
			      patients_scheduled = erase_patient(pat_schedul, patients_scheduled);
			      patients_scheduled.push_back(aux_waiting);
			      
			      return true;
		      }
		      
		      new_schedul.clear();
		      
		 }
		 //Si llego a este punto, significa que no pudo ingresarlo al sistema, por lo que debe restaurarlo
		/* new_schedul = try_insert(patients_list[pat_schedul - 1].id, patients_list[pat_list - 1].machine, firstday_aux, patData);
		 
		 if(new_schedul.size() == 0){
			cout << "PROBLEMA" << endl;
			exit(0);
		 }*/
		 
		 return false;
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
