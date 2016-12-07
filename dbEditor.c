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

#define MSGSZ 128

typedef struct message {
   long mtype; //type of the msg
   char mtext[MSGSZ]; //size of the msg
   char accountNumber[5];
   char PIN[3];
   float amountOfFunds;
 } msg_buff; 

void promptUser();
void printMessage();

msg_buff *sbuf;
long msgtyp = 1; 
int msqid; //messageq id returned by the queue
int msgflg = IPC_CREAT | 0666; // message flag

int main(int argc, char* argv[])						
{	
    size_t buf_length; //buffer length
    key_t key = 1234;
    int maxmsgsz = MSGSZ;
     
    sbuf = (msg_buff *)malloc((unsigned)(sizeof(msg_buff) - sizeof sbuf->mtext + maxmsgsz));
    if (sbuf == NULL) {
		(void) fprintf(stderr, "msgop: %s %d byte messages.\n", "could not allocate message buffer for", MSGSZ);
		exit(1);
    }
    
    //initializing the message queue
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
		
		sbuf->mtype = msgtyp;
        strncpy(sbuf->accountNumber, accountNumber, sizeof(accountNumber));
        strncpy(sbuf->PIN, PIN, sizeof(PIN));
        strncpy(sbuf->mtext, mtext, MSGSZ);
		sbuf->amountOfFunds = amountOfFunds; 
		
		printMessage();
		
		//sending a message
		if(msgsnd(msqid, sbuf, MSGSZ, msgflg) == -1)
		{
			perror ("msgop: msgsnd failed");
		}
	

	}
}

void printMessage() {
	printf("\n%ld\n",  sbuf->mtype);
    int c; 
    for (c=0;c < MSGSZ; c++) {
		printf("%c",  sbuf->mtext[c]);
	}
    printf("\n");
    for (c=0;c < 5; c++) {
		printf("%c",  sbuf->accountNumber[c]);
	}
	printf("\n");
	for (c=0;c < 3; c++) {
		printf("%c",  sbuf->PIN[c]);
	}
    printf("\n");
    printf("%.2f\n",  sbuf->amountOfFunds);
}





