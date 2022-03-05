#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // malloc & free
#include <stdint.h> // use guaranteed 64-bit integers
#include "tokenizer.h" // Create header file and reference that
#include "memory.h" // built-in functions to read and write to a specific file

int32_t* reg; // Array of 32 32-bit registers

void init_regs();
bool interpret(char* instr);
void write_read_demo();
char* tokenizeHelper(char **token);
bool operation(char** tokens, char* compare);
int option(char** tokens);
void decToHex(int n);

/**
 * Initialize register array for usage.
 * Malloc space for each 32-bit register then initializes each register to 0.
 * Do not alter this function!
 */
void init_regs(){
	int reg_amount = 32;
	reg = malloc(reg_amount * sizeof(int32_t)); // 32 * 4 bytes
	for(int i = 0; i < 32; i++)
		reg[i] = i;
}



/**
 * Fill out this function and use it to read interpret user input to execute RV64 instructions.
 * You may expect that a single, properly formatted RISC-V instruction string will be passed
 * as a parameter to this function.
 */
// ADD X6 X0 X29
bool interpret(char* instr){
		char delim[1]=","; 						   //delimeter used.
		char** token = tokenize(instr, delim[0]);  //tokenize instr
		char** tokenTemp = token;  	     //This is a copy used to find that is the operation, since the original (char** token) will be altered.		    
		char* regs[20]; 				 //registers entered by user wil be stored here.
		char* temp;					 	 //Will point to either  add or addi
		char add[3] = "ADD"; 			 //add and addi are string that will be compared to define operation 
		char addi[4] = "ADDI";
		char *str = NULL;			 	 //str, str1 and str2 are used in SW and LW to parse string
		char *str1 = NULL;	
		char *str2 = NULL;
		char* mem_file = "mem.txt";		//to read from "mem.txt" file
		int destReg;					//destination register.
		int reg1;						//value of register
		int reg2;						//value of register
		int i =0;
		int offset;						//offset, baseRegister, and newaddress are used in SW and LW to complete operation
		int baseRegister;
		int32_t newaddress;									
		bool op; 						//returns true if it is either ADD or ADDI
		int opSwLd;						//returns 1 if it is SW or 2 if it is LW
	
		 //perfom operation:
		if((op = operation(tokenTemp, (temp = add)))){//ADD
			token++;								//go to the next token 
			while(*token != NULL){
			printf(" token is %s \n", *token );
			regs[i] =  tokenizeHelper(token);		//this will help us to separate any characters and pass only numebers.
			i++;
			token++;
			}
			destReg = atoi(regs[0]);				//parsing to an int
			reg1 = atoi(regs[1]);
			reg2 = atoi(regs[2]);
			reg[destReg] = reg[reg1] + reg[reg2];		//Do operation 
		}
		else if((op = operation(tokenTemp,(temp = addi)))){ //ADDI
			token++;								//go to the next token 
			while(*token != NULL){
			regs[i] =  tokenizeHelper(token);		//this will help us to separate any characters and pass only numebers.
			i++;
			token++;
			}
			destReg = atoi(regs[0]);		//parsing to an int
			reg1 = atoi(regs[1]);
			reg2 = atoi(regs[2]);
			reg[destReg] = reg[reg1] + reg2;		//Do operation 
		}
		else if ((opSwLd= option(token))==1){  //SW
			str = strtok(token[1], "X"); //tokenize destination, offset, and base register
			str1 = strtok(token[2], "(X)");
			str2 = strtok(NULL, "(X)");
			newaddress = reg[atoi(str)];
			offset = atoi(str1);
			baseRegister = atoi(str2);
			destReg = offset + baseRegister;
			write_address(newaddress, destReg, mem_file);
		}	
		else if((opSwLd = option(token))==2){ //LW
			printf("It is Lw\n");		//tokenize destination, offset, and base register
			str = strtok(token[1], "X");
			str1 = strtok(token[2], "(X)");
			str2 = strtok(NULL, "(X)");
			destReg	= atoi(str);
			offset = atoi(str1);
			baseRegister = atoi(str2);
			newaddress = reg[baseRegister]+offset;
			int32_t read = read_address(newaddress, mem_file);
			decToHex(read);
			reg[destReg] = read;
		}
		else{
			return false;
		}
		
	return true;
}
/*This function will return only a char* of digits only*/
char* tokenizeHelper(char **token){
	char *pointer = (char *)malloc((3) * sizeof(char));
   char *temp = token[0];
   char* result = pointer;
   int i =0;
   while(*temp != NULL){
	   if(isdigit(*temp)){
      *pointer = *temp;
	  pointer++;
	   }  
      temp++;
      i++;
   }
   return result;
   
}
/*returns 1 if it is SW or 2 if it is LW*/
int option(char** tokens){
	char sw[2] = "SW";
	char lw[2] = "LW";
	char *temp = tokens[0];
	int i = 0;
	int answer = 0;
	int ascii = (int)temp[i];

	for( i = 0; temp[i] != '\0'; i++){ //true if SW is 3
		if(temp[i] == sw[i] && ascii == 83){
				answer = 1;
				return 1;
		}
		else{
			answer = 0;
		}
	}
	if(answer == 0){
		for(i = 0; temp!= '\0'; i++){
			if(temp[i] == lw[i] && ascii == 76){
				answer = 2;
				return 2;
			}
			else{
				answer =0;
			}
		}
	}
	return answer;
}
/*returns true if it is either ADD or ADDI*/
bool operation(char** tokens, char* compare){
	int answer = 0;
	char *tempToken = tokens[0];
	int i = 0 ;
	int ascii = (int)tempToken[i];

	for( i =0; tempToken[i] != '\0'; i++){
		if(tempToken[i] == compare[i] ){
			answer = 1;
		}
		else{
			answer = 0;
			return false;
		}
	}
	if(answer == 1){
		//printf("it is true \n");
		return true;
	}
}
/*This function converts from decimal to hexadecimal*/
void decToHex(int n){
	char hexaDeciNum[30];
	int rem, i = 0;
	
	while(n != 0){
		rem = n % 16;
		
		if(rem < 10){
			rem = rem +48;
		}
		else{
			rem = rem + 55;
		}
		hexaDeciNum[i] = rem;
		i++;
		n = n/16;
	}
	printf("\n print hex converstion\n");
	for(int j = i -1; j >=0; j--){
	
		printf("%c", hexaDeciNum[j]);
	}
}


