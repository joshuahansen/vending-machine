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

/**
 * @file ppd_coin.h defines the coin structure for managing currency in the
 * system. You should declare function prototypes for managing coins here
 * and implement them in ppd_coin.c
 **/
#ifndef PPD_COIN
#define PPD_COIN
#define COIN_DELIM ","
struct ppd_system;
/**
 * enumeration representing the various types of currency available in
 * the system. 
 **/
enum denomination
{
    FIVE_CENTS, TEN_CENTS, TWENTY_CENTS, FIFTY_CENTS, ONE_DOLLAR, 
    TWO_DOLLARS, FIVE_DOLLARS, TEN_DOLLARS
};

/**
 * represents a coin type stored in the cash register. Each demonination
 * will have exactly one of these in the cash register.
 **/
struct coin
{
    /**
     * the denomination type
     **/
    enum denomination denom;
    /**
     * the count of how many of these are in the cash register
     **/
    unsigned count;
};
void add_coins(struct ppd_system *system, int denom, unsigned count);
void resetCoinLevels(struct ppd_system *system);
void save_coins(struct ppd_system *system);
unsigned denom_to_cents(enum denomination denom);
enum denomination cents_to_denom(int cents);
void coinChange(unsigned int dollars, unsigned int cents, struct coin *change_in_coins, struct coin *cash_register);
void printChange(struct coin *change_in_coins);
void init_change(struct coin *change_in_coins);
void removeCoins(struct coin *change_in_coins, struct coin *cash_register);
#endif
