/**
 * @file lexicon.c
 * @author Yousif Mansour - yamansou
 * @date 2022-02-23
 * 
 * This program handles operations that work with the lexicon 
 * of words that the player can guess in wordle. lexicon can read a list
 * of words from a formatted file, can choose a random word from
 * the list of words, and can check to see if a word is in the list.
 * 
 */
#include "lexicon.h"
#include "io.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/** Large prime multiplier used to choose a word pseudo-randomly. */
#define MULTIPLIER 4611686018453

/** Initial capacity of the word list */
#define INITIAL_CAPACITY 10

/** The global list of all words */
static char **wordList;

/** The global length of the list of words */
static int wordListLen;

/**
 * Implements the binary search algorithm to quickly search for words in the list.
 * Recursivley searches through wordList from low to high index. Cuts off halves of the 
 * list if the element is not in that half. Instance size is halved everytime -> O(logn)
 * @param word the target word being searched for in the list
 * @param low the lowest index being considred in wordList
 * @param high the highest index being considered in wordList
 * @return true if the word exists in wordList
 * @return false if the word does not exist in wordList
 */
static bool binarySearch( char const word[], long low, long high )
{
    //if low > high, then entire list has been searched
    //and key was not found (base case)
    if ( low > high )
        return false;
    
    //else compare key to middle element (recursive case)
    else {
        long mid = ( low + high ) / 2;
        int result = strcmp( wordList[ mid ], word );

        //if they're equal, return true
        if ( result == 0 )
            return true;
        
        //if middle element is greater than word, word is in left
        else if ( result > 0 )
            return binarySearch( word, low, mid - 1 );

        //vice versa
        else 
            return binarySearch( word, mid + 1, high );
    }
}

/**
 * MergeSort's helper merger function. Merges all the elements from 
 * left and right in sorted order into the list. 
 * 
 * @param left left half being sorted
 * @param leftLength number of elements in left half
 * @param right right half being sorted
 * @param rightLength number of elements in right half
 * @param list the list that will be sorted 
 * @param n the number of total elements in the sorted list
 */
static void merge( char **left, long leftLength, char **right, long rightLength, char **list, long n )
{
    //initialized the indices of the left and right halves being sorted
    long leftIndex = 0, rightIndex = 0;

    //while there are still elements to process..
    while ( leftIndex + rightIndex < n ) {

        //if the right list has been fully processed OR if the left list still has elements AND the current 
        //left element is less than the next right element, take element from left half
        if ( rightIndex == rightLength || ( leftIndex < leftLength && strcmp( left[ leftIndex ], right[ rightIndex ] ) < 0 ) ) {
            list[ leftIndex + rightIndex ] = left[ leftIndex ];
            leftIndex++;

        //else take element from right half
        } else {
            list[ leftIndex + rightIndex ] = right[ rightIndex ];
            rightIndex++;
        }
    }
}

/**
 * Creates a new sub-array from the given origList. this sub-array is a copy 
 * of the elements from origList from indices start - end (inclusive).
 * @param origList the list being copied
 * @param start the index of the first element to be copied
 * @param end the index of the last element to be copied
 * @return char** reference to the new sub-array
 */
static char **copyArray( char **origList, long start, long end )
{
    //calculate length of new list and allocate enough memory for it
    long newListLen = end - start + 1;
    char **newList = malloc( newListLen * sizeof(char *) );

    //copy elements from the original list into the new list
    for ( int i = 0; i < newListLen; i++ )
        newList[ i ] = origList[ i + start ];

    //return the new list
    return newList;
}

/**
 * Implementation of MergeSort algorithm. 
 * Recursively splits list of strings in half until the base case of a list 
 * that contains only one string, then merges all the lists in sorted order.
 * @param list the list of strings to be sorted
 * @param n the length of that list
 * @return char** a pointer to the new sorted list of strings
 */
static void mergeSort( char **list, long n )
{

    //base case of one element in list is "sorted"
    if ( n != 1 ) {

        //calcualte middle index and length of left and right halves
        long mid = n / 2;
        long leftLength = mid;
        long rightLength = n % 2 == 0 ? mid : mid + 1;

        //make copies of the left and right halves of the list
        char **left = copyArray( list, 0, mid - 1 );
        char **right = copyArray( list, mid, n - 1 );

        //recursively mergeSort each half until halves are 1 element each
        mergeSort( left, leftLength );
        mergeSort( right, rightLength );

        //merge each half together in sorted order
        merge( left, leftLength, right, rightLength, list, n );

        //free up memory with unneeded left and right lists
        free( left );
        free( right );
    }
}

void readWords( char const filename[] )
{

    //set up the file scanner and exit if cannot open
    FILE *fp; 
    if ( ( fp = fopen( filename, "r" ) ) == NULL ) {
        fprintf( stderr, "Can't open the word list: %s\n", filename );
        exit( EXIT_FAILURE );
    }

    //initialize the word list as a pointer to char pointers (array of strings)
    //with the initial capacity of 10 char pointers, and a length of zero
    wordList = (char **) malloc( INITIAL_CAPACITY * sizeof(char *) );
    wordListLen = 0;
    int capacity = INITIAL_CAPACITY;

    //continue to scan string as long as there are more strings in the file
    //using this boolean flag allows the program to still execute the loop one 
    //more time if readLine returns false.
    bool getAnotherLine = true;
    while( getAnotherLine ) {

        //the length of any string in the array is word_len + 1 (for the null terminator at the end)
        char *str = (char *) malloc( ( WORD_LEN + 1 ) * ( sizeof(char) ) );

        //read in a word into str and flag if another line should be read
        getAnotherLine = readLine( fp, str, WORD_LEN );

        //if the array of strings is at capacity, double its capacity or 
        //set the capacity to the word limit, whichever is lower
        if ( wordListLen == capacity ) {
            capacity = capacity > WORD_LIMIT / 2 ? WORD_LIMIT : capacity * 2;
            wordList = (char **) realloc( wordList, capacity * sizeof( wordList[ 0 ] ) );
        }

        //if the list's length is at the word limit, exit
        if ( wordListLen == WORD_LIMIT ) {
            fprintf( stderr, "Invalid word file\n" );
            fclose( fp );
            exit( EXIT_FAILURE );
        }

        //add the string from readLine into the list
        wordList[ wordListLen++ ] = str;
    }

}

void chooseWord( long seed, char word[ WORD_LEN + 1 ] )
{
    //calculate random index using given randomization formula
    //and pick the random word
    long randomIndex = ( seed % wordListLen ) * MULTIPLIER % wordListLen;
    char *chosenWord = wordList[ randomIndex ];

    //copy the chosen word to the given word
    for ( int i = 0; i <= WORD_LEN; i++ )
        word[ i ] = chosenWord[ i ];
    
}

bool inList( char const word[] )
{
    //calls binary search recursive algorithm with starting paramters
    return binarySearch( word, 0, wordListLen - 1 );
}

void sort()
{

    //call the mergeSort recursive algorithm with the starting parameters
    mergeSort( wordList, wordListLen );

    //checking for dupliactes in a sorted list entails 
    //checking if neighbors are identical, hence it is O(n)
    for ( int i = 0; i < wordListLen - 1; i++ ) {
        if ( strcmp( wordList[ i ], wordList[ i + 1 ] ) == 0 ) {
            fprintf( stderr, "Invalid word file\n" );
            exit( EXIT_FAILURE );
        }
    }

}
