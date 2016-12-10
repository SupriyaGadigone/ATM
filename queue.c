/**
 * http://www.techcrashcourse.com/2016/06/c-program-to-implement-queue-using-linked-list.html
 * 
 * C Program to Implement Queue Data Structure using Linked List
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
#define MSGSZ 128


/* Create an empty queue */
void initialize(bank *accounts) {
   accounts = (bank *)malloc(sizeof(bank));
   accounts->front = NULL;
   accounts->back = NULL;
}

void enqueue(bank *accounts, account *acc) {
    //accounts = (bank *)malloc(sizeof(bank));
	//account *temp = (account *)malloc(sizeof(acc));
	//temp = acc;
      acc->next = NULL;
	if (accounts->back == NULL) {
		 accounts->front = acc;
		 accounts->back = acc;
    }else {
		accounts->back->next = acc;
        accounts->back = acc;
    }
}
  
/*
 Print's Queue
*/
void printQueue(bank *accounts) {
	int c;
	account *temp = accounts->front;
  
    if ((accounts->front == NULL) && (accounts->back == NULL)) {
        printf("Queue is Empty\n");
        return;
    }
 
    while (temp != NULL) {
        for (c=0;c < 5; c++)
           printf("%c",  temp->accountNumber[c]);
        printf("\n");
        for (c=0;c < 3; c++)
           printf("%c",  temp->PIN[c]);
        printf("\n");
        printf("%.2f\n",  temp->amountOfFunds);

        temp = temp->next;
        if(temp != NULL)
            printf("-->");
    }
}


