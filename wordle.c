/**
 * @file wordle.c
 * @author Yousif Mansour - yamansou
 * @date 2022-02-27
 * 
 * A fully functioning implementation of the game of wordle, a game where a player
 * has to guess a random 5-letter target word by making guesses and learning more 
 * and more about how close their guess is to the target word. 
 * 
 * Takes two command-line arguments: <word-list-file> [seed-number]
 * 
 * word-list-file : Represents the list of words that are part of the lexicon of the current game. 
 *                    Must be a file containing only WORD_LEN long words seperated by line feeds.
 * 
 * seed-number : used to randomly select the target word chosen from the list of words.
 *                 must be a positive long integer.
 * 
 * Keeps track of the the number of guesses it took the player to win in a file named "scores.txt"
 * 
 */
#include "io.h"
#include "lexicon.h"
#include "history.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

/** The number of colors that can be printed */
#define NUM_COLORS 3

/** The index of the input-file name in the cmnd-line arguments array */
#define FILE_ARG_INDEX 1

/** The index of the seed in the cmnd-line arguments array */
#define SEED_ARG_INDEX 2

/**
 * Prints the correct usage for the command-line arguments and exits
 * with system failure
 */
static void printUsageError()
{
    fprintf( stderr, "usage: wordle <word-list-file> [seed-number]" );
    exit( EXIT_FAILURE );
}

/**
 * Process the user's guess using the provided rules of wordle.
 * Prints each character in the user's guess in the appropriate color.
 * @param userWord The word that the user guessed
 * @param targetWord The target word in this game of wordle
 *        precondtion: Both userWord and targetWord are valid words from the wordList
 */
static void processWord( char userWord[], char targetWord[] )
{    

    //arbitrary integers chosen to represent the three printable colors
    short const green = 0, yellow = 1, defaultColor = 2;

    //keeps track of the current color being printed 
    //effectively works as a state machine
    short currentColor = defaultColor;

    //Here, I generate a truth table for every character in the target word. 
    //Effectively, targetLetterUsed[ n ] being true means that the 
    //character targetWord[ n ] has a userWord character that is "tied" to it.
    //userWord characters become "tied" to a targetWord character when they are found equal
    //to a targetWord character. Being "tied" is a one-to-one relationship, meaning each targetWord 
    //character can only be tied to one userWord character. This is used to print out the exact 
    //number of green and yellow characters for targetWords and userWords with duplicate letters.
    
    //initailize the truth table to mark correctly guessed characters (green characters) as tied
    bool targetLetterUsed[ WORD_LEN ];
    for ( int i = 0; userWord[ i ]; i++ )
        targetLetterUsed[ i ] = userWord[ i ] == targetWord[ i ];

    //for every character that the user guessed
    for ( int i = 0; userWord[ i ]; i++ ) {

        //if it is in the correct position, print it green and go on to next character
        if ( userWord[ i ] == targetWord[ i ] ) {

            //sets current color to green if it isn't already and prints character
            if ( currentColor != green ) {
                colorGreen();
                currentColor = green;
            }

            putc( userWord[ i ], stdout );

            //go on to next character
            continue;
        }

        //keep track if character was printed yellow
        bool charYellowPrinted = false;

        //for all characters in the targetWord
        for ( int j = 0; targetWord[ j ]; j++ ) {

            //if the character that the user guessed is equal to a target in any 
            //position j AND if that target character is not already tied to a user character...
            if ( userWord[ i ] == targetWord[ j ] && !targetLetterUsed[ j ] ) {

                //mark that this target character is now tied
                targetLetterUsed[ j ] = true;

                //sets current color to yellow if it isn't already
                if ( currentColor != yellow ) {
                    colorYellow();
                    currentColor = yellow;
                }
                
                //prints character in yellow and flags that a yellow character has been printed
                putc( userWord[ i ], stdout );
                charYellowPrinted = true;

                break;
            }
        }  

        //if a yellow character was not printed, by process of elimination it is default color
        if ( !charYellowPrinted ) {

            //sets current color to default if it isn't already
            if ( currentColor != defaultColor ) {
                colorDefault();
                currentColor = defaultColor;
            }

            putc( userWord[ i ], stdout );
        }

    }

    //make sure to return to default color if last character printed was yellow or green
    if ( currentColor != defaultColor ) {
        colorDefault();
        currentColor = defaultColor;
    }

    //print a line-feed
    putc( '\n', stdout );
}

/**
 * Parses the second command-line argument, the seed used 
 * for randomization. Stores it in the seed address.
 * @param str String being parsed. Must be string of only digits [0-9]
 * @param seed the address where the seed should be stored
 */
