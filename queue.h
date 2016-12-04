#ifndef ATM_queue_h
#define ATM_queue_h

#define MSGSZ 128


typedef struct msgbuf {
   long    mtype; //type of the msg
   char    mtext[MSGSZ]; //size of the msg
   char accountNumber[5];
   char PIN[3];
   float amountOfFunds;
   struct msgbuf *next;
 } message_buf;
 
 
extern void initialize(void);
int getQueueSize();
int getFrontElement();
int getBackElement();
void isEmpty();
void enqueue(message_buf *buf);
void dequeue();
void printQueue();


#endif
