/**
	Database Editor
	
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
#include "queue.c" 
#define MSGSZ 128

typedef struct msg {
   long mtype; //type of the msg
   char mtext[MSGSZ]; //size of the msg
 } msg; 

bank *accounts;
int numOfAccounts = 0;
FILE* database; 

void updateDatabase(char accountNumber[5], char PIN[3], char *amountOfFunds);
void populateDB();
void* dbEditor(void* arg);
void* dbServer(void *arg);

int main() {
	initialize(accounts); 
	pthread_t server;
	pthread_t editor; 
	
	pthread_create(&server, NULL, dbServer, (void*)NULL); 
	pthread_create(&editor, NULL, dbEditor, (void*)NULL);
	
	pthread_join(server, NULL); 
	pthread_join(editor, NULL);  
	
	return 0; 
}

void updateDatabase(char accountNumber[5], char PIN[3], char *amountOfFunds) {
	database = fopen("dataBase.txt", "w");
	if(database == NULL) {
		printf("Error opening file!\n");
		exit(1);
	}
	
	fprintf(database, "%s,,%s,,%s", accountNumber, PIN, amountOfFunds); 
	
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
				strncpy(temp->accountNumber, token, sizeof(token));
			}
			else if(counter == 1) {
				strncpy(temp->PIN, token, sizeof(token));
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

void* dbEditor(void* arg) {
	key_t serverKey = 1234;
	int msgflg = IPC_CREAT | 0666; // message flag
	
	msg *message = NULL; 
	message->mtype = 1; // Update DB message
	int editorID;
	
	if((editorID = msgget(serverKey, 0666))	< 0)
	{
		perror("msgget");
		exit(1);
	}
	
	for(;;) {
		char accountNumber[5] = {'\0'};
		char PIN[3] = {'\0'};
		char *amountOfFunds = "\0";

		printf("\nPlease enter an account number (5 digits) \n");	
		scanf("%s", accountNumber);	
		
		printf("\nPlease enter your PIN number (3 digits) \n");	
		scanf("%s", PIN);	
		
		printf("\nPlease enter amount of funds (precision = 2 decimals)\n");	
		scanf("%s", amountOfFunds);
		
		strcpy(message->mtext, accountNumber); 
		strcat(message->mtext, PIN);
		strcat(message->mtext, amountOfFunds);
		
		//sending a message
		if(msgsnd(editorID, message, sizeof(msg), msgflg) == -1)
		{
			perror ("Update DB message failed");
		}
	}	
}

void* dbServer(void *arg) {
	key_t serverKey = 1234;
	int serverID;
	msg message; 
	int i;  
	
	if((serverID = msgget(serverKey, 0666))	< 0)
	{
		perror("msgget for server failed");
		exit(1);
	}
	
	for(;;) {
		if(msgrcv (serverID, &message, sizeof(msg), 1, 0) < 0) // receive Update DB message
		{
			perror("msgget");
			exit(1);
		}
		
		if(message.mtype == 1) {	//Update DB
			char accountNumber[5]; 
			char PIN[3];
			char *amountOfFunds = {'\0'};

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
			
		}	
	}
	
}