static void getSeed( char *str, long *seed )
{
    
    //initialize the seed parsed
    *seed = 0;

    //parse all characters in string
    for ( int i = 0; str[ i ]; i++ ) {
        //get character from string
        char digit = str[ i ];

        //if character is not digit, is invalid
        if ( digit < NUMBER_0 || digit > NUMBER_9 )
            printUsageError();

        //shift the base over
        *seed *= BASE_10;

        //if integer being parsed goes negative, overflow error
        if ( *seed < 0 )
            printUsageError();

        //add the current digit to the seed
        *seed += ( digit - NUMBER_0 );
    }
}

/**
 * Starting point of the wordle game. Houses nearly all 
 * the game logic, including game-loops. Also properly handles 
 * command-line arguments.
 * @param argc the number of command-line arguments
 * @param argv the string array holding command-line arguments
 *             usage: wordle <word-list-file> [seed-number]
 * @return int exit status
 */
int main( int argc, char *argv[] )
{

    // check for proper usage
    if ( argc < FILE_ARG_INDEX + 1 || argc > SEED_ARG_INDEX + 1 )
        printUsageError();

    // read in the list of words using the 1st command-line argument
    readWords( argv[ FILE_ARG_INDEX ] );

    // initialize seed used for random word picking
    long seed;

    // if the user provided a seed, scan it
    if ( argc == SEED_ARG_INDEX + 1 )
        getSeed( argv[ SEED_ARG_INDEX ], &seed );

    // if not, generate seed using time
    else
        seed = time( NULL );

    // pick a random target word from the list of words
    char targetWord [ WORD_LEN + 1 ];
    chooseWord( seed, targetWord );

    //initialize and keep track of the user's number of guesses and 
    //initialize the pointer to the user's guess
    int numValidGuesses = 0;
    char userWord[ WORD_LEN + 1 ];

    //alphabetically sort the list of words to make word look ups faster
    sort();

    //continute to get valid guesses until the user guesses the word. 
    //This is the main game loop, where each loop represents every valid guess
    //a user makes. The user has not made a guess yet, initialize guessIsCorrect to 
    //false and continue looping until they have made the correct guess.
    bool guessIsCorrect = false;
    while ( !guessIsCorrect ) {

        //continue to get userWord until it is valid and in list
        //this is the second game loop, where each loop represents every single
        //guess a user makes, including invalid ones. The user has not made a guess yet, 
        //initialize wordIsValid to false and continue to looping until they have made 
        //a valid guess
        bool wordIsValid = false;
        while ( !wordIsValid ) { 

            //initialize (or reinitialize) userWord, userWordLen, and wordIsValid
            for ( int i = 0; i < WORD_LEN; i++ ) 
                userWord[ i ] = NULL_TERMINATOR;
            int userWordLen = 0;

            //assume the guess is valid everytime and try to prove that it is not
            wordIsValid = true;

            //read the word in character by character until a new line or EOF
            char letter;
            while ( ( letter = getc( stdin ) ) != '\n' && letter != EOF && letter != '\r' ) {

                //if character is not valid, then flag as invalid
                if ( letter < LOWERCASE_A || letter > LOWERCASE_Z )
                    wordIsValid = false;

                //concatenate the letter to the word only if within the bounds of the userWord array
                if ( userWordLen < WORD_LEN )
                    userWord[ userWordLen ] = letter;

                //continue to increment userWordLen anyway for later error-checking
                userWordLen++;

            }

            //if reached EOF or if user input "quit", then quit and output the targetWord
            if ( letter == EOF || strcmp( "quit", userWord ) == 0 ) {
                fprintf( stdout, "The word was \"%s\"\n", targetWord );
                exit( EXIT_SUCCESS );
            }

            //if userWord is not correct length, it is invalid
            if ( userWordLen != WORD_LEN )
                wordIsValid = false;

            //set wordIsValid to the intersection of it and the word being in the list
            //this shortcircuits if the word is already found to be invalid, slightly 
            //improving efficiency
            wordIsValid = wordIsValid && inList( userWord );

            //if word is invalid, output that it is invalid
            if ( !wordIsValid )
                fprintf( stdout, "Invalid guess\n" );

        }

        //guess is correct if userWord and targetWord are identical
        guessIsCorrect = strcmp( userWord, targetWord ) == 0;

        //process the word only if it is incorrect
        //(We do not print an all-green correct guess)
        if ( !guessIsCorrect )
            processWord( userWord, targetWord );

        //keep track of the number of valid guesses
        numValidGuesses++;

    } 

    //user has now guessed the correct word. 
    //print their number of guesses and update and print their score records
    fprintf( stdout, numValidGuesses == 1 ? "Solved in %d guess\n" : "Solved in %d guesses\n", numValidGuesses );
    updateScore( numValidGuesses );

    exit( EXIT_SUCCESS );

}
