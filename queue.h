#ifndef ATM_queue_h
#define ATM_queue_h

#define MSGSZ 128

typedef struct account {
	char accountNumber[6];
	char PIN[4];
	float amountOfFunds;
	struct account *next; 
} account;

typedef struct bank {
	account *front;
	account *back ;
} bank;  
 
void initialize(bank *accounts);
void enqueue(bank *accounts, account *acc);
void dequeue(bank *accounts);
void printQueue(bank *accounts);
#endif
