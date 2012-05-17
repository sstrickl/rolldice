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

// Local functions
int get_num_dice(int temp_int, int default_num);
int *get_num_sides(char *dice_string, int temp_int, int *res_int);
int *get_num_drop(char *dice_string, int temp_int, int *res_int);
int *get_num_rolls(int temp_int, int *res_int);
int *get_mutiplier(char *dice_string, int temp_int, int *res_int);
int *get_plus_modifier(char *dice_string, int temp_int, int *res_int);
int *get_minus_modifier(char *dice_string, int temp_int, int *res_int);
int is_too_big(int num);


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
	fprintf(stderr, "Error in reading random device!\n");
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
    int temp_int = -1, *dice_nums, *res_int;
    const int DEFAULT_NUM_DICE = 1;

    if((dice_nums = malloc ( DICE_ARRAY_SIZE * sizeof(int))) == NULL){
        perror("rolldice");
    	return NULL;
    }

    dice_nums[NUM_ROLLS] = 1;
    dice_nums[NUM_DICE] = DEFAULT_NUM_DICE;
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
            dice_nums[NUM_DICE] = get_num_dice(temp_int, DEFAULT_NUM_DICE);
            dice_string++;
            res_int = get_num_sides(dice_string, temp_int, res_int);
            if (res_int == NULL){
                free(dice_nums);
                return NULL;
            } else {
                dice_nums[NUM_SIDES] = *res_int;
            }
            break;
	    case 's':
            dice_string++;
            res_int = get_num_drop(dice_string, temp_int, res_int);
            if (res_int == NULL){
		        free(dice_nums);
                return NULL;
            } else {
                dice_nums[NUM_DROP] = *res_int;
            }
		    break;
	    case 'x':
            dice_string++;
            res_int = get_num_rolls(temp_int, res_int);
            if (res_int == NULL){
                free(dice_nums);
                return NULL;
            } else {
                dice_nums[NUM_ROLLS] = *res_int;
            }
            break;
	    case '*':
            dice_string++;
            res_int = get_mutiplier(dice_string, temp_int, res_int);
            if (res_int == NULL){
                free(dice_nums);
                return NULL;
            } else {
                dice_nums[MULTIPLIER] = *res_int;
            }
            break;
	    case '+':
            dice_string++;
            res_int = get_plus_modifier(dice_string, temp_int, res_int);
            if (res_int == NULL){
                free(dice_nums);
                return NULL;
            } else {
                dice_nums[MODIFIER] = *res_int;
            }
            break;
	    case '-':
            dice_string++;
            res_int = get_minus_modifier(dice_string, temp_int, res_int);
            if (res_int == NULL){
                free(dice_nums);
                return NULL;
            } else {
                dice_nums[MODIFIER] = *res_int;
            }
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

int get_num_dice(int temp_int, int default_num){
    if( (temp_int <= 0 ) || is_too_big(temp_int) )
        return default_num;
    else
        return temp_int;
}

int is_too_big(int num){
    return num >= MAXSHORT;
}

int *get_num_sides(char *dice_string, int temp_int, int *res_int){
    const char *PERCENT = "%";
    if(strncmp(dice_string, PERCENT, 1) == 0){
        temp_int = 100;
        res_int = &temp_int;
        return res_int;
    }
    else if( (sscanf(dice_string, "%d", &temp_int) < 1 ) ||
            (temp_int < 2) || is_too_big(temp_int) ) {
        return NULL;
    } else {
        res_int = &temp_int;
        return res_int;
    }
}

int *get_num_drop(char *dice_string, int temp_int, int *res_int){
    if( (sscanf(dice_string, "%d", &temp_int) < 1) ||
        (temp_int < 0) || is_too_big(temp_int) ) {
        return NULL;
    } else {
        res_int = &temp_int;
        return res_int;
    }
}

int *get_num_rolls(int temp_int, int *res_int){
    if( ( temp_int < 1 ) || is_too_big(temp_int) ) {
        return NULL;
    } else {
        res_int = &temp_int;
        return res_int;
    }
}

int *get_mutiplier(char *dice_string, int temp_int, int *res_int){
    if( (sscanf(dice_string, "%d", &temp_int) < 1) ||
        (temp_int < 0) || is_too_big(temp_int) ) {
        return NULL;
    } else {
        res_int = &temp_int;
        return res_int;
    }
}

int *get_plus_modifier(char *dice_string, int temp_int, int *res_int){
    if( (sscanf(dice_string, "%d", &temp_int) < 1) ||
        (temp_int < 0) || is_too_big(temp_int) ) {
        return NULL;
    } else {
        res_int = &temp_int;
        return res_int;
    }
}

int *get_minus_modifier(char *dice_string, int temp_int, int *res_int){
    if( (sscanf(dice_string, "%d", &temp_int) < 1) ||
        (temp_int < 0) || is_too_big(temp_int) ) {
        return NULL;
    } else {
        res_int = &temp_int;
        return res_int;
    }
}

