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

#include "ppd_menu.h"
/**
 * @file ppd_menu.c handles the initialised and management of the menu 
 * array
 **/

/**
 * @param menu the menu item array to initialise
 **/
void init_menu( struct menu_item* menu)
{
	int count;
	/*Array of names for menu options*/
	char names[NUM_MENU_ITEMS][MENU_NAME_LEN + ONE] = {"Display Items", "Purchase Items", "Save and Exit",
						 "Add Item", "Remove Item", "Display Coins", "Reset Stock", "Reset Coins", "Abort Program"};
	/*array of function pointers*/
	menu_function functions[] = {display_items, purchase_item, save_system, add_item, remove_item, display_coins, reset_stock, reset_coins, abort_program};
	/*loop through arrays and assign them to the menu_item struct*/
	for(count = ZERO; count < NUM_MENU_ITEMS; count++)
	{
		strcpy(menu[count].name, names[count]);
		menu[count].function = functions[count];
	}
}

/**
 * @return a menu_function that defines how to perform the user's
 * selection
 **/
menu_function get_menu_choice(struct menu_item *menu)
{
	/*get user input and return function pointer to that selection*/
	int inputSelection;
	fgets_menu(&inputSelection);
		
    return menu[inputSelection - ONE].function;
}
/*loop through array of menu_items and print out the name in a readible format*/
void mainMenu(struct menu_item *menu)
{	
	int count;
	printf("\n%s", "Main Menu\n");
	for(count = 0; count < OPTIONS; count++)
	{
		printf("%d. %s\n", count + ONE, menu[count].name);
	}
	printf("%s","Administrator-Only Menu:\n");
	for(count = OPTIONS; count < NUM_MENU_ITEMS; count++)
	{
		printf("%d. %s\n", count + ONE, menu[count].name);
	}
	printf("%s", "Select your option (1-9):\n");
}
/*buffer handling and input selection for menu choices*/
void fgets_menu(int *inputSelection)
{
	char input[MENU_INPUT + EXTRA_CHARS];
	char *inputPointer;

	fgets(input, sizeof(input), stdin);
	if(input[strlen(input) - ONE] != '\n')
	{
		printf("\nSelection was too long please only select 1-9\n");
		read_rest_of_line();
	}
	else
	{
		input[strlen(input) - ONE] = '\0';
		*inputSelection = strtol(input, &inputPointer, 10);
		if(*inputPointer != 0)
		{
			printf("Input was not numeric\n");
			*inputSelection = 20;
		}
	}
}
