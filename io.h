/**
 * @file io.h
 * @author Yousif Mansour - yamansou
 * @date 2022-02-23
 * 
 * This program deals with direct input/output manipulation. This 
 * program can read a single word from a file and store it in a string, 
 * and it can change the output text color to green, yellow, or default.
 * 
 */
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

/** Character value of the null terminator */
#define NULL_TERMINATOR '\0'

/** The character value of the char 'a' */
#define LOWERCASE_A 'a'

/** The character value of the char 'z' */
#define LOWERCASE_Z 'z'

/**
 * This function reads n number of characters from the file fp 
 * and stores those as a string with pointer str. If a file has a 
 * line that is not exactly n characters followed by a line-feed or EOF, 
 * exit the system with an error. 
 * @param fp the file being read from
 * @param str the pointer where the word will be stored
 * @param n the number of letters in the word
 * @return true if there are more lines to read
 * @return false if there are no more lines to read
 */
bool readLine( FILE *fp, char str[], int n );

/**
 * Outputs the ANSI Escape sequence for the color green
 */
void colorGreen();

/**
 * Outputs the ANSI Escape sequence for the color yellow
 */
void colorYellow();

/**
 * Outputs the ANSI Escape sequence for the terminal's default color
 */
void colorDefault();
