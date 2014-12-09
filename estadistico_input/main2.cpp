 #include <iostream>
#include <stdlib.h>
#include <vector>
#define FIRST 5.0

using namespace std;

int main()
{
	int days;
	cin >> days;
	
	int time;
	cin >> time;
	
	int type_machines = 2;
	
	int type_machine_1;
	cin >> type_machine_1;
	
	int type_machine_2;
	cin >> type_machine_2;
	
	int time_sessions[4][2];
	
	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 2; j++)
			time_sessions[i][j] = 0;
	
	for(int i = 0; i < days; i++){
		int patients;
		cin >> patients;
		//cout << patients << endl;
		for(int j = 0; j < patients; j++){
			int category;
			cin >> category;
			//cout << category << " ";
			
			int release;
			cin >> release;
			//cout << release << " ";
			
			int due;
			cin >> due;
			//cout << due << " ";
			
			int sessions;
			cin >> sessions;
			//cout << sessions << " ";
			
			int interruptions;
			cin >> interruptions;
			//cout << interruptions << " ";
			
			int mach_type;
			cin >> mach_type;
			//cout << mach_type << " ";
			
			int first_time = 5;
			
			int time;
			cin >> time;
			//cout << time << endl;
			
			time_sessions[category - 1][mach_type - 1]+= time * sessions + first_time;
		}
	}
	
	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 2; j++)
			cout << time_sessions[i][j] << " ";
	cout << endl;
	return 0;
}
