/**
 * @file history.c
 * @author Yousif Mansour - yamansou
 * @date 2022-03-01
 * 
 * Maintains a scoreboard of the number of guesses it has taken the
 * user to guess the word for every game of wordle they have played.
 */
#include "history.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

/** If it took the user more than this many guesses
    don't keep track of the exact number of guesses */
#define MAX_NUM_GUESSES 10

/** The character value of the ' ' character */
#define SPACE_CHAR ' '

/**
 * Gets the next integer in the file after skipping spaces.
 * 
 * !!!Does not do any error checking at all!!
 * !!!Assumes file has some number of spaces followed by digits!!!
 * 
 * @param file the file being read from, hopefully "scores.txt"
 * @return int the next integer in the scores file
 */
static int nextInt( FILE *file ) {
    
    //skip the spaces in the file
    char ch = getc( file );
    while( ch == SPACE_CHAR ) {
        ch = getc( file );
    }

    //parse the integer from the characters
    int returnValue = 0;
    while( ch >= NUMBER_0 && ch <= NUMBER_9 ) {
        returnValue = returnValue * BASE_10 + ( ch - NUMBER_0);
        ch = getc( file );
    }
    
    //return the integer
    return returnValue;

}

void updateScore( int guessCount ) {

    //set up the file scanner and creates the file if it does not exist
    FILE *fp; 
    if ( ( fp = fopen( "scores.txt" , "r" ) ) == NULL ) {

        //opening a file that does not exist for writing with fopen creates the file
        fp = fopen( "scores.txt", "w" );

        //set the scores to all zero and close the file writer
        fprintf( fp, "0 0 0 0 0 0 0 0 0 0");
        fclose( fp );

        //reopen the file, but for reading
        fp = fopen( "scores.txt", "r" );
    }

    //read the scores into an array of integers
    int scores[ MAX_NUM_GUESSES ];
    for ( int i = 0; i < MAX_NUM_GUESSES; i++ )
        scores[ i ] = nextInt( fp );

    //increment the guess count in the array of scores if the score is under MAX_NUM_GUESSES
    if ( guessCount < MAX_NUM_GUESSES )
        scores[ guessCount - 1 ]++;

    //but if it is equal to MAX_NUM_GUESSES or more, increment the last value in the scores array
    else 
        scores[ MAX_NUM_GUESSES - 1 ]++;

    //now that the scores have been read in and updated, 
    //close the reader and open the file for writing
    fclose( fp );
    FILE *fileWriter = fopen( "scores.txt", "w" );

    //print out the formatted first MAX_NUM_GUESSES - 1 rows of the scores table to stdout
    //and print the updated first MAX_NUM_GUESSES - 1 values to the scores file to fileWriter
    for (int i = 0; i < MAX_NUM_GUESSES - 1; i++ ) {
        fprintf( stdout, "%2d  : %4d\n", i + 1, scores[ i ] );
        fprintf( fileWriter, "%d ", scores[ i ] );
    }

    //print the final row and value to stdout and the file respectively
    fprintf( stdout, "%2d+ : %4d\n", MAX_NUM_GUESSES, scores[ MAX_NUM_GUESSES - 1 ] );
    fprintf( fileWriter, "%d\n", scores[ MAX_NUM_GUESSES - 1 ] );

}
