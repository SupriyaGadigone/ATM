/**
	Database server
	
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

#define MSGSZ 128

typedef struct account {
	char accountNumber[5];
	char PIN[3];
	float amountOfFunds;
} account;

typedef struct msg {
   long mtype; //type of the msg
   char mtext[MSGSZ]; //size of the msg
 } msg; 

account *accounts; 
int numOfAccounts = 0;
FILE* database; 

void updateDatabase(char accountNumber[5], char PIN[3], char amountOfFunds[100], int numAcc);
account* populateDB();
void* dbServer(void *arg);

int main() {
	pthread_t server; 
	pthread_create(&server, NULL, dbServer, (void*)NULL);
	pthread_join(server, NULL);  
		
	return 0; 
}

void updateDatabase(char accountNumber[5], char PIN[3], char amountOfFunds[100], int numAcc) {
	database = fopen("dataBase.txt", "w");
	if(database == NULL) {
		printf("Error opening file!\n");
		exit(1);
	}
	
	int i;
	for(i = 0; i < numAcc; i++) {	
		fprintf(database, "%s,", accountNumber);
	
		for(i = 0; i < 3; i++) {
			fprintf(database, "%c", PIN[i]);
		}
		fprintf(database,",");
		fprintf(database, "%s", amountOfFunds);   
		fprintf(database, "\n"); 
	}
	
	fclose(database); 

}

account* populateDB() {	
	database = fopen("dataBase.txt", "r");
	account *arr; 

	if(!feof(database)) {
		arr = (account*) malloc(sizeof(account));
		fscanf(database, "%s %s %f", arr->accountNumber, arr->PIN, &arr->amountOfFunds);
	}
	
	numOfAccounts = 1;

	while(!feof(database)) {
		account* temp = (account*)malloc(sizeof(account));
		fscanf(database, "%s %s %f",temp->accountNumber, temp->PIN, &temp->amountOfFunds);
		assert(temp);
		account* t = (account*)realloc(arr, sizeof(account)*(numOfAccounts + 1));
		
		if(t != NULL) {
			arr = t;
		}
		
		arr[numOfAccounts] = *temp;

		numOfAccounts++;
	}
	
	fclose(database); 
	return arr;
	
	
	
}

void* dbServer(void *arg) {
	key_t serverKey = 1234;
	key_t atmKey = 5678;
	int msgflg = IPC_CREAT | 0666;
	
	int serverID;
	int atmID; 
	
	msg message; 
	int i;
	bool check = false; 
	
	/*
	message = (msg *)malloc((unsigned)(sizeof(msg) - sizeof message->mtext + MSGSZ));
    if (message == NULL) {
		(void) fprintf(stderr, "msgop: %s %d byte messages.\n", "could not allocate message buffer for", MSGSZ);
		exit(1);
	}
	*/
	
	if((serverID = msgget(serverKey, msgflg))	< 0)	// message queue for editor
	{
		perror("msgget for server failed");
		exit(1);
	}
	
	if((atmID = msgget(atmKey, msgflg))	< 0)		// message queue for ATM
	{
		perror("msgget for server failed");
		exit(1);
	}
	
	for(;;) {
		msgrcv(serverID, &message, sizeof(msg), 1, IPC_NOWAIT); // receive Update DB message
		msgrcv(atmID, &message, sizeof(msg), 2, IPC_NOWAIT); 
		
		if(message.mtype == 1) {	//Update DB
			char accountNumber[5]; 
			char PIN[3];
			char amountOfFunds[100];

			for(i = 0; i < 5; i++) {
				accountNumber[i] = message.mtext[i];
			}
			
			for(i = 0; i < 3; i++) {
				PIN[i] = message.mtext[i+5];
			}
			
			//Convert back to float in updateDatabase
			for(i = 0; i < sizeof(message.mtext); i++) {
				amountOfFunds[i] = message.mtext[i+8];
			}
			
			updateDatabase(accountNumber, PIN, amountOfFunds, numOfAccounts); 
			//printQueue(accounts); 
			
		}
		
		else if(message.mtype == 2) {
			char accountNumber[5]; 
			char PIN[3];
			
			int i; 
			for(i = 0; i < 5; i++) {
				accountNumber[i] = message.mtext[i];
			}
			for(i = 0; i < 3; i++) {
				PIN[i] = message.mtext[i+3]; 
			}
			accountNumber[5] = '\0';
			PIN[3] = '\0';
			
			for(i = 0; i < numOfAccounts; i++) {
				if(strcmp(accounts[i].accountNumber, accountNumber) == 0 && strcmp(accounts[i].PIN, PIN) == 0) {
					check = true; 
				}
			}
			
			if(check) {
				strcpy(message.mtext, "OK"); 
			}
			else {
				strcpy(message.mtext, "NOT OK"); 
			}
			
			message.mtype = 3; // Confirm/Revoke login
			if(msgsnd(atmID, &message, strlen(message.mtext) + 1, 0) < 0) {
				perror("msgsnd confirm");
				exit(1); 
			}
		}	
	}
	pthread_exit(0); 
	
}


