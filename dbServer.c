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
void* atm(void *arg);

int main() {
	accounts = (bank *)malloc(sizeof(bank));
	//initialize(accounts); 
	pthread_t server; 
	pthread_t atm1;
	
	pthread_create(&server, NULL, dbServer, (void*)NULL);
	pthread_create(&atm1, NULL, atm , (void*)NULL);
	
	pthread_join(server, NULL);  
	pthread_join(atm1, NULL);  
		
	return 0; 
}

void updateDatabase(char accountNumber[5], char PIN[3], char amountOfFunds[10]) {
	database = fopen("dataBase.txt", "a");

	if(database == NULL) {
		printf("Error opening file!\n");
		exit(1);
	}

	fprintf(database, "%s,", accountNumber);
	fprintf(database, "%s,", PIN);
	fprintf(database, "%s\n", amountOfFunds);  
	
	perror("updateDatabase");
	
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
	//key_t editorKey = 5678;
	key_t serverKey = 1234;
	
	int msgflg = IPC_CREAT | 0666;
	
	//int atmID; 
	int editorID;
	
	msg message; 

	

	int i;
	//bool check = false; 
	
	
	
	if((editorID = msgget(serverKey, msgflg))< 0)	// message queue for editor
	{
		perror("msgget for server failed");
		exit(1);
	}
	

	
	for(;;) {
		if(msgrcv(editorID, &message, sizeof(msg), 1, 0)>0) // receive Update DB message
		{
			perror("msgrcv: editor worked");
			
		}
		 
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
			for(i = 0; i < 10; i++) {
				amountOfFunds[i] = message.mtext[i+8];
			}
			
			
			updateDatabase(accountNumber, PIN, amountOfFunds); 
			//printQueue(accounts); 
			
		}
	
	}
	pthread_exit(0); 
	
}

void* atm(void *arg)
{
	key_t atmKey = 5678;
	key_t serverKey = 1111;
	
	int msgflg = IPC_CREAT | 0666;
	int atmID; 
	int serverID;
	
	int check = 0; 
	
	
	msg message;
	
	if((atmID = msgget(atmKey, msgflg))	< 0)		// message queue for ATM
	{
		perror("msgget for server failed");
		exit(1);
	}
	
	if((serverID = msgget(serverKey, msgflg))	< 0)		// message queue for ATM
	{
		perror("msgget for server2 failed");
		exit(1);
	}
	
	for(;;){
		
		if(msgrcv(serverID, &message, 1000, 2, 0) < 0)
		{
			perror("msgrcv");	
		}
		if(msgrcv(serverID, &message, 1000, 4, 0) < 0)
		{
			perror("msgrcv");	
		}
		if(msgrcv(serverID, &message, 1000, 6, 0) < 0)
		{
			perror("msgrcv");	
		}


		if(message.mtype == 2) { //check if PIN and accountnumber are valid or not
				
				char accountNumber[5]; 
				char PIN[3];
				
				int i; 
				for(i = 0; i < 5; i++) {
					accountNumber[i] = message.mtext[i];
				}
				for(i = 0; i < 3; i++) {
					PIN[i] = message.mtext[i+5]; 
				}
				accountNumber[5] = '\0';
				PIN[3] = '\0';
                
				account *temp;
				temp = accounts->front;
	
				while (temp != NULL) {
					if(strcmp(temp->accountNumber, accountNumber) == 0 && strcmp(temp->PIN, PIN) == 0) {
						check = 1; 
					}
					temp = temp->next; 
				}
				if(check == 1) {
					strcpy(message.mtext, "OK"); 
				}
				else {
					strcpy(message.mtext, "NOT OK"); 
				}

				message.mtype = 3; // Confirm/Revoke login
				if(msgsnd(atmID, &message, 1000, 0) == -1) {
					perror("msgsnd"); 
				}
			}
			
			if(message.mtype == 4) //display funds 
			{
				account *temp;
				temp = accounts->front;
				char mtext[5];
				strcpy(mtext, message.mtext);
				while (temp != NULL) {
					if(strcmp(temp->accountNumber, mtext) == 0 ) {
						printf("%f",temp->amountOfFunds);
						
						char str[80];
						sprintf(str, "%f", temp->amountOfFunds);
						strcpy(message.mtext, str);
					}
					temp = temp->next; 
				}
				message.mtype = 5;
				if(msgsnd(atmID, &message, 1000, 0) == -1) {
					perror("msgsnd"); 
				}
				
			}
			
			if(message.mtype == 6) //withdrawfunds 
			{
				
				char accountNumber[5]; 
				char amount[10];
				char PIN[3];
				float a;
				float balance;
				
				
				int i; 
				for(i = 0; i < 5; i++) {
					accountNumber[i] = message.mtext[i];
				}
				for(i = 0; i < 3; i++) {
					PIN[i] = message.mtext[i+5]; 
				}
				for(i = 0; i < 10; i++) {
					amount[i] = message.mtext[i+8]; 
				}
				accountNumber[5] = '\0';
				PIN[10] = '\0';
				amount[10] = '\0';
				
				
				a = strtof(amount, NULL);  //converting to float
				
				account *temp;
				temp = accounts->front;
	
				while (temp != NULL) {
					if(strcmp(temp->accountNumber, message.mtext) == 0 ) {
						balance = temp->amountOfFunds;
					}
					temp = temp->next; 
				}
				if(balance < a)
				{
					strcpy(message.mtext, "NOT ENOUGH"); 
					
				}
				else
				{
					strcpy(message.mtext, "ENOUGH FUNDS");
					balance = balance - a;
				}
				
				
				char s[80];
				sprintf(s, "%f", balance);
				strcpy(message.mtext, s);
				
				updateDatabase(accountNumber, PIN, s);
				
				message.mtype = 7;
				if(msgsnd(atmID, &message, 1000, 0) == -1) {
					perror("msgsnd"); 
				}
				
			}
			
			
	
	}
	pthread_exit(0);
}


