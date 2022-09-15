/**
 * @file lexicon.h
 * @author Yousif Mansour
 * @date 2022-02-23
 * This program handles operations that work with the lexicon 
 * of words that the player can guess in wordle. lexicon can read a list
 * of words from a formatted file, can choose a random word from
 * the list of words, and can check to see if a word is in the list.
 * 
 */
#include <stdbool.h>

/** Maximum lengh of a word on the word list. */
#define WORD_LEN 5

/** Maximum number of words on the word list. */
#define WORD_LIMIT 100000

/**
 * Reads the words list from the file with name filename.
 * 
 * @param filename the filename that holds the input
 */
void readWords( char const filename[] );

/**
 * This function choosees a word from the current word list 
 * randomly using the given seed to generate a pseudorandom number.
 * 
 * @param seed seed used to generate number
 * @param word where the random word should be stored
 */
void chooseWord( long seed, char word[] );

/**
 * Checks if the given word is in the list of words.
 * 
 * @param word the word being searched for
 * @return true if the word exists
 * @return false if else
 */
bool inList( char const word[] );

/**
 * Sorts the list of words in alphabetical order.
 * Then checks if there are any duplicates in the list 
 * and exits with an error if there are any.
 */
void sort();
