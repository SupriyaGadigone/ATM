/**
	ATM: Prompts customer for account number and PIN and allows them to check amount of funds or make a withdrawal 
	
	@author Kshamina Ghelani
	@author Supriya Gadigone
	
**/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdbool.h>
#include "queue.c" 
#define MSGSZ 128

typedef struct msg {
   long mtype; //type of the msg
   char mtext[MSGSZ]; //size of the msg
 } msg; 

void* atm(void* arg);

int main() {
	pthread_t atMachine; 
	pthread_create(&atMachine, NULL, atm, (void*)NULL);
	pthread_join(atMachine, NULL); 
		
	return 0; 
}
/*
 * Function used to send and receive messages from server
 */
void* atm(void* arg) {
	char accountNumber[5] = {'\0'};
	char PIN[3] = {'\0'};
	int numLogin = 0;
	bool valid = false;
		
	int serverID;
	int atmID; 
	int msgflg = IPC_CREAT | 0666;
	int input;
	char amount[100];
	key_t serverKey = 1111;
	key_t atmKey = 5678;

	msg *message;
	message = (msg *)malloc((unsigned)(sizeof(msg) - sizeof message->mtext + MSGSZ));
	
	while(1) {
		while(valid == false && numLogin < 3) {
			printf("\nPlease enter an account number (5 digits) \n");	
		    scanf("%s", accountNumber);	
		    
			printf("\nPlease enter your PIN number (3 digits) \n");	
			scanf("%s", PIN);
			
			strcpy(message->mtext,accountNumber);
			strcat(message->mtext,PIN);
			
			message->mtype = 2; // Customer login
			if((serverID = msgget(serverKey, msgflg)) < 0) {
				perror("msgget"); 
				exit(1); 
			}
			
			if((atmID = msgget(atmKey, msgflg)) < 0) { 
				perror("msgget"); 
				exit(1); 
			}
			
			if(msgsnd(serverID, message, 1000, 0) == -1) { // send user inputted account # and PIN to dbServer
				perror("msgsnd");  
			}
			
			if(msgrcv(atmID, message, 1000, 3, 0) < 0) // receive OK or NOT OK message
			{
				perror("msrcv");
	
			}
	
			if(strcmp(message->mtext, "OK") == 0) { // if OK, continue to further options
				valid = true; 
				printf("\nPress 1 to display funds and 2 to withdraw \n");	
				scanf("%d", &input);
				switch (input){
					
				case 1:		// Display funds
					message->mtype = 4;
					strcpy(message->mtext,accountNumber);
					if(msgsnd(serverID, message, 1000, 0) == -1) 
					{
						perror("msgsnd");  
			        }
					
					if(msgrcv(atmID, message, 1000, 5, 0) < 0) // receive OK or NOT OK message
					{
						perror("msrcv");
				    }
				    
				    printf("Funds in account: %s", message->mtext);
				    perror(message->mtext);
				    
				    break;
				    
				case 2:		// Withdrawal
				  printf("\nWithdrawal amount\n");	
				  scanf("%s", amount);
				  
				  
				    message->mtype = 6;
					strcpy(message->mtext,accountNumber);
					strcat(message->mtext,PIN);
					strcat(message->mtext,amount);
					
					if(msgsnd(serverID, message, 1000, 0) == -1) 
					{
						perror("msgsnd");  
			        }
					
					if(msgrcv(atmID, message, 1000, 7, 0) < 0) // Receive NOT ENOUGH or ENOUGH FUNDS from dbServer
					{
						perror("msrcv");
				    }

				    printf("%s", message->mtext);
				    perror(message->mtext);
				    
				    break;    
				}
				
			}
			else {
				numLogin++; 
				perror("else");
				perror(message->mtext);
			}
			
		}


	}
	pthread_exit(0);  	
}

