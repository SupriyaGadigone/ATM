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
void initialize(void); 

int main(int argc, char* argv[])						
{	
	
	initialize();
	int msqid; //messageq id returned by the queue
	int msgflg = IPC_CREAT | 0666; // message flag
	struct message_buf *msgp; // pointer to message buffer
    size_t buf_length; //buffer length
    key_t key = 1234;
    
    msgp = (message_buf *)malloc((unsigned)(sizeof(message_buf) - sizeof(msgp->mtext + MSGSZ)));    
    if (msgp == NULL) {
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
		message_buf *buff; 
		buff = (struct msgbuf *)malloc(sizeof(struct msgbuf));
		
		char accountNumber[5];
		char PIN[3];
		float amountOfFunds;

		printf("\nPlease enter an account number (5 digits) \n");	
		//fgets(accountNumber, 20, stdin);
		scanf("%s", accountNumber);	
		printf("****%s", accountNumber);

		printf("\nPlease enter your PIN number (3 digits) \n");	
		scanf("%s", PIN);	
		
		printf("\nPlease enter amount of funds (precision = 2 decimals)\n");	
		scanf("%f", &amountOfFunds);
		
		buff->mtype = 1;
		buff->mtext = "Update DB";
        strncpy(buff->accountNumber, accountNumber, sizeof(accountNumber));
        strncpy(buff->PIN, PIN, sizeof(PIN));
		buff->amountOfFunds = amountOfFunds; 
		buff->next = NULL; 
		
		enqueue(buff); 
		//enqueue(1, "Update DB", accountNumber, PIN, amountOfFunds);

		
	}
}





