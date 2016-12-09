/**
	Database Editor
	
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

void* dbEditor(void* arg);

int main() {	
	pthread_t editor;
	pthread_create(&editor, NULL, dbEditor, (void*)NULL);
    pthread_join(editor, NULL);
	
	return 0; 
}

void* dbEditor(void* arg) {
	key_t serverKey = 1234;
	int msgflg = 0666 | IPC_CREAT; // message flag
	
	msg *message; 
	int editorID;
	
	message = (msg *)malloc((unsigned)(sizeof(msg) - sizeof message->mtext + MSGSZ));
    if (message == NULL) {
		(void) fprintf(stderr, "msgop: %s %d byte messages.\n", "could not allocate message buffer for", MSGSZ);
		exit(1);
	}
	
	message->mtype = 1; // Update DB message
	
	if((editorID = msgget(serverKey, msgflg)) < 0)
	{
		perror("msgget");
		exit(1);
	}
	
	for(;;) {
		char accountNumber[5] = {'\0'};
		char PIN[3] = {'\0'};
		char amountOfFunds[100] = {'\0'};

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
	pthread_exit(0); 	
	
}
