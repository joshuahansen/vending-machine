/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2016 Assignment #2
 * Full Name        : Joshua Hansen
 * Student Number   : s3589185
 * Course Code      : COSC1076
 * Program Code     : BP094
 * Start up code provided by Paul Miller
 * Some codes are adopted here with permission by an anonymous author
 ***********************************************************************/
#include "ppd_coin.h"
#include "ppd_stock.h"
 /**
  * @file ppd_coin.c implement functions here for managing coins in the
  * "cash register" contained in the @ref ppd_system struct.
  **/
void add_coins(struct ppd_system *system, int cents, unsigned count)
{
	struct coin newCoin;
	enum denomination newDenom = cents_to_denom(cents);
	/*add values to new coin struct*/
	newCoin.denom = newDenom;
	newCoin.count = count;
	/*add struct into the cash register array*/
	system->cash_register[newDenom] = newCoin;
}

/*reset coin levels to specified default count*/
void resetCoinLevels(struct ppd_system *system)
{
	int i;
	for(i = ZERO; i < NUM_DENOMS; i++)
	{
		system->cash_register[i].count = DEFAULT_COIN_COUNT;
	}
}
/*save coins to file*/
void save_coins(struct ppd_system *system)
{
	FILE *fp;
	int i;
	char nums[PRICE];
	unsigned cents;
	fp = fopen(system->coin_file_name, "w");	/*open coing file for writting*/
	if(fp == NULL)
	{
		printf("File '%s' could not be opened for writing.\n\n", system->coin_file_name);
		exit(EXIT_FAILURE);
	}
	printf("Writting coins to file");
	for(i = NUM_DENOMS - ONE; i >= ZERO; i--)
	{
		printf(".");
		cents = denom_to_cents(system->cash_register[i].denom);
		sprintf(nums, "%d", cents);
		fputs(nums, fp);
		fputs(COIN_DELIM, fp);
		sprintf(nums, "%d", system->cash_register[i].count);
		fputs(nums, fp);
		fputs("\n", fp);
	}
	fclose(fp);
	printf("\n");		
}
/*convert the denom value into cents*/
unsigned denom_to_cents(enum denomination denom)
{
	if(denom == FIVE_CENTS)
	{
		return FIVECENTS;
	}
	else if(denom == TEN_CENTS)
	{
		return TENCENTS;
	}
	else if(denom == TWENTY_CENTS)
	{
		return TWENTYCENTS;
	}
	else if(denom == FIFTY_CENTS)
	{
		return FIFTYCENTS;
	}
	else if(denom == ONE_DOLLAR)
	{
		return ONEDOLLAR;
	}
	else if(denom == TWO_DOLLARS)
	{
		return TWODOLLARS;
	}
	else if(denom == FIVE_DOLLARS)
	{
		return FIVEDOLLARS;
	}
	else
	{
		return TENDOLLARS;
	}
}
/*check value of coin read in and set equivilent denomination*/
enum denomination cents_to_denom(int cents)
{	
	if(cents == TENDOLLARS)
	{
		return TEN_DOLLARS;
	}
	else if(cents == FIVEDOLLARS)
	{
		return FIVE_DOLLARS;
	}
	else if(cents == TWODOLLARS)
	{
		return TWO_DOLLARS;
	}
	else if(cents == ONEDOLLAR)
	{
		return ONE_DOLLAR;
	}
	else if(cents == FIFTYCENTS)
	{
		return FIFTY_CENTS;
	}
	else if(cents == TWENTYCENTS)
	{
		return TWENTY_CENTS;
	}
	else if(cents == TENCENTS)
	{
		return TEN_CENTS;
	}
	else
	{
		return FIVE_CENTS;
	}
}
/*initialise change_in_coins array to zero*/
void init_change(struct coin *change_in_coins)
{
	int i;
	for(i = ZERO; i < NUM_DENOMS; i++)
	{
		change_in_coins[i].count = 0;
		change_in_coins[i].denom = i;
	}
}
/*calculate the coins that need to be returned*/
void coinChange(unsigned int dollars, unsigned int cents, struct coin *change_in_coins, struct coin *cash_register)
{
	/*loop while dollar amount is greater than zero to return*/
	while(dollars != ZERO)
	{
		if(dollars >= 10 && cash_register[TEN_DOLLARS].count > ZERO)
		{
			dollars = dollars - 10;
			change_in_coins[TEN_DOLLARS].count++;
		}
		else if(dollars >= 5 && cash_register[FIVE_DOLLARS].count > ZERO)
		{
			dollars = dollars - 5;
			change_in_coins[FIVE_DOLLARS].count++;
		}
		else if(dollars >= 2 && cash_register[TWO_DOLLARS].count > ZERO)
		{
			dollars = dollars - 2;
			change_in_coins[TWO_DOLLARS].count++;
		}
		else if(dollars >= 1 && cash_register[ONE_DOLLAR].count > ZERO)
		{
			dollars = dollars - 1;
			change_in_coins[ONE_DOLLAR].count++;
		}
		else
		{
			printf("ERROR\n");
			break;
		}
	}
	/*loop while cents amount is still greater than zero*/
	while(cents != ZERO)
	{
		if(cents >= 50 && cash_register[FIFTY_CENTS].count > ZERO)
		{
			cents = cents - 50;
			change_in_coins[FIFTY_CENTS].count++;
		}
		else if(cents >= 20 && cash_register[TWENTY_CENTS].count > ZERO)
		{
			cents = cents - 20;
			change_in_coins[TWENTY_CENTS].count++;
		}
		else if(cents >= 10 && cash_register[TEN_CENTS].count > ZERO)
		{
			cents = cents - 10;
			change_in_coins[TEN_CENTS].count++;
		}
		else if(cents >= 5 && cash_register[FIVE_CENTS].count > ZERO)
		{
			cents = cents - 5;
			change_in_coins[FIVE_CENTS].count++;
		}
		else
		{
			printf("ERROR\n");
			break;
		}
	}
}
/*loop through the array and print out the change that needs to be returned*/
void printChange(struct coin *change_in_coins)
{
	int i;
	unsigned change;
	for(i = NUM_DENOMS - ONE; i >= ZERO; i--)
	{
		while(change_in_coins[i].count > ZERO) /*loop while theres multiple of same coin*/
		{
			change = denom_to_cents(change_in_coins[i].denom); /*get denom of coins*/
			if(change >= 100)	/*change into dollars*/
			{
				change = change/100;
				printf(" $%u", change);
			}
			else
			{
				printf(" %uc", change);
			}
			change_in_coins[i].count--; /*reduce count by one*/
		}
	}
}
/*loop through cash register array removing change given*/
void removeCoins(struct coin *change_in_coins, struct coin *cash_register)
{
	int i;
	for(i = ZERO; i < NUM_DENOMS; i++)
	{
		cash_register[i].count = cash_register[i].count - change_in_coins[i].count;
	}
}
