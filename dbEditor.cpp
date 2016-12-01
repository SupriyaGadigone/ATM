/**
	Database Editor
	
	@author Kshamina Ghelani
	@author Supriya Gadigone
**/
#include <stdio.h>



void promptUser();

/**
 Main 
 **/
int main(int argc, char* argv[])						
{									
	
	promptUser();
	
	return 0;
}

/**
	Prompts the user for their account number, PIN and amount of funds to add to acocunt.
*/
void promptUser()
{
	int accountNumber, PIN, amountOfFunds;

	printf("\nPlease enter an account number\n");	
	scanf("%i", &accountNumber);		// read user inputted value
	printf("\nPlease enter your PIN number\n");	
	scanf("%i", &PIN);
	printf("\nPlease enter amount of funds\n");	
	scanf("%i", &amountOfFunds);
		
	
}
