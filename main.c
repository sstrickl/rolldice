/*
 * main.c - v1.11 - 01 Feb 2010
 * (c) Stevie Strickland, 1999-2010
 *
 * This program has been placed under the GPL.  Any bugfixes or enhancements
 * will be greatly appreciated :)
 *
 * Stevie Strickland - sstrickl@ccs.neu.edu
 */

#include "rolldice.h"

/* The version number :) */
static const int MAJOR_VERSION = 1;
static const int MINOR_VERSION = 10;

/* The long options for this program in an struct option array */
struct option long_opts[] = {{"help", 0, NULL, 'h'},
			     {"version", 0, NULL, 'v'},
			     {"random", 0, NULL, 'r'},
			     {"urandom", 0, NULL, 'u'},
			     {"separate", 0, NULL, 's'}};

/* For getopt usage */
extern int optind;

/* Stores the random number file to use */
rand_type rand_file = UNDEF;

/* Should we print out the separate results of the dice rolls or not? */
static int print_separate;

/*
 * print_usage() - Prints the usage for rolldice
 *
 * Parameters: Value with which to exit program
 * Returns: none
 */
static void print_usage(int exitval) {
    fprintf(stderr, "Usage: rolldice [options] <dice string>\n");
    fprintf(stderr, "For dice string format and options, ");
    fprintf(stderr, "see rolldice(6).\n");
    exit(exitval);
}

/*
 * print_version() - Prints the version number
 *
 * Parameters: none
 * Returns: none
 */
static void print_version() {
    printf("rolldice, v%d.%d\n", MAJOR_VERSION, MINOR_VERSION);
    printf("Written by Stevie Strickland (sstrickl@ccs.neu.edu)\n");
    exit(EXIT_SUCCESS);
}

/* print_rolls() - Prints the rolls, either just the totals or the
 *                 separate rolls, also.
 * 
 * Parameters: Dice string with which to calculate dice rolls
 * Returns: None
 */
void print_rolls(int *dice_nums) {
    int i, j, k, temp_int, temp_index, temp_total;
    int* temp_roll;
    
    if(dice_nums == NULL) {
        fprintf(stderr, "Problems with the dice string, either malformed "
			"or numbers are too large,\nso quitting!\n");
	exit(EXIT_FAILURE);
    }
    
    if((temp_roll = malloc(sizeof(*temp_roll) * dice_nums[NUM_DICE])) == NULL) {
        perror("rolldice");
	exit(EXIT_FAILURE);
    }

    for(i = 0; i < dice_nums[NUM_ROLLS]; i++) {
	temp_total = 0;
	if(print_separate) printf("Roll #%d: (", i+1);
	for(j = 0; j < dice_nums[NUM_DICE]; j++) {
	    temp_roll[j] = rolldie(dice_nums[NUM_SIDES]);
	    if(print_separate) printf("%d ", temp_roll[j]);
	    temp_total += temp_roll[j];
	}
	for(j = 0; j < dice_nums[NUM_DROP]; j++) {
	    temp_int = MAXSHORT;
	    for(k = 0; k < dice_nums[NUM_DICE]; k++)
		if(temp_int > temp_roll[k]) {
		    temp_int = temp_roll[k];
		    temp_index = k;
		}
	    if(print_separate) printf("- %d ", temp_int);
	    temp_total -= temp_int;
	    temp_roll[temp_index] = MAXSHORT;
	}
	if(print_separate) printf(") ");
	if(dice_nums[MULTIPLIER] != 1) {
	    if(print_separate) printf("* %d ", dice_nums[MULTIPLIER]);
	    temp_total *= dice_nums[MULTIPLIER];
	}
	if(dice_nums[MODIFIER]) {
	    if(print_separate) printf("+ %d ", dice_nums[MODIFIER]);
	    temp_total += dice_nums[MODIFIER];
	}
	if(print_separate) printf("= ");
	printf("%d ", temp_total);
	if(print_separate) printf("\n");
    }
    if(!print_separate) printf("\n");
}

int main(int argc, char **argv) {
  
    int c, index, *totals, *dice_nums;
    
    dice_nums = NULL;

    while((c = getopt_long(argc, argv, "hvrus", long_opts, NULL)) != -1) {
      switch(c) {
      case 'h':
	print_usage(EXIT_SUCCESS); break;
      case 'v':
	print_version(); break;
      case 'r':
	if(rand_file == URANDOM) {
	  fprintf(stderr, "Choose either '-r' or '-u', please.\n");
	  return EXIT_FAILURE;
	}
	rand_file = RANDOM; break;
      case 'u':
	if(rand_file == RANDOM) {
	  fprintf(stderr, "Choose either '-r' or '-u', please.\n");
	  return EXIT_FAILURE;
	}
	rand_file = URANDOM; break;
      case 's':
	print_separate = 1; break;
      }
    }
    
    init_random(rand_file);
    
    if ( argc < 2 ) {
      print_usage(EXIT_FAILURE);
    }

    for(index = optind; argv[index] != NULL; index++) {
      dice_nums = parse_string( argv[index] );
      if ( dice_nums == NULL ) {
	fprintf(stderr, "%s: Failure in getting dice attributes\n", argv[0]);
	return EXIT_FAILURE;
      }
      print_rolls(dice_nums);
      free(dice_nums);
    }
    
    return EXIT_SUCCESS;
}
