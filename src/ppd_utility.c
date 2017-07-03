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
/**
 * @file ppd_utility.c contains implementations of important functions for
 * the system. If you are not sure where to implement a new function, 
 * here is probably a good spot.
 **/

void read_rest_of_line(void)
{
    int ch;
    /* keep retrieving characters from stdin until we
     * are at the end of the buffer
     */
    while(ch = getc(stdin), ch!='\n' && ch != EOF)
        ;
    /* reset error flags on stdin */
    clearerr(stdin);
}

/**
 * @param system a pointer to a @ref ppd_system struct that holds all 
 * the data for the system we are creating
 **/
BOOLEAN system_init(struct ppd_system * system)
{
	system->stock_file_name = NULL;		/*set file name to null*/
	system->coin_file_name = NULL;		/*set coin name to null*/
	
	/*allocate memory to a new item list head*/
	system->item_list = (struct ppd_list *) malloc (sizeof(struct ppd_list));
	if(system->item_list == NULL)
	{
		printf("Out of Memory\n");
		exit(ONE);
	}
	system->item_list->head = NULL;		/*set head to point to NULL*/
	system->item_list->count = ZERO;	/*set list counter to 0*/

    return TRUE;
}

/**
 * loads the stock file's data into the system. This needs to be 
 * implemented as part of requirement 2 of the assignment specification.
 **/
BOOLEAN load_stock(struct ppd_system * system, const char *filename)
{
	FILE *fp;
	char line[1000 + ONE];
	char *token, *ptr;
	struct ppd_stock *newStock;	

	fp = fopen(filename, "r");	/*open file for reading*/
	if(fp == NULL)
	{
		printf("File '%s' could not be opened for reading. \n\n", filename);	
		exit(EXIT_FAILURE);
	}
	
	while(fgets(line, sizeof(line), fp) != NULL)	/*loop while file has more lines*/
	{	
		newStock = newItem();			/*create a new blank item*/
		token = strtok(line, "|");		/*get fisrt token & assign to id*/
		strcpy(newStock->id, token);
		token = strtok(NULL, "|");		/*second token assigned to name*/
		strcpy(newStock->name, token);
		token = strtok(NULL, "|");		/*third token assigned to description*/
		strcpy(newStock->desc, token);
		token = strtok(NULL, ".");		/*tokenize using . for price instead od |*/
		newStock->price.dollars = strtol(token, &ptr, 10);
		token = strtok(NULL, "|");
		newStock->price.cents = strtol(token, &ptr, 10);
		token = strtok(NULL, "|");		
		newStock->on_hand = strtol(token, &ptr, 10);	/*set last token of the line to on hand*/
	
		addToList((*system).item_list, newStock);	/*add item into the list*/
	}
	fclose(fp);

	return TRUE;
}

/**
 * loads the contents of the coins file into the system. This needs to
 * be implemented as part 1 of requirement 18.
 **/
BOOLEAN load_coins(struct ppd_system *system, const char *filename)
{
	
	FILE *fp;
	char line[1000 + ONE];
	char *token, *ptr;
	int denom;
	unsigned count;

	fp = fopen(filename, "r");	/*open file for reading*/
	if(fp == NULL)
	{
		printf("File '%s' could not be opened for reading. \n\n", filename);	
		exit(EXIT_FAILURE);
	}
	
	while(fgets(line, sizeof(line), fp) != NULL)
	{
		token = strtok(line, COIN_DELIM);
		denom = strtol(token, &ptr, 10);
		token = strtok(NULL, COIN_DELIM);
		count = strtol(token, &ptr, 10);

		add_coins(system, denom, count);
	}
	fclose(fp);

    return TRUE;

}

/**
 * @param system a pointer to a @ref ppd_system struct that holds all 
 * the data for the system we are creating
 **/
void system_free(struct ppd_system * system)
{
	struct ppd_node *current = system->item_list->head, *nextNode;

	while(current != NULL)			/*run while list has items*/
	{
		nextNode = current->next;	/*point to next node*/
		free(current->data);		/*free current nodes data*/
		free(current);			/*free current node*/
		current = nextNode;		/*move to next node*/
	}
	free(system->item_list);		/*free list pointer memory*/
}
/*search list and return pointer to its node*/
struct ppd_node *searchItems(struct ppd_system *system, char *selection)
{
	struct ppd_node *current = system->item_list->head;
	while(current != NULL)
	{
		if(strcmp(selection, current->data->id) == ZERO)	/*find matching id*/
		{
			break;
		}
		current = current->next;
	}
	return current;
}
/*calculate maximum length of an items name*/
int maxNameLen(struct ppd_system *system)
{
	int size = NAME;	/*set original length to length of name(4)*/
	struct ppd_node *current;
	current = system->item_list->head;
	while(current != NULL)
	{
		if(strlen(current->data->name) > size)		/*if length of name is greater than size make new max length*/
		{
			size = strlen(current->data->name);
		}
		current = current->next;
	}
	return size;	/*return size in of the longest name*/
}
