#ifndef ATM_queue_h
#define ATM_queue_h

#define MSGSZ 128

// info in each account
typedef struct account {
	char accountNumber[6];
	char PIN[4];
	float amountOfFunds;
	struct account *next; 
} account;

// accounts in the bank
typedef struct bank {
	account *front;
	account *back ;
} bank;  
 
void enqueue(bank *accounts, account *acc);
#endif
