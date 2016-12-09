SYSC 4001 Assignment 3

Kshamina Ghelani - 100980239
Supriya Gadigone - 100967862

There are three C files. 

dbServer.c - the database server
- receives messages from the atm and the editor
- sends messages back only to the atm, to confirm/revoke login, to display funds and to confirm/deny withdrawels

dbEditor.c - the database editor
- The editor prompts the user for account number, PIN and initial amount of funds
- it sends an update DB message to the server so the server can save that information in a queue and write it to a file

ATM.c - the atm interface
- the ATM prompts the user for an account number and PIN
- the server tells the atm whether those were valid
- if valid, the atm prompts the user to either display or withdraw funds
- if not, the PIN is reprompted for
- if the account num and PIN is invalid 3x in a row, the account gets locked

dataBase.txt
- testfile which represents the database

Running Instructions:
Open 3 different terminals and compile and run each c file in this order.  

Terminal 1: 
	compile - "gcc dbEditor.c -o dbEditor -lpthread"
	run - "./dbEditor"

Terminal 2: 
	compile - "gcc ATM.c -o ATM -lpthread"
	run - "./ATM"

Terminal 3: 
	compile - "gcc dbServer.c -o dbServer -lpthread"
	run - "./dbServer"

First, populate your db by adding accounts through the db editor. 
You can then type into the atm to display/withdraw funds. 

The concurrency of the threads was tested by having three different terminals and update the database throught the editor while accessing the atm atm at the same time and seeing if the values were updated properly.  

