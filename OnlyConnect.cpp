/* File: OnlyConnect.cpp
 *
 * TODO: Edit these comments to describe anything interesting or noteworthy in your implementation.
 *
 * TODO: Edit these comments to leave a puzzle for your section leader to solve!
 *
 * Puzzle: SPRCLFRGLSTCXPLDCS
 *
 * Hint: Mary Poppins
 */
#include "OnlyConnect.h"
#include "GUI/SimpleTest.h"
#include "vector.h"
#include <cctype>
#include "strlib.h"
#include <iostream>
using namespace std;

bool isConsonant(char letter);
/*
 This function's base case is the empty string, and it returns the empty string as well.
 Next, the variable "letter" was declared that points to the first character in the string.
 The first recursive step is for when the first letter in the string is a consonant; if it is, it
 concatenates that letter with the previous output, which creates a new card, calling the function
 onlyConnectize again until it reaches the base case which is the empty string.
 If the following letter in a string is not a consonant, the function skips that letter, and the
 function is called again from the next letter, causing it to call itself again, creating new cards,
 until it reaches the base case.
 */
string onlyConnectize(string phrase) {
    phrase = toUpperCase(phrase);
    if (phrase.length() == 0) {
        return phrase;
    }
    char letter = phrase.at(0);
    if (isConsonant(letter)) {
        return (letter + onlyConnectize(phrase.substr(1)));
    }
    else {
        return onlyConnectize(phrase.substr(1));
    }
}

// The function below determines whether a particular character in a given string is a consonant, which can be
// determined by finding all the characters that 1) are a letter and 2) are not a vowel.
bool isConsonant(char letter) {
    letter = toUpperCase(letter);
    if (isalpha(letter) && letter != ('A') && letter != ('E') && letter != ('I') && letter != ('O') &&
        letter != ('U') && letter != ('Y')) {
            return true;
    }
    else {
        return false;
    }
}







/* * * * * * Provided Test Cases * * * * * */

PROVIDED_TEST("Converts lower-case to upper-case.") {
    EXPECT_EQUAL(onlyConnectize("lowercase"), "LWRCS");
    EXPECT_EQUAL(onlyConnectize("uppercase"), "PPRCS");
}

PROVIDED_TEST("Handles non-letter characters properly.") {
    EXPECT_EQUAL(onlyConnectize("2.718281828459045"), "");
    EXPECT_EQUAL(onlyConnectize("'Hi, Mom!'"), "HMM");
}

PROVIDED_TEST("Handles single-character inputs.") {
    EXPECT_EQUAL(onlyConnectize("A"), "");
    EXPECT_EQUAL(onlyConnectize("+"), "");
    EXPECT_EQUAL(onlyConnectize("Q"), "Q");
}



STUDENT_TEST("Handles the empty string.") {
    EXPECT_EQUAL(onlyConnectize(""), "");
}
STUDENT_TEST("Handles strings separated by spaces with correct spacing in the output.") {
    EXPECT_EQUAL(onlyConnectize("       h   e     l          l           o    "), "HLL");
}

/* TODO: You will need to add your own tests into this suite of test cases. Think about the sorts
 * of inputs we tested here, and, importantly, what sorts of inputs we *didn't* test here. Some
 * general rules of testing:
 *
 *    1. Try extreme cases. What are some very large cases to check? What are some very small cases?
 *
 *    2. Be diverse. There are a lot of possible inputs out there. Make sure you have tests that account
 *       for cases that aren't just variations of one another.
 *
 *    3. Be sneaky. Don't just try standard inputs. Try weird ones that you wouldn't expect anyone to
 *       actually enter, but which are still perfectly legal.
 *
 * Happy testing!
 */






