/**
	Database SErver 
	
	@author Kshamina Ghelani
	@author Supriya Gadigone
**/
#include <stdlib.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/msg.h>

#define MSGSZ  128

void printMessage();

typedef struct message {
   long mtype; //type of the msg
   char mtext[MSGSZ]; //size of the msg
   char accountNumber[5];
   char PIN[3];
   float amountOfFunds;
 } msg_buff; 

int msqid;
key_t key; 
msg_buff rbuf;

int main(int argc, char* argv[])						
{		
	key = 1234;// same key that was set in editor 

	//initializignt he queue again
	if((msqid = msgget(key, 0666))	< 0)
	{
		perror("msgget");
		exit(1);
	}
	
	//receive a message
	if(msgrcv (msqid, &rbuf, MSGSZ, 1, 0) < 0)
	{
		perror("msgget");
		exit(1);
	}			
	
	FILE* file = fopen("dataBase.txt", "w");
	if(file == NULL) {
		printf("Error opening file!\n");
		exit(1);
	}
	
	const char *text = "Account Number\tPIN\tAmount of Funds";
	fprintf(file, "%s\n", text);
	fprintf(file, "%s", rbuf.accountNumber); 
	fprintf(file, "%s", rbuf.PIN); 
	fprintf(file, "%f", rbuf.amountOfFunds);  
	
	printMessage();
	
	return 0;
}

void printMessage() {
	printf("\n%ld\n",  rbuf.mtype);
    int c; 
    for (c=0;c < MSGSZ; c++) {
		printf("%c",  rbuf.mtext[c]);
	}
    printf("\n");
    for (c=0;c < 5; c++) {
		printf("%c",  rbuf.accountNumber[c]);
	}
	printf("\n");
	for (c=0;c < 3; c++) {
		printf("%c",  rbuf.PIN[c]);
	}
    printf("\n");
    printf("%.2f\n",  rbuf.amountOfFunds);
}
