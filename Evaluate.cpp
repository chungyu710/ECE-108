#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

void displayVarState(int array[], int size) {
	int i = 1;
	while(i < size+1) {
		cout << i << ": " << array[i] << endl;
		i++;
	}
}

void reset(int array[], int size) {
	int i = 0;
	while(i < size) {
		array[i] = 0;
		i++;
	}
}

//this function converts a string into an array of integers
void getNumber(char myArray[], int data[]) {
	reset(data, 100);
	//interate until end of line/end of myArray
	int i = 0;
	int k = 0;
	int number = 0;
	bool negative_present = false;
	while(myArray[i] != 0) {
		if(myArray[i] >= 48 && myArray[i] <= 57) {		
			number = (number*10) + (myArray[i]-48);
		}

		if(myArray[i] == 45) {		//if char is '-' (negative)
			negative_present = true;
		}

		if(number != 0 && (myArray[i+1] < 48 || myArray[i+1] > 57)) {
			if(negative_present == true) {
				data[k] = number*-1;
			}
			else {
				data[k] = number;
			}

			number = 0;
			negative_present = false;
			k++;
		}
		i++;
	}
}

int main (int argc, char* argv[]) {
	
	//access txt file
	FILE * pFile;
	char myArray[100];
	pFile = fopen (argv[1], "r");
	


	//fgets(myArray,100,pFile);			//store current line of txt into myArray
	int line_number = 0;
	int num_vars = 0;
	int num_clauses = 0;
	int var_state[100];
	int data[100];
	int input[100];

	int clause_ouput = -1;
	int var_output = -1;

	while(fgets(myArray,100,pFile) != NULL) {
		int data[100] = {0};
		getNumber(myArray, data);
		
		line_number++;
		//the first line in txt file
		//sets numVars and numClauses accordingly
		if(line_number == 1) {
			num_vars = data[0];
			num_clauses = data[1];
			cout << "num_vars: " << num_vars << endl;
			cout << "num_clauses: " << num_clauses << endl;
			
			int i = 1;
			while(i < num_vars+1) {
				var_state[i] = 1;
				i++;
			}

			string user_input_str;
			getline(cin, user_input_str);
			char user_input_char_array[user_input_str.size()+1];
			user_input_str.copy(user_input_char_array, user_input_str.size()+1);
			getNumber(user_input_char_array, input);


			i = 0;
			while(input[i] != 0) {
				if(input[i] < 0) {
					var_state[input[i]*-1] = 0;
				}
				i++;
			}
			displayVarState(var_state, num_vars);
		}
		//not the first line of txt file
		else {
			//iterates through current clause
			var_output = false;
			int i = 0;
			while(data[i] != 0 && var_output == false) {
				if(data[i] < 0) {
					if(!var_state[data[i]*-1] == true)
					{
						var_output = true;
					}
				}
				else if (data[i] > 0) {
					if(var_state[data[i]] == true) {
						var_output = true;
					}
				}
				i++;
			}

			//if one of the clauses evaluates to false, the final output is false
			if(var_output == false) {
				cout << "False" << endl;
				return 0;
			}
		}

	}
	cout << "True" << endl;
	return 1;
}
