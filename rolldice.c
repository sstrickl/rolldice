/**
 * rolldice.c - v1.11 - 01 Feb 2010
 * (c) Stevie Strickland, 1999-2012
 * 
 * This program has been placed under the GPL.  Any bugfixes or enhancements
 * will be greatly appreciated :)
 *
 * Stevie Strickland - sstrickl@ccs.neu.edu
 */

#include "rolldice.h"

// File pointer for random device
static FILE* ran_dev;

void init_random(rand_type rand_file) {
    if(rand_file == RANDOM) {
	if((ran_dev = fopen("/dev/random", "r")) == NULL) {
	    fprintf(stderr, "Error in opening /dev/random!\n");
	    exit(EXIT_FAILURE);
	}
    }
    else if((ran_dev = fopen("/dev/urandom", "r")) == NULL) {
	fprintf(stderr, "Error in opening /dev/urandom!\n");
	exit(EXIT_FAILURE);
    }
}

static int get_random(int sides) {
    unsigned int ret_value;
  
    if(!(fread(&ret_value, sizeof(unsigned int), 1, ran_dev) == 1)) {
	printf("Error in reading random device!\n");
	exit(EXIT_FAILURE);
    }
    return (int)(ret_value % sides);
}

/* rolldie() - Rolls a single die
 *
 * Parameters: int num_sides - number of sides of the die to roll
 * Returns: int - the result of the roll
 */
int rolldie ( int num_sides ) {

    return (1 + get_random(num_sides));
}

/* parse_string() - Parses a string for dice rolling attributes
 *
 * Parameters: char *dice_string - string to parse
 * Returns: int * - array of nums describing the different aspects of the 
 * dice to be rolled
 */
int *parse_string(char *dice_string) {
    int temp_int = -1, *dice_nums;

    if((dice_nums = malloc ( DICE_ARRAY_SIZE * sizeof(int))) == NULL)
	return NULL;

    dice_nums[NUM_ROLLS] = 1;
    dice_nums[NUM_DICE] = 1;
    dice_nums[NUM_SIDES] = 6;
    dice_nums[MULTIPLIER] = 1;
    dice_nums[MODIFIER] = 0;
    dice_nums[NUM_DROP] = 0;

    while(*dice_string != '\0') {
	if( isdigit(*dice_string) ) {
	    sscanf(dice_string, "%d", &temp_int);
	    while(isdigit(*(++dice_string)));
	}
	else {
	    switch(*dice_string) {
	    case 'd':
		if(temp_int > 0 && temp_int < MAXSHORT)
		    dice_nums[NUM_DICE] = temp_int;
		dice_string++;
		if(*dice_string == '%')
		    dice_nums[NUM_SIDES] = 100;
		else if( (sscanf(dice_string, "%d", &temp_int) < 1 ) ||
			 (temp_int < 2) || (temp_int >= MAXSHORT) ) {
		    free(dice_nums);
		    return NULL;
		} else dice_nums[NUM_SIDES] = temp_int;
		break;
	    case 's':
		if( (sscanf(++dice_string, "%d", &temp_int) < 1) ||
		    (temp_int < 0) || (temp_int >= MAXSHORT)) {
		    free(dice_nums);
		    return NULL;
		} else dice_nums[NUM_DROP] = temp_int;
		break;
	    case 'x':
		if( ( temp_int < 1 ) || (temp_int >= MAXSHORT) ) {
		    free(dice_nums);
		    return NULL;
		} else dice_nums[NUM_ROLLS] = temp_int;
		dice_string++;
		break;
	    case '*':
		if( (sscanf(++dice_string, "%d", &temp_int) < 1) ||
		    (temp_int < 0) || (temp_int >= MAXSHORT)) {
		    free(dice_nums);
		    return NULL;
		} else dice_nums[MULTIPLIER] = temp_int;
		break;
	    case '+':
		if( (sscanf(++dice_string, "%d", &temp_int) < 1) ||
		    (temp_int < 0) || (temp_int >= MAXSHORT)) {
		    free(dice_nums);
		    return NULL;
		} else dice_nums[MODIFIER] = temp_int;
		break;
	    case '-':
		if( (sscanf(++dice_string, "%d", &temp_int) < 1) ||
		    (temp_int < 0) || (temp_int >= MAXSHORT)) {
		    free(dice_nums);
		    return NULL;
		} else dice_nums[MODIFIER] = -temp_int;
		break;
	    default:
		dice_string++;
		break;
	    }
	    temp_int = 0;
	}
    }
    
    return dice_nums;
}




