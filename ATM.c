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
#include "queue.c" 
#define MSGSZ 128

typedef struct msg {
   long mtype; //type of the msg
   char mtext[MSGSZ]; //size of the msg
 } msg; 

bank *accounts;
int numOfAccounts = 0;
FILE* database; 

void updateDatabase(char accountNumber[5], char PIN[3], char amountOfFunds[100]);
void populateDB();
void* dbEditor(void* arg);
void* dbServer(void *arg);

int main() {
	initialize(accounts); 
	pthread_t server;
 
	pthread_create(&server, NULL, dbServer, (void*)NULL);
	pthread_join(server, NULL);  
		
	return 0; 
}

void updateDatabase(char accountNumber[6], char PIN[4], char amountOfFunds[100]) {
	database = fopen("dataBase.txt", "w");
	if(database == NULL) {
		printf("Error opening file!\n");
		exit(1);
	}
	
	fprintf(database, "%s,", accountNumber);
	int i;
	for(i = 0; i < 3; i++) {
		fprintf(database, "%c", PIN[i]);
	}
	fprintf(database,",");
	//fprintf(database, "%s,", PIN);
	fprintf(database, "%s", amountOfFunds);   
	fprintf(database, "\n"); 
	
	fclose(database); 
}

void populateDB() {
	char *token;
	char *data;
	int counter = 0; 
	const char delimeter[2] = ","; 
	account *temp = NULL;
	
	database = fopen("dataBase.txt", "r");
	char line[256];
	
	while(fgets(line, sizeof(line), database)) {
		data = line; 
		token = strtok(data, delimeter);
		while( token != NULL ) 
		{
			if(counter == 0) {
				strncpy(temp->accountNumber, token, 5);
			}
			else if(counter == 1) {
				strncpy(temp->PIN, token, 3);
			}
			else if(counter == 2) {
				float f = atof(token);
				temp->amountOfFunds = f; 
			}
			token = strtok(NULL, delimeter);
			counter++;
		}
		enqueue(accounts, temp); 
		counter = 0; 
	}
	fclose(database); 	
}

void* dbServer(void *arg) {
	key_t serverKey = 1234;
	int serverID;
	msg message; 
	int i;  
	
	/*
	message = (msg *)malloc((unsigned)(sizeof(msg) - sizeof message->mtext + MSGSZ));
    if (message == NULL) {
		(void) fprintf(stderr, "msgop: %s %d byte messages.\n", "could not allocate message buffer for", MSGSZ);
		exit(1);
	}
	*/
	
	if((serverID = msgget(serverKey, 0666))	< 0)
	{
		perror("msgget for server failed");
		exit(1);
	}
	
	for(;;) {
		if(msgrcv (serverID, &message, sizeof(msg), 1, 0) < 0) // receive Update DB message
		{
			perror("msrcv");
			exit(1);
		}
		
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
			
			updateDatabase(accountNumber, PIN, amountOfFunds); 
			//printQueue(accounts); 
			
		}	
	}
	pthread_exit(0); 
	
}

