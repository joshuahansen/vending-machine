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

#include "ppd_main.h"
#include "ppd_menu.h"
#include "ppd_options.h"

/**
 * @file ppd_main.c contains the main function implementation and any 
 * helper functions for main such as a display_usage() function.
 **/

/**
 * manages the running of the program, initialises data structures, loads
 * data and handles the processing of options. The bulk of this function
 * should simply be calling other functions to get the job done.
 **/


int main(int argc, char **argv)
{
	BOOLEAN run = TRUE;
	menu_function func;
    /* uncomment this menu structure when you are ready to work on the 
     * menu system with function pointers*/
    struct menu_item menu[NUM_MENU_ITEMS];
    
	/* represents the data structures to manage the system */
    struct ppd_system system;
    (void)system;
	
    /* init the system */
	system_init(&system);
    /* validate command line arguments */
	system.stock_file_name = argv[1];
	if(system.stock_file_name == NULL)
	{
		printf("Not enough files loaded\n");
		return EXIT_SUCCESS;
	}
	if(argv[2] != NULL)
	{
		system.coin_file_name = argv[2];
		printf("%s loaded into system\n", system.coin_file_name);
		load_coins(&system, system.coin_file_name);
	}
    /* load data */
	load_stock(&system, system.stock_file_name);
    /* test if everything has been initialised correctly */

    /* initialise the menu system */
	init_menu(menu);
	
    /* loop, asking for options from the menu */
	while(run == TRUE)
	{
		mainMenu(menu);
		func = get_menu_choice(menu);
		run = (*func)(&system);
	}
    /* run each option selected */

    /* until the user quits */

    /* make sure you always free all memory and close all files 
     * before you exit the program
     */	
    return EXIT_SUCCESS;
}
