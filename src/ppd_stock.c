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
#include "ppd_stock.h"

 /**
  * @file ppd_stock.c this is the file where you will implement the 
  * interface functions for managing the stock list.
  **/

void initalisePpdList(struct ppd_system *system)
{
	system->item_list = (struct ppd_list *) malloc (sizeof(struct ppd_list));	/*allocat memory for item list*/
	if(system->item_list == NULL)							/*check if memory was not set*/
	{
		printf("Out of Memory\n");
		exit(ONE);
	}

	system->item_list->head = NULL;				/*set head pointer to null*/
	system->item_list->count = ZERO;			/*set item count to zero*/
}

struct ppd_stock *newItem()					/*create new item*/
{
	struct ppd_stock *newItem;
	newItem = (struct ppd_stock *) malloc (sizeof(struct ppd_stock));	/*allocate memory for new item*/
	if(newItem == NULL)
	{
		printf("Can not allocate memory\n");
		exit(ONE);
	}
	return newItem;					/*return pointer to the new item*/
}

void addToList(struct ppd_list *item_list, struct ppd_stock *newStock)			/*add item to list in alphabetical order*/
{
	struct ppd_node *newNode = (struct ppd_node *)malloc(sizeof(struct ppd_node));		/*allocate memory for new node*/

	if(newNode == NULL)
	{
		printf("Unable to allocate memory for the new node\n");
		exit(ONE);
	}
	newNode->data = newStock;				/*assign the new stock to the node data*/
	newNode->next = NULL;					/*set the node next pointer to null*/
	if(item_list->head == NULL)
	{
		item_list->head = newNode;			/*add to start of the list*/
		printf("Added to blank list\n");
	}
	else if(strcmp(newStock->name, item_list->head->data->name) == ZERO || 
			strcmp(newStock->name, item_list->head->data->name) < ZERO)	/*check if name is equal or less than first item in list*/
	{
		newNode->next = item_list->head;
		item_list->head = newNode;		/*add item to start of the list*/
		printf("Added to the start of the list\n");
	}
	else
	{
		struct ppd_node *current = item_list->head, *previous = current;
		BOOLEAN added = FALSE;
		while(added != TRUE)	/*loop while item was not added to the list*/
		{
			/*if items name is earlier in alphabetical order than current node add before current node*/
			if(strcmp(newStock->name, current->data->name) == ZERO || 
					strcmp(newStock->name, current->data->name) < ZERO)
			{
				printf("Added before current node\n");
				newNode->next = current;
				previous->next = newNode;
				added = TRUE;
			}
			/*if theres no next node add item to end of the list*/
			else if(current->next == NULL)
			{
				current->next = newNode;
				printf("Added to the end of the list\n");
				added = TRUE;
			}
			/*move to next item and check spelling*/			
			else
			{
				previous = current;
				current = current->next;
				added = FALSE;
			}
		}
	}
	item_list->count++;			/*increment list counter by 1*/
}
/*print the node information in a formatted output*/
void printNode(struct ppd_stock *data, int size)
{
	printf("%-5s | %-*s | %-9u | $ %u.%02u\n", data->id, size, data->name, data->on_hand, data->price.dollars, data->price.cents);
}

/*display linked list*/
void displayList(struct ppd_system *system, int size)
{
	struct ppd_node *current;
	current = system->item_list->head;	
	while(current != NULL)
	{
		printNode(current->data, size);
		current = current->next;
	}
}
/*set each stock items on hand levels to default levels*/
void resetStockLevels(struct ppd_system *system)
{
	struct ppd_node *current = system->item_list->head;
	while(current != NULL)
	{
		current->data->on_hand = DEFAULT_STOCK_LEVEL;
		current = current->next;
	}
}
void generateID(struct ppd_system *system, char *itemID)	/*make a new item ID*/
{
	BOOLEAN match = FALSE, noMatch = FALSE;
	struct ppd_node *current = system->item_list->head;
	char newId[IDLEN];
	int num = ONE;
	
	sprintf(newId, "I000%d", num);		/*add num to end of string*/
	strcpy(newId, newId);			/*copy sting into array*/

	while(noMatch != TRUE)			/*loop while id is a match*/
	{
		while(current!= NULL)
		{
			if(strcmp(current->data->id, newId) == ZERO)	/*if id is a match*/
			{
				match = TRUE;
				num++;			/*add one to number*/
				break;
			}
			current = current->next;
		}
		if(match == FALSE)
		{
			strcpy(itemID, newId);		/*copy id to input pointer*/
			noMatch = TRUE;		
		}
		else
		{
			if(num < 10)
			{
				sprintf(newId, "I000%d", num);	/*if number is less than 2 didgets*/
				strcpy(newId, newId);		/*create new id to check if avalible*/
			}
			else if(num < 100)
			{
				sprintf(newId, "I00%d", num);	/*create id if the number is greater than 10*/
				strcpy(newId, newId);
			}
		}
		current = system->item_list->head;		/*set current data back to start of the list*/
		match = FALSE;					/*reset match to false*/
	}
}
/*save stock to file*/
void save_stock(struct ppd_system *system)
{
	FILE *fp;
	struct ppd_node *current = system->item_list->head;
	char nums[PRICE];
	fp = fopen(system->stock_file_name, "w");	/*open file for writing*/
	if(fp == NULL)		/*error message if file can not be opened for writting*/
	{
		printf("File '%s' could not be opened for writing.\n\n",system->stock_file_name);
		exit(EXIT_FAILURE);
	}
	printf("Writing stock to file");
	while(current != NULL)
	{
		printf(".");
		fputs(current->data->id, fp);
		fputs("|", fp);
		fputs(current->data->name, fp);
		fputs("|", fp);
		fputs(current->data->desc, fp);
		fputs("|", fp);
		sprintf(nums, "%d", current->data->price.dollars);
		fputs(nums, fp);
		fputs(".", fp);
		sprintf(nums, "%02d", current->data->price.cents);
		fputs(nums, fp);
		fputs("|", fp);
		sprintf(nums, "%d", current->data->on_hand);
		fputs(nums, fp);
		fputs("\n", fp);
		
		current = current->next;	/*point to next item*/
	}
	fclose(fp);
	printf("\n");
}
