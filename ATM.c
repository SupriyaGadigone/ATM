/**
	ATM
	
	@author Kshamina Ghelani
	@author Supriya Gadigone
	
	TO COMPILE: 
	* gcc ATM.c -o ATM -lpthread
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

void* atm(void* arg) {
	char accountNumber[5] = {'\0'};
	char PIN[3] = {'\0'};
	int numLogin = 0;
	bool valid = false;
		
	int serverID;
	int atmID; 
	int msgflg = IPC_CREAT | 0666;
	int input;
	key_t serverKey = 1111;
	key_t atmKey = 5678;
	//key_t sevKey = 9101;
	
	
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
			
			
			if(msgsnd(serverID, message, 1000, 0) == 0) {
				perror("msgsnd to server from atm");  
			}
			
			
			if(msgrcv(atmID, message, 1000, 3, 0) > 0) // receive OK or NOT OK message
			{
				perror("msrcv wroked*");
	
			}
	
			
			if(strcmp(message->mtext, "OK") == 0) {
				valid = true; 
				printf("\nPress 1 to display funds and 2 to withdraw \n");	
				scanf("%d", &input);
				switch (input){
					
				case 1:
					message->mtype = 4;
					strcpy(message->mtext,accountNumber);
					if(msgsnd(serverID, message, 1000, 0) == 0) 
					{
						perror("msgsnd to server from atm of display funds");  
			        }
					
					
					if(msgrcv(atmID, message, 1000, 3, 0) > 0) // receive OK or NOT OK message
					{
						perror("msrcv wroked for display funds");
				    }
				    
				    printf("Funds in account: %s", message->mtext);
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
		// LOCK THE ATM L8	

	}
	pthread_exit(0);  	
}

