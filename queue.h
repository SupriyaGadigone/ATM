#ifndef ATM_queue_h
#define ATM_queue_h

#define MSGSZ 128


typedef struct message {
   long mtype; //type of the msg
   char mtext[MSGSZ]; //size of the msg
   char accountNumber[5];
   char PIN[3];
   float amountOfFunds;
   struct message *next;
 } msg; 
 
 typedef struct msgbuf {
	 msg *front;
	 msg *back;
 } msg_buff; 
 
 
void initialize(msg_buff *buffer);
void enqueue(msg_buff *buffer, msg *msg);
void dequeue(msg_buff *buffer);
void printQueue(msg_buff *buffer);


#endif
