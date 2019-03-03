#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

//global variable
//specifying size of arrays needed
int arraySize = 300;

//function that initializes a given int array
void reset(int array[], int size) {
	int i = 0;
	while(i < size) {
		array[i] = 0;
		i++;
	}
}

//function that converts a string into an array of integers
void getNumber(char myArray[], int data[]) {
	
	reset(data, arraySize);
	int i = 0;
	int k = 0;
	int number = 0;
	bool negative_present = false;

	//interate until end of line/end of myArray
	while(myArray[i] != 0) {		
		if(myArray[i] != 32) {								//32 is ascii value for 'SPACE'
			if(myArray[i] >= '0' && myArray[i] <= '9') {		
				number = (number*10) + (myArray[i]-48);
			}

			//if char is '-' (negative)
			if(myArray[i] == '-') {
				negative_present = true;
			}

			if(myArray[i+1] < '0' || myArray[i+1] > '9') {
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
		}
		i++;
	}
}

//converts standard input from cin to int array "input" 
void processInputToArray(int input[]) {
	string user_input_str;
	getline(cin, user_input_str);
	char user_input_char_array[user_input_str.size()+1];
	user_input_str.copy(user_input_char_array, user_input_str.size()+1);
	getNumber(user_input_char_array, input);
}

//prepares the current set of inputs to be evaluated
int loadInput(int input[], int indx, int currentLoad[]) {
	if(input[indx] == 0 && input[indx+1] == 0) {
		//no more inputs left to process, terminate program
		return -1;											
	}
	else {
		reset(currentLoad, arraySize);
		int i = 0;
		while(input[indx] != 0) {
			currentLoad[i] = input[indx];
			i++;
			indx++;
		}
	}
	//return indx for beginning of next load
	return indx + 1;
}

//where the magic happens
int main (int argc, char* argv[]) {
	
	//access DIMACS file
	FILE * pFile;
	char myArray[arraySize];
	pFile = fopen (argv[1], "r");

	int indx = 0;
	int input[arraySize];
	int currentLoad[arraySize];
	
	//convert standard input from cin to int array "input"
	processInputToArray(input);

	while(1) {
		
		//update indx
		indx = loadInput(input, indx, currentLoad);
		if(indx == -1) {
			//if no more inputs to process, terminate program 
			return 0;
		}

		rewind(pFile);
		int line_number = 0;
		int num_vars = 0;
		int num_clauses = 0;
		
		int var_state[arraySize];
		int data[arraySize];
		
		int truth_value = -1;
		int clause_ouput = -1;
		int var_output = -1;

		while(fgets(myArray,arraySize,pFile) != NULL && truth_value == -1) {
			
			//convert string to int array "data"
			int data[arraySize] = {0};
			getNumber(myArray, data);
			
			//keep track of current line number
			line_number++;
			
			//the first line in txt file sets numVars and numClauses accordingly
			if(line_number == 1) {
				num_vars = data[0];
				num_clauses = data[1];

				//all variables are set to false by default
				int i = 1;
				while(i < num_vars+1) {
					var_state[i] = 0;
					i++;
				}

				//if variable is positive, set to true
				i = 0;
				while(currentLoad[i] != 0) {
					if(currentLoad[i] > 0) {
						var_state[currentLoad[i]] = 1;
					}
					i++;
				}
			}


			//after the first line of txt file
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
					cout << "false" << endl;
					truth_value = 0;
				}
			}
		}
		//if not final output is not false after iterating through DIMACS file
		//then final output is true
		if(truth_value != 0) {
			cout << "true" << endl;
			truth_value = 1;
		}
	}
	return 0;
}