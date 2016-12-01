/**
	Database Editor
	
	@author Kshamina Ghelani
	@author Supriya Gadigone
**/

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

extern int errno;       // error NO.
#define MSGPERM 0600    // msg queue permission
#define MSGTXTLEN 128   // msg text length

void promptUser();

key_t key; /* key to be passed to msgget() */ 
int msgflg; /* msgflg to be passed to msgget() */ 
int msqid; /* return value from msgget() */ 
int msgqid;


/**
 Main 
 **/
int main(int argc, char* argv[])						
{		
	
	  // create a message queue. If here you get a invalid msgid and use it in msgsnd() or msgrcg(), an Invalid Argument error will be returned.
  msgqid = msgget(key, msgflg);
  if (msgqid < 0) {
    printf("failed to create message queue with msgqid = %d\n", msgqid);
    return 1;
  }
  printf("message queue %d created\n",msgqid);				
	
	promptUser();
	
	return 0;
}

/**
	Prompts the user for their account number, PIN and amount of funds to add to acocunt.
*/
void promptUser()
{
	int accountNumber, PIN, amountOfFunds;

	printf("\nPlease enter an account number\n");	
	scanf("%i", &accountNumber);		// read user inputted value
	printf("\nPlease enter your PIN number\n");	
	scanf("%i", &PIN);
	printf("\nPlease enter amount of funds\n");	
	scanf("%i", &amountOfFunds);
	
}
