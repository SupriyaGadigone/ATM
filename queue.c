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


struct msgbuf *front, *back;
 


/* Create an empty queue */
void initialize(void) {
   front = back = NULL;
}
 
/* Returns queue size */
int getQueueSize() {
    struct msgbuf *temp = front;
    int count = 0;
     
    if(front == NULL && back == NULL)
        return 0;
 
    while(temp != back){
        count++;
        temp = temp->next;
    }
    if(temp == back)
        count++;
         
    return count;
}
  
/* Returns Frnt Element of the Queue 
int getFrontElement() {
    return front->data;
}
 
 Returns the Rear Element of the Queue 
int getBackElement() {
    return back->data;
}
*/
 
/*
Check's if Queue is empty or not 
*/
void isEmpty() {
    if (front == NULL && back == NULL)
        printf("Empty Queue\n");
    else
        printf("Queue is not Empty\n");
}
/*
Adding elements in Queue
*/
void enqueue(long msgType, char mtext[MSGSZ], char *accountNumber, char *PIN, float amountOfFunds) {
    struct msgbuf *temp;
    temp = (struct msgbuf *)malloc(sizeof(struct msgbuf));
    temp->mtype = msgType;
    temp->mtext[MSGSZ] = mtext[MSGSZ];
    strncpy(temp->accountNumber, accountNumber, sizeof(accountNumber));
    strncpy(temp->PIN, PIN, sizeof(PIN));
    temp->amountOfFunds = amountOfFunds;
    temp->next = NULL;
     
    if (back == NULL) {
        front = back = temp;
    } else {
        back->next = temp;
        back = temp;
    }
}
 
/*
Removes an element from front of the queue
*/
void dequeue() {
    struct msgbuf *temp;
    if (front == NULL) {
        printf("\nQueue is Empty \n");
        return;
    } else {
        temp = front;
        front = front->next;
        if(front == NULL){
            back = NULL;
        }

        free(temp);
    }
}
  
/*
 Print's Queue
*/
void printQueue() {
	struct msgbuf *temp = front;
  
    if ((front == NULL) && (back == NULL)) {
        printf("Queue is Empty\n");
        return;
    }
 
    while (temp != NULL) {
        printf("%ld\n",  temp->mtype);
        printf("%s\n",  temp->mtext);
        printf("%s\n",  temp->accountNumber);
        printf("%s\n",  temp->PIN);
        printf("%f\n",  temp->amountOfFunds);

        temp = temp->next;
        if(temp != NULL)
            printf("-->");
    }
}


