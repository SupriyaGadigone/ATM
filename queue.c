/**
 * http://www.techcrashcourse.com/2016/06/c-program-to-implement-queue-using-linked-list.html
 * 
 * C Program to Implement Queue Data Structure using Linked List
 */

#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MSGSZ 128


/* Create an empty queue */
void initialize(msg_buff *buffer) {
   buffer = (msg_buff *)malloc(sizeof(msg_buff));
   buffer->front = NULL;
   buffer->back = NULL;
}

void enqueue(msg_buff *buffer, msg *mesg) {
	msg *temp;
	temp = (msg *)malloc(sizeof(msg));
	temp = mesg;
	
	if (buffer->back == NULL) {
		buffer->front = buffer->back = temp;
    } else {
		buffer->back->next = temp;
        buffer->back = temp;
    }
}
 
/*
Removes an element from front of the queue
*/
void dequeue(msg_buff *buffer) {
    msg *temp;
    if (buffer->front == NULL) {
        printf("\nQueue is Empty \n");
        return;
    } else {
        temp = buffer->front;
        buffer->front = buffer->front->next;
        if(buffer->front == NULL){
            buffer->back = NULL;
        }

        free(temp);
    }
}
  
/*
 Print's Queue
*/
void printQueue(msg_buff *buffer) {
	int c;
	msg *temp = buffer->front;
  
    if ((buffer->front == NULL) && (buffer->back == NULL)) {
        printf("Queue is Empty\n");
        return;
    }
 
    while (temp != NULL) {
        printf("\n%ld\n",  temp->mtype);
        for (c=0;c < MSGSZ; c++)
            printf("%c",  temp->mtext[c]);
        printf("\n");
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


