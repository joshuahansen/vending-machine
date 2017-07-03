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
#ifndef PPD_SHARED
#define PPD_SHARED

#define ID_INPUT 10 
#define EXTRA_CHARS 2
#define OPTIONS 3
#define PRICE 4
#define CONFIRM 3
#define ZERO 0
#define ONE 1
#define NAME 4

/*define coins and notes in cents*/
#define FIVECENTS 5
#define TENCENTS 10
#define TWENTYCENTS 20
#define FIFTYCENTS 50
#define ONEDOLLAR 100
#define TWODOLLARS 200
#define FIVEDOLLARS 500
#define TENDOLLARS 1000
/**
 * datatype to represent a boolean value within the system
 **/
typedef enum truefalse
{
    /**
     * the constant for false
     **/
    FALSE, 
    /**
     * the constant for true
     **/
    TRUE
} BOOLEAN;
#endif
