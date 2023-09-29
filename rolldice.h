/*
 * rolldice.h - v1.14 - 18 December 2012
 * (c) Stevie Strickland, 1999-2012
 *
 * This program has been placed under the GPL.  Any bugfixes or enhancements
 * will be greatly appreciated :)
 *
 * Stevie Strickland - sstrickl@ccs.neu.edu
 */

/* Standard includes */
#include <stdio.h>
#include <stdlib.h>

/* For GNU getopt */
#include <getopt.h>

/* For GNU readline */
#include <readline/readline.h>

/* For exit values */
#include <sysexits.h>

/* For the time() function */
#include <time.h>

/* For the strstr() function */
#include <string.h>

/* For some bounds */
#include <limits.h>

/* For the isdigit() function */
#include <ctype.h>

/* The following #defines give the position of each important dice-related
 * number inside of the dice_nums array.  The final #define gives us the
 * size of the dice_nums array, which should be the number of other 
 * #defines below.
 */
#define NUM_ROLLS 0
#define NUM_DICE 1
#define NUM_SIDES 2
#define MULTIPLIER 3
#define MODIFIER 4
#define NUM_DROP 5
#define NUM_INITIALIZED 6
#define DICE_ARRAY_SIZE 7

/* The following #defines give the tokens for each part of the format
 * string.  Perhaps eventually I'll change parse_string to use strtok()
 * instead of strstr() :)
 */
#define ROLL_IDENT "x"
#define DICE_SIDES_IDENT "d"
#define MULTI_IDENT "*"
#define MOD_PLUS_IDENT "+"
#define MOD_MINUS_IDENT "-"
#define DROP_IDENT "s"

// Defines values for true and false, just for testing stuff boolean-wise :)
#define TRUE_VAL 1
#define FALSE_VAL 0

// Defines values for the random number file to use
typedef enum {UNDEF, URANDOM, RANDOM} rand_type;

// External function declarations for using rolldice() and kin.
extern int parse_string(char *dice_string, int *dice_nums);
extern int rolldie(int num_sides);
extern void init_random(rand_type rand_file);
