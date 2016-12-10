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
	key_t serverKey = 1111;
	key_t atmKey = 5678;
	//key_t sevKey = 9101;
	
	
	msg *message;
	message = (msg *)malloc((unsigned)(sizeof(msg) - sizeof message->mtext + MSGSZ));
	
	while(1) {
		printf("\nPlease enter an account number (5 digits) \n");	
		scanf("%s", accountNumber);	
		
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
				exit(1); 
			}
			
			
			if(msgrcv(atmID, message, 1000, 3, 0) > 0) // receive OK or NOT OK message
			{
				perror("msrcv wroked*");
				exit(1);
			}
			
			if(strcmp(message->mtext, "OK") == 0) {
				valid = true; 
				
			}
			else {
				numLogin++; 
			}
			
		}
		// LOCK THE ATM L8	

	}
	pthread_exit(0);  	
}

