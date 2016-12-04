/**
	Database Editor
	
	@author Kshamina Ghelani
	@author Supriya Gadigone
**/

#include <math.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <string.h>
#include "queue.c"


void promptUser();
void initialize(void); 

/**
 Main 
 **/
int main(int argc, char* argv[])						
{	
	
	initialize();
	int msqid; //messageq id returned by the queue
	int msgflg = IPC_CREAT | 0666; // message flag
    key_t key; // key to be passed to msgget()
    size_t buf_length; //buffer length

	promptUser();
	
	return 0;
}

/**
	Prompts the user for their account number, PIN and amount of funds to add to acocunt.
*/
void promptUser()
{
	char accountNumber[5] = "\0";
	char PIN[3] = "\0";
	float amountOfFunds;

	printf("\nPlease enter an account number (5 digits) \n");	
	scanf("%s", accountNumber);	
	printf("****%s", accountNumber);

	printf("\nPlease enter your PIN number (3 digits) \n");	
	scanf("%s", PIN);	
	
	printf("\nPlease enter amount of funds (precision = 2 decimals)\n");	
	scanf("%f", &amountOfFunds);
	
	enqueue(1, "fdsd", accountNumber, PIN, amountOfFunds);
	printQueue(); 
}





