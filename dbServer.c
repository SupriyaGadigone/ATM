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
#include "queue.c" 
#define MSGSZ 128

typedef struct msg {
   long mtype; //type of the msg
   char mtext[MSGSZ]; //size of the msg
 } msg; 

bank *accounts;
int numOfAccounts = 0;
FILE* database; 

void updateDatabase(char accountNumber[5], char PIN[3], char amountOfFunds[10]);
void populateDB();
void* dbServer(void *arg);

int main() {
	initialize(accounts); 
	pthread_t server; 
	pthread_create(&server, NULL, dbServer, (void*)NULL);
	pthread_join(server, NULL);  
		
	return 0; 
}

void updateDatabase(char accountNumber[5], char PIN[3], char amountOfFunds[10]) {
	database = fopen("dataBase.txt", "a");
	//make sure appends
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
	
	account *temp; 
	temp = (account *)malloc(sizeof(account));
	strcpy(temp->accountNumber, accountNumber); 
	strcpy(temp->PIN, PIN); 
	temp->amountOfFunds = atof(amountOfFunds);  
	enqueue(accounts, temp);
	 
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
	key_t atmKey = 5678;
	key_t serverKey = 1234;
	
	int msgflg = IPC_CREAT | 0666;
	
	int atmID; 
	int editorID;
	
	msg message; 
	msg message2; 
	int i;
	bool check = false; 
	
	/*
	message = (msg *)malloc((unsigned)(sizeof(msg) - sizeof message->mtext + MSGSZ));
    if (message == NULL) {
		(void) fprintf(stderr, "msgop: %s %d byte messages.\n", "could not allocate message buffer for", MSGSZ);
		exit(1);
	}
	*/
	
	if((editorID = msgget(serverKey, msgflg))	< 0)	// message queue for editor
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
		msgrcv(editorID, &message, sizeof(msg), 1, IPC_NOWAIT); // receive Update DB message
		msgrcv(atmID, &message2, sizeof(msg) , 2, IPC_NOWAIT) ;
		

		
		if(message.mtype == 1) {	//Update DB
			char accountNumber[5]; 
			char PIN[3];
			char amountOfFunds[10];

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
		
		else if(message.mtype == 2) { //check if PIN and accountnumber are valid or not
			char accountNumber[5]; 
			char PIN[3];
			
			int i; 
			for(i = 0; i < 5; i++) {
				accountNumber[i] = message2.mtext[i];
			}
			for(i = 0; i < 3; i++) {
				PIN[i] = message2.mtext[i+3]; 
			}
			accountNumber[5] = '\0';
			PIN[3] = '\0';
			
			account *temp = accounts->front;
			while (temp != NULL) {
				if(strcmp(temp->accountNumber, accountNumber) == 0 && strcmp(temp->PIN, PIN) == 0) {
					check = true; 
				}
				temp = temp->next; 
			}
			printf("got here");
			if(check) {
				strcpy(message2.mtext, "OK"); 
			}
			else {
				strcpy(message2.mtext, "NOT OK"); 
			}
			
			message.mtype = 3; // Confirm/Revoke login
			if(msgsnd(atmID, &message2, strlen(message.mtext) + 1, 0) < 0) {
				perror("msgsnd confirm");
				exit(1); 
			}
		}	
	}
	pthread_exit(0); 
	
}


