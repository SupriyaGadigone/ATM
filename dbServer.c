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
#include "queue.c"

#define MSGSZ  128

msg_buff *buffer;
int msqid;
key_t key; 

int main(int argc, char* argv[])						
{		
	msg_buff rbuf;
	
	key = 1234;// same key that was set in editor 
	
	printf("1**got here");
	//initializignt he queue again
	if((msqid = msgget(key, 0666))	< 0)
	{
		perror("msgget");
		exit(1);
	}
	
	printf("2***got heheh");
	//receive a message
	if(msgrcv (msqid, &rbuf, MSGSZ, 1, 0) < 0)
	{
		perror("msgget");
		exit(1);
	}			
	
	printf("gets here!");
	
	printf("%s\n", rbuf.front->mtext);	
	
	return 0;
}
