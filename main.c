/*
 * main.c - v1.14 - 18 December 2010
 * (c) Stevie Strickland, 1999-2012
 *
 * This program has been placed under the GPL.  Any bugfixes or enhancements
 * will be greatly appreciated :)
 *
 * Stevie Strickland - sstrickl@ccs.neu.edu
 */

/* For the abs() function */
#include <stdlib.h>

#include "rolldice.h"
#include "version.h"


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
    exit(EX_OK);
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
     
    if((temp_roll = malloc(sizeof(*temp_roll) * dice_nums[NUM_DICE])) == NULL) {
        perror("rolldice");
    	exit(EX_OSERR);
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
	    temp_int = SHRT_MAX;
	    for(k = 0; k < dice_nums[NUM_DICE]; k++)
		if(temp_int > temp_roll[k]) {
		    temp_int = temp_roll[k];
		    temp_index = k;
		}
	    if(print_separate) printf("- %d ", temp_int);
	    temp_total -= temp_int;
	    temp_roll[temp_index] = SHRT_MAX;
	}
	if(print_separate) printf(") ");
	if(dice_nums[MULTIPLIER] != 1) {
	    if(print_separate) printf("* %d ", dice_nums[MULTIPLIER]);
	    temp_total *= dice_nums[MULTIPLIER];
	}
	if(dice_nums[MODIFIER]) {
	    if(print_separate){
            if (dice_nums[MODIFIER] > 0)
                printf("+ %d ", dice_nums[MODIFIER]);
            else
                printf("- %d ", abs(dice_nums[MODIFIER]));

        }
	    temp_total += dice_nums[MODIFIER];
	}
	if(print_separate) printf("= ");
	printf("%d ", temp_total);
	if(print_separate) printf("\n");
    }
    if(!print_separate) printf("\n");
}

/* roll_from_stdin() - parse stdin, one roll by line
 * 
 * Parameters: None
 * Returns: EX_OK or EXIT_DATAERR (if bad stdin)
 */
int roll_from_stdin(){
     int *dice_nums = NULL;
     static char *line = (char *)NULL;

     line = readline("");
     while(line){
       dice_nums = parse_string( line );
       if ( dice_nums == NULL ) {
         return EX_DATAERR;
       }
       free(line);
       print_rolls(dice_nums);
       free(dice_nums);

       line = (char *)NULL;
       line = readline("");
     }
    return EX_OK;
}

/* roll_from_args() - parse command line args to roll dices
 * 
 * Parameters: args passed to CLI
 * Returns: EX_OK or EXIT_DATAERR (if bad command line)
 */
int roll_from_args(char **argv){
    int *dice_nums = NULL;
    int index;

    for(index = optind; argv[index] != NULL; index++) {
      dice_nums = parse_string( argv[index] );
      if ( dice_nums == NULL ) {
        return EX_DATAERR;
      }
     print_rolls(dice_nums);
     free(dice_nums);
    }
    return EX_OK;
}


int main(int argc, char **argv) {
    int c;

    while((c = getopt_long(argc, argv, "hvrus", long_opts, NULL)) != -1) {
      switch(c) {
      case 'h':
	print_usage(EX_OK); break;
      case 'v':
	print_version(); break;
      case 'r':
	if(rand_file == URANDOM) {
	  fprintf(stderr, "Choose either '-r' or '-u', please.\n");
	  return EX_USAGE;
	}
	rand_file = RANDOM; break;
      case 'u':
	if(rand_file == RANDOM) {
	  fprintf(stderr, "Choose either '-r' or '-u', please.\n");
	  return EX_USAGE;
	}
	rand_file = URANDOM; break;
      case 's':
	print_separate = 1; break;
      }
    }
    
    init_random(rand_file);
    
    if ( optind == argc ) {
      return roll_from_stdin();
    }
    else {
      return roll_from_args(argv);
    }
}
