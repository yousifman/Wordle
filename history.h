/**
 * @file history.h
 * @author Yousif Mansour - yamansou
 * @date 2022-03-01
 * 
 * Maintains a scoreboard of the number of guesses it has taken the
 * user to guess the word for every game of wordle they have played.
 */

/** Character value of the number 0 */
#define NUMBER_0 '0'

/** Character value of the number 9 */
#define NUMBER_9 '9'

/** The integer number 10, used for parsing integers from strings */
#define BASE_10 10

/**
 * Reads in the current user score from "score.txt", 
 * Updates and prints their new scores, 
 * then saves their scores back into "score.txt"
 * @param guessCount number of guesses the it took the user to guess the word
 */
void updateScore( int guessCount );
