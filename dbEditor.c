/**
	Database Editor
	
	@author Kshamina Ghelani
	@author Supriya Gadigone
**/

#include <stdlib.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/msg.h>
#include "queue.c"

#define MSGSZ 128

void promptUser();

msg_buff *buffer; 

int main(int argc, char* argv[])						
{	
	initialize(buffer);
	int msqid; //messageq id returned by the queue
	int msgflg = IPC_CREAT | 0666; // message flag
    size_t buf_length; //buffer length
    key_t key = 1234;
    int maxmsgsz = MSGSZ;
        
    buffer = (msg_buff *)malloc((unsigned)(sizeof(msg_buff) - sizeof buffer->front->mtext + maxmsgsz));
    if (buffer == NULL) {
		(void) fprintf(stderr, "msgop: %s %d byte messages.\n", "could not allocate message buffer for", MSGSZ);
		exit(1);
    }
    
    if((msqid = msgget(key, msgflg)) == -1) {
		perror("msgget failed");
		exit(1);
	}

	promptUser();
	return 0;
}

/**
	Prompts the user for their account number, PIN and amount of funds to add to acocunt.
*/
void promptUser()
{
	for(;;) {
		msg *mesg; 
		mesg = (msg *)malloc(sizeof(msg));
		
		const char *mtext = "Update DB";
		char accountNumber[5] = {'\0'};
		char PIN[3] = {'\0'};
		float amountOfFunds;

		printf("\nPlease enter an account number (5 digits) \n");	
		scanf("%s", accountNumber);	
		
		printf("\nPlease enter your PIN number (3 digits) \n");	
		scanf("%s", PIN);	
		
		printf("\nPlease enter amount of funds (precision = 2 decimals)\n");	
		scanf("%f", &amountOfFunds);
		
		mesg->mtype = 1;
        strncpy(mesg->accountNumber, accountNumber, sizeof(accountNumber));
        strncpy(mesg->PIN, PIN, sizeof(PIN));
        strncpy(mesg->mtext, mtext, sizeof(mtext));
		mesg->amountOfFunds = amountOfFunds; 
		mesg->next = NULL; 
		
		enqueue(buffer, mesg); 
		printQueue(buffer); 

	}
}





