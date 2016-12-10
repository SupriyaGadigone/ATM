/**
 * Queue created with reference to: http://www.techcrashcourse.com/2016/06/c-program-to-implement-queue-using-linked-list.html
 * 
 * C Program to Implement Queue Data Structure using Linked List
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
#define MSGSZ 128

/*
 * Add to the queue
*/
void enqueue(bank *accounts, account *acc) {
	acc->next = NULL;
	if (accounts->back == NULL) {
		 accounts->front = acc;
		 accounts->back = acc;
    }else {
		accounts->back->next = acc;
        accounts->back = acc;
    }
}



