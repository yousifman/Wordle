/**
 * @file io.c
 * @author Yousif Mansour - yamansou
 * @date 2022-02-23
 * 
 * This program deals with direct input/output manipulation. This 
 * program can read a single word from a file and store it in a string, 
 * and it can change the output text color to green, yellow, or default.
 * 
 */
#include "io.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

/** The ANSI Escape sequence for the color green */
static const char const green[] = { 0x1b, 0x5b, 0x33, 0x32, 0x6d, NULL_TERMINATOR };

/** The ANSI Escape sequence for the color yellow */
static const char const yellow[] = { 0x1b, 0x5b, 0x33, 0x33, 0x6d, NULL_TERMINATOR };

/** The ANSI Escape sequence for the defualt color */
static const char const defaultColor[] = { 0x1b, 0x5b, 0x30, 0x6d, NULL_TERMINATOR };

bool readLine( FILE *fp, char str[], int n )
{

    //initialize the character read from the file
    char ch;

    //read n number of letters and store it in our word
    for ( int i = 0; i < n; i++ ) {
        ch = getc( fp );

        //if the current letter is not a lowercase english letter file is invalid
        if ( ch < LOWERCASE_A || ch > LOWERCASE_Z ) {
            fprintf( stderr, "Invalid word file\n" );
            fclose( fp );
            exit( EXIT_FAILURE );
        }

        //build up the string
        str[ i ] = ch;
    }

    //Add the null terminator to the string
    str[ n ] = NULL_TERMINATOR;

    //read the next character
    ch = getc( fp );

    //if we have reached the end of file, return false indicating 
    //there are no more words to read
    if ( ch == EOF )
        return false;

    //if we have not reached the end of file, the next character must be a 
    //line-feed. If not, then exit with an error
    if ( ch != '\n' ) {
        fprintf( stderr, "Invalid word file\n" );
        fclose( fp );
        exit( EXIT_FAILURE );
    }

    //check the special case where there is a line-feed followed by an EOF. 
    //if it is EOF, return true indicating no more words in file
    ch = getc( fp );
    if ( ch == EOF )
        return false;
    
    //return the character to file if it was not EOF and return true indicating more words
    else {
        ungetc( ch, fp );
        return true;
    }

}

void colorGreen()
{
    printf( "%s", green );
}

void colorYellow()
{
    printf( "%s", yellow );
}

void colorDefault()
{
    printf( "%s", defaultColor );
}
