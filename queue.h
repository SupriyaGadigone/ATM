#ifndef ATM_queue_h
#define ATM_queue_h

#define MSGSZ 128


typedef struct msgbuf {
   long    mtype; //type of the msg
   char    mtext[MSGSZ]; //size of the msg
   char accountNumber;
   char PIN;
   float amountOfFunds;
   struct msgbuf *next;
 } message_buf;
 
 
extern void initialize();
int getQueueSize();
int getFrontElement();
int getBackElement();
void isEmpty();
void enqueue(long msgType, char mtext[MSGSZ], char accountNumber, char PIN, float amountOfFunds);
void dequeue();
void printQueue();


#endif