/**
 * Simple demo program to show the usage of read_address() and write_address() found in memory.c
 * Before and after running this program, look at mem.txt to see how the values change.
 * Feel free to change "data_to_write" and "address" variables to see how these affect mem.txt
 * Use 0x before an int in C to hardcode it as text, but you may enter base 10 as you see fit.
 */
void write_read_demo(){
	int32_t data_to_write = 0xFFF; // equal to 4095
	int32_t address = 0x98; // equal to 152
	char* mem_file = "mem.txt";

	// Write 4095 (or "0000000 00000FFF") in the 20th address (address 152 == 0x98)
	int32_t write = write_address(data_to_write, address, mem_file);
	if(write == (int32_t) NULL)
		printf("ERROR: Unsucessful write to address %0X\n", 0x40);
	int32_t read = read_address(address, mem_file);

	printf("Read address %lu (0x%lX): %lu (0x%lX)\n", address, address, read, read); // %lu -> format as an long-unsigned
}

/**
 * Prints all 32 registers in column-format
 */
void print_regs(){
	int col_size = 10;
	for(int i = 0; i < 8; i++){
		printf("X%02i:%.*lld", i, col_size, (long long int) reg[i]);
		printf(" X%02i:%.*lld", i+8, col_size, (long long int) reg[i+8]);
		printf(" X%02i:%.*lld", i+16, col_size, (long long int) reg[i+16]);
		printf(" X%02i:%.*lld\n", i+24, col_size, (long long int) reg[i+24]);
	}
}

/**
 * Your code goes in the main
 *
 */
int main(){
	// Do not write any code between init_regs
	init_regs(); // DO NOT REMOVE THIS LINE

	print_regs();

	// Below is a sample program to a write-read. Overwrite this with your own code.
	//write_read_demo();

	printf(" RV32 Interpreter.\nType RV32 instructions. Use upper-case letters and comma as a delimiter.\nEnter 'EOF' character to end program\n");


	char* instruction = malloc(1000 * sizeof(char));
	bool is_null = false;
	// fgets() returns null if EOF is reached.
	is_null = fgets(instruction, 1000, stdin) == NULL;
	while(!is_null){
		interpret(instruction);
		printf("\n");
		print_regs();
		printf("\n");

		is_null = fgets(instruction, 1000, stdin) == NULL;
	}

	printf("Good bye!\n");

	return 0;
}
