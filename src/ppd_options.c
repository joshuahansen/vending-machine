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

#include "ppd_utility.h"
#include "ppd_options.h"

/**
 * @file ppd_options.c this is where you need to implement the main 
 * options for your program. You may however have the actual work done
 * in functions defined elsewhere. 
 * @note if there is an error you should handle it within the function
 * and not simply return FALSE unless it is a fatal error for the 
 * task at hand. You want people to use your software, afterall, and
 * badly behaving software doesn't get used.
 **/

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this particular function should never fail.
 **/
BOOLEAN display_items(struct ppd_system * system)
{	
	int size;
	size = maxNameLen(system); /*get length of the longest name and set to size*/
	printf("Items Menu\n");
	printf("----------\n");
	printf("%-5s | %-*s | %-9s | %-8s\n", "ID", size, "Name", "Available", "Price"); /*print out menu in a formatted table*/
	printf("------------------------------------------------------\n");
	displayList(system, size);
    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true when a purchase succeeds and false when it does not
 **/
BOOLEAN purchase_item(struct ppd_system * system)
{
	char selection[IDLEN + EXTRA_CHARS];
	struct ppd_node *selectedItem;
	int exit;
	printf("Purchase Item\n");
	printf("-------------\n");
	printf("Please enter the id of the item you wish to purchase: ");	
	exit = fgets_details(selection, IDLEN + EXTRA_CHARS); 
	while(exit != EXIT)		/*loop while enter has not been pressed*/
	{
		if(exit == VALID)		/*run if the input is valid*/
		{
			selectedItem = searchItems(system, selection);		/*search for item*/
			if(selectedItem != NULL && selectedItem->data->on_hand > ZERO)
			{
				printf("You have selected \"%s %s\". This will cost you $%u.%02u.\n", 
				selectedItem->data->name, selectedItem->data->desc,		/*print out details of the selected items if greater than zero*/
				selectedItem->data->price.dollars, selectedItem->data->price.cents);
				payment(selectedItem, system);					/*take user payment*/
				break;	
			}
			else if(selectedItem != NULL && selectedItem->data->on_hand <= ZERO) /*print error message id there is no items avaliable*/
			{
				printf("The %s is not avaliable. Please select an avaliable item.\n",
				selectedItem->data->name);
				break;
			}
			else				/*error message if there was no matching item found*/
			{
				printf("No match was found\n");		
				printf("Please enter the id of the item you wish to purchase: ");
				exit = fgets_details(selection, IDLEN + EXTRA_CHARS); 
			}
		}
		else			/*error message if the input was to long.*/
		{
			printf("Please enter a shorter ID: ");
			exit = fgets_details(selection, IDLEN + EXTRA_CHARS);
		} 
	}
    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true when a save succeeds and false when it does not
 **/
BOOLEAN save_system(struct ppd_system * system)
{
	save_stock(system);
	save_coins(system);
	system_free(system);		/*call system_free that frees up the alocated memory of the list*/
    return FALSE;			/*return false to exit loop in main*/
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true when adding an item succeeds and false when it does not
 **/
BOOLEAN add_item(struct ppd_system * system)
{	/*create an array for each input tyoe*/
	char id[IDLEN + EXTRA_CHARS], name[NAMELEN + EXTRA_CHARS], desc[DESCLEN + EXTRA_CHARS], nums[PRICE + EXTRA_CHARS], *ptr, *token;
	unsigned int dollars, cents;
	struct ppd_stock *newStock;
	int exit = INVALID, numExit = ZERO;
	BOOLEAN check = FALSE;

	while(exit != VALID && exit != EXIT)		/*while input is not valid loop*/
	{
		printf("Enter the item name: ");
		exit = fgets_details(name, NAMELEN + EXTRA_CHARS);
	}
	if(exit == EXIT) 		/*exit system*/
	{
		return TRUE;
	}
	else				/*reset to invalid for next loop*/
	{
		exit = INVALID;
	}
	while(exit != VALID && exit != EXIT)		/*loop while description is not valid*/
	{
		printf("Enter the item description: ");
		exit = fgets_details(desc, DESCLEN + EXTRA_CHARS);
	}
	if(exit == EXIT)
	{
		return TRUE;
	}
	else
	{
		exit = INVALID;
	}
	while(exit != VALID && exit != EXIT)  /*loop while price is not correct*/
	{
		printf("Enter the price of this item: ");
		numExit = fgets_details(nums, PRICE + EXTRA_CHARS);
		if(numExit == EXIT)
		{
			exit = numExit;
		}
		if(numExit == VALID)
		{
			token = strtok(nums, ".");			/*tokenize price input*/
			dollars = strtol(token, &ptr, 10);		/*set first token to dollars*/
			token = strtok(NULL, "\n");
			if(token != NULL)			/*check to see if user entered a valid input*/
			{
				cents = strtol(token, &ptr, 10);		/*set number to coins*/
				check = (cents % 5 == ZERO);			/*check input to make sure price is valid coins return true*/
			}			
			if(check == TRUE)		/*if all input is valid exit while loop*/
			{
				exit = numExit;
			}
			printf("Invalid price. Please enter a price with valid coins\n");
		}
	}
	if(exit == EXIT)
	{
		return TRUE;
	}
	

	generateID(system, id);			/*create a new id for the product*/
	
	newStock = newItem();			/*create a new stock item*/

	strcpy(newStock->id, id);		/*assign values to new item*/
	strcpy(newStock->name, name);
	strcpy(newStock->desc, desc);
	newStock->price.dollars = dollars;
	newStock->price.cents = cents;
	newStock->on_hand = DEFAULT_STOCK_LEVEL;	/*set on hand to default levels*/
	
	addToList((*system).item_list, newStock);	/*add the items to the stock link list*/
	
    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true when removing an item succeeds and false when it does not
 **/
BOOLEAN remove_item(struct ppd_system * system)
{
	char selection[IDLEN + EXTRA_CHARS];
	struct ppd_node *current, *previous;		/*create two points to the list*/
	current = system->item_list->head;		/*set current to first item in list*/
	previous = current;
	printf("Remove Item\n");
	printf("-----------\n");
	printf("Enter the item id of the item you wish to remove from the menu: ");	
	if(fgets_details(selection, IDLEN + EXTRA_CHARS) == EXIT)		/*exit system if use presses enter*/
	{
		return TRUE;
	}
	while(current != NULL)			/*loop while current pointer is not equal to null*/
	{
		if(strcmp(selection, current->data->id) == ZERO)		/*check if current node id matches*/
		{
			if(current == system->item_list->head)			/*if the item is the first item remove it then point to next item*/
			{
				system->item_list->head = current->next;	
				printf("\"%s - %s %s\" has been removed from the system.\n", 
					current->data->id, current->data->name, current->data->desc);
				free(current->data);				/*free data memory*/
				free(current);					/*free current node memory*/
				system->item_list->count--;			/*reduce list count*/
				return TRUE;
			}
			else							
			{
				previous->next = current->next;					/*set previous node to point to next node*/
				printf("\"%s - %s %s\" has been removed from the system.\n", 
					current->data->id, current->data->name, current->data->desc);
				free(current->data);						/*free current nodes data memory*/
				free(current);							/*free node memory*/
				system->item_list->count--;					/*reduce list count by one*/
				return TRUE;
			}
		}
		previous = current;			/*move previous node to current node*/
		current = current->next;		/*move current node to next node*/
	}

    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this function cannot fail.
 **/
BOOLEAN reset_stock(struct ppd_system * system)
{
	char confirm[CONFIRM + EXTRA_CHARS];
	int exit;
 	printf("Do you want to reset all stock to defualt levels? y/n: ");		/*get confirmation to reset stock levels*/
	exit = fgets_details(confirm, CONFIRM + EXTRA_CHARS);
	if(exit == EXIT)
	{
		return TRUE;
	}
	else
	{
		if(strcmp(confirm, "y") == ZERO || strcmp(confirm, "Y") == ZERO)
		{
			printf("ALL ITEMS STOCK LEVELS SET TO DEFAULT\n");
			resetStockLevels(system);					/*set stock levels to default levels*/
			return TRUE;
		}
		else
		{
			return TRUE;
		}
	}
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this function cannot fail.
 **/
/*reset coins to the defualt values*/
BOOLEAN reset_coins(struct ppd_system * system)
{
    	char confirm[CONFIRM + EXTRA_CHARS];
	int exit;
 	printf("Do you want to reset all coins to defualt levels? y/n: ");		/*get confirmation to reset coins to default levels*/
	exit = fgets_details(confirm, CONFIRM + EXTRA_CHARS);
	if(exit == EXIT)
	{
		return TRUE;
	}
	else
	{
		if(strcmp(confirm, "y") == ZERO || strcmp(confirm, "Y") == ZERO)
		{
			printf("All coins have been reset to the default level of %d\n", DEFAULT_COIN_COUNT);
			resetCoinLevels(system);	/*set coins levels to default levels*/
			return TRUE;
		}
		else
		{
			return TRUE;
		}
	}
	return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this function cannot fail
 **/
BOOLEAN display_coins(struct ppd_system * system)
{
	int i;
	/*array of coin types for menu*/
	char denomination[NUM_DENOMS][10] = {"5 cents","10 cents","20 cents","50 cents","1 dollar","2 dollar","5 dollar","10 dollar"};
	/*create a table to print out coin summary*/
	printf("%s\n%s\n", "Coins Summary", "-------------");
	printf("%s | %s\n\n","Denomination", "Count");
	/*loop through the coin array printing out details*/
	for(i = ZERO; i < NUM_DENOMS; i++)
	{	
		printf("%-12s | %u\n", denomination[system->cash_register[i].denom], system->cash_register[i].count);
	}
    return TRUE;
}

BOOLEAN abort_program(struct ppd_system *system)
{
	printf("Program Aborted\n");
	system_free(system);			/*free system memory*/
	return FALSE;
}

int fgets_details(char *input, int bufferSize)
{
	fgets(input, bufferSize, stdin);		/*revieve in array and buffer size*/
	if(strcmp(input, "\n") == ZERO)
	{
		printf("Back to main menu\n");
		return EXIT;				/*exit input*/
	}
	else if(input[strlen(input) - ONE] != '\n')
	{
		printf("\nInput was to long. Please make it shorter\n");
		read_rest_of_line();				/*empty buffer*/
		return INVALID;					/*return invalid input*/
	}
	else
	{
		input[strlen(input) - ONE] = '\0';		/*set second last element to \0*/
		return VALID;					/*return valid input*/
	}
}

void payment(struct ppd_node *selectedItem, struct ppd_system *system)
{
	unsigned int inputMoney = ZERO, dollars, cents;
	int total, change = ZERO;
	struct coin change_in_coins[NUM_DENOMS];
	BOOLEAN exit = FALSE;
	init_change(change_in_coins);
	dollars = selectedItem->data->price.dollars;		/*get itmes price dollars*/
	cents = selectedItem->data->price.cents;		/*get item price cents*/
	total = (dollars * 100) + cents;			/*convert price into cents*/
	printf("Please hand over the money - type in the value of each note/coin in cents.\n");	
	printf("Press enter on a new and empty line to cancel this purchase:\n");
	exit = fgets_pay_input(&inputMoney, dollars, cents);
	while(exit != EXIT)				/*loop while not equal to exit*/
	{	
		if(exit != INVALID)
		{
			if(valid_coin(&inputMoney) == TRUE)
			{
				total = total - inputMoney;		/*remove input amount from total*/
				dollars = total/100;			/*convert back to dollars and cents*/
				cents = total - (dollars * 100);
				if(total <= ZERO)			/*if total is less than zero purchase item*/
				{
					printf("Thank you. Here is your %s", selectedItem->data->name);	
					if(total < ZERO)
					{
						change = total * - ONE;			/*calculate change*/
						dollars = change/100;
						cents = change - (dollars * 100);
						printf(", and your change of $%u.%02u", dollars, cents);
						if(system->coin_file_name != NULL)
						{
							printf(":");
							coinChange(dollars, cents, change_in_coins, system->cash_register);
							removeCoins(change_in_coins, system->cash_register);
							printChange(change_in_coins);
						}
						printf(".\nPlease come back soon.\n");
						selectedItem->data->on_hand--;			/*reduce data on hand by one*/
						break;
					}
					else						/*purchase item with now change*/
					{
						printf(".\nPlease come back soon.\n");
						selectedItem->data->on_hand--;
						break;
					}
				}
			}
			else		/*print error if input is invalid*/
			{
				printf("Error: %u is not a valid denomination of money\n", inputMoney);
			}
			inputMoney = ZERO;
		}
		exit = fgets_pay_input(&inputMoney, dollars, cents);		/*get next user input*/
	}
}

int fgets_pay_input(unsigned int *inputMoney, unsigned int dollars, unsigned int cents)
{
	/*get input for payment from user*/
	char input[PRICE + EXTRA_CHARS];
	char *inputPointer;
	printf("You still need to give us $%u.%02u: ", dollars, cents);
	fgets(input, sizeof(input), stdin);
	if(strcmp(input, "\n") == ZERO)
	{
		return EXIT;
	}
	else if(input[strlen(input) - ONE] != '\n')
	{
		printf("\nSelection was to long\n");
		read_rest_of_line();
		return INVALID;
	}
	else
	{
		input[strlen(input) - ONE] = '\0';
		*inputMoney = strtol(input, &inputPointer, 10);
		return VALID;
	}
}

/*check input is a valid coin denomination*/
BOOLEAN valid_coin(unsigned int *inputMoney)
{
	if(*inputMoney == FIVECENTS || *inputMoney == TENCENTS || *inputMoney == TWENTYCENTS ||
		*inputMoney == FIFTYCENTS || *inputMoney == ONEDOLLAR || *inputMoney == TWODOLLARS ||
		*inputMoney == FIVEDOLLARS || *inputMoney == TENDOLLARS)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
} 
