/**
	Database Editor
	
	@author Kshamina Ghelani
	@author Supriya Gadigone
**/

#include <sys/ipc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
	int accountNumber, PIN;
	float amountOfFunds;
	printf("\nPlease enter an account number\n");	
	scanf("%c", &accountNumber);		
	printf("\nPlease enter your PIN number\n");	
	scanf("%c", &PIN);
	printf("\nPlease enter amount of funds\n");	
	scanf("%f", &amountOfFunds);
	
}





