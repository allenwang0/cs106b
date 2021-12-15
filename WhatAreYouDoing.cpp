/* Allen Wang
 * CS 106B
 * Assignment 3 Problem 3
 */
#include "WhatAreYouDoing.h"
#include "GUI/SimpleTest.h"
#include "strlib.h"
#include "set.h"
#include "vector.h"
#include <cctype>
using namespace std;

void allEmphasesOfRec(Vector<string> upper, Vector<string> lower, int index, Vector<string> currSentence, Set<Vector<string>>& capSentences, const string& sentence);
Set<string> allEmphasesOfStrings(const Set<vector<string>>& capSentences);

//Returns the set of strings representing all the possible combinations of capitalization in the sentence.
Set<string> allEmphasesOf(const string& sentence) {
    Vector<string> words = tokenize(sentence);
    int sentenceLength = words.size();
    Vector<string> lowerCase;
    Vector<string> upperCase;
    for (int i = 0; i < sentenceLength; i++) {
        lowerCase += toLowerCase(words[i]);
        upperCase += toUpperCase(words[i]);
    }
    Vector<string> currSentence;
    Set<Vector<string>> capSentences;
    // The helper function is called to populate the above set of sentences (vectors.)
    allEmphasesOfRec(upperCase, lowerCase, 0, currSentence, capSentences, sentence);
    Set<string> strSentences = {};
    for (const Vector<string>& capSentence: capSentences) {
        // Creates a new empty sentence from the current vector.
        string sentCombination = "";
        for (const string& str: capSentence) {
            // Adds each word in the vector to the sentence.
            sentCombination += str;
        }
        // Adds the sentence to the set of sentences
        strSentences += sentCombination;
    }
    return strSentences;
}

// Returns a set of vectors of strings representing all the possible combinations of capitalization in a sentence.
void allEmphasesOfRec(Vector<string> upper, Vector<string> lower, int index, Vector<string> currSentence, Set<Vector<string>>& capSentences, const string& sentence) {
    // Base case is when the index (number of words considered) is equal to the total number of words in the sentence.
    // When the condition is met, the completed sentence (vector) is added to the set of sentences (vectors.)
    if (index == upper.size()) {
        capSentences += currSentence;
        return;
    }
    else if (isalpha(upper[index][0])) {
        // If the string is a word, the string is added to the current sentence vector.
        Vector<string> upperSentence = currSentence += upper[index];
        // The function runs through recursive calls, adding strings from the upper vector until the base case
        // and the complete uppercase sentence (vector) is added to the set.
        allEmphasesOfRec(upper, lower, index + 1, upperSentence, capSentences, sentence);
        // Each final word (uppercase string) is removed and replaced with the lowercase version of the word.
        currSentence.remove(currSentence.size()-1);
        Vector<string> lowerSentence = currSentence += lower[index];
        // The function is recursively called until each last word (depending on the current size of considered words)
        // is replaced and reaches the base case, adding each complete vector to the set of sentences.
        allEmphasesOfRec(upper, lower, index + 1, lowerSentence, capSentences, sentence);
    }
    else {
        // If the string is not a word, the string is added to current sentence vector.
        Vector<string> upperSentence = currSentence += upper[index];
        // The function is called again with the newly lengthened sentence vector.
        allEmphasesOfRec(upper, lower, index + 1, currSentence, capSentences, sentence);
    }
}




/* * * * * * Test Cases * * * * * */

/* TODO: Add your own tests here. You know the drill - look for edge cases, think about
 * very small and very large cases, etc.
 */

STUDENT_TEST("Student Test: Correctly returns an empty string") {
    Set<string> expected = {
        ""
    };

    EXPECT_EQUAL(allEmphasesOf(""), expected);
}

STUDENT_TEST("Student Test: Correctly returns a string of characters") {
    Set<string> expected = {
        "!@# $%^ &*("
    };

    EXPECT_EQUAL(allEmphasesOf("!@# $%^ &*("), expected);
}

STUDENT_TEST("Student Test: Enumerates all options in a longer case.") {
    Set<string> expected = {
        "die luft der freiheit weht",
        "die luft der freiheit WEHT",
        "die luft der FREIHEIT weht",
        "die luft der FREIHEIT WEHT",
        "die luft DER freiheit weht",
        "die luft DER freiheit WEHT",
        "die luft DER FREIHEIT weht",
        "die luft DER FREIHEIT WEHT",
        "die LUFT der freiheit weht",
        "die LUFT der freiheit WEHT",
        "die LUFT der FREIHEIT weht",
        "die LUFT der FREIHEIT WEHT",
        "die LUFT DER freiheit weht",
        "die LUFT DER freiheit WEHT",
        "die LUFT DER FREIHEIT weht",
        "die LUFT DER FREIHEIT WEHT",
        "DIE luft der freiheit weht",
        "DIE luft der freiheit WEHT",
        "DIE luft der FREIHEIT weht",
        "DIE luft der FREIHEIT WEHT",
        "DIE luft DER freiheit weht",
        "DIE luft DER freiheit WEHT",
        "DIE luft DER FREIHEIT weht",
        "DIE luft DER FREIHEIT WEHT",
        "DIE LUFT der freiheit weht",
        "DIE LUFT der freiheit WEHT",
        "DIE LUFT der FREIHEIT weht",
        "DIE LUFT der FREIHEIT WEHT",
        "DIE LUFT DER freiheit weht",
        "DIE LUFT DER freiheit WEHT",
        "DIE LUFT DER FREIHEIT weht",
        "DIE LUFT DER FREIHEIT WEHT",
    };

    EXPECT_EQUAL(allEmphasesOf("Die luft der freiheit weht"), expected);
}


/* * * * * * Test cases from the starter files below this point. * * * * * */

PROVIDED_TEST("Provided Test: Enumerates all options in a simple case.") {
    Set<string> expected = {
        "hello",
        "HELLO",
    };

    EXPECT_EQUAL(allEmphasesOf("Hello"), expected);
}

PROVIDED_TEST("Provided Test: Each option has the right length.") {
    string sentence = "Hello, world!";
    for (string option: allEmphasesOf(sentence)) {
        EXPECT_EQUAL(option.length(), sentence.length());
    }
}

PROVIDED_TEST("Provided Test: Enumerates all options in a more typical case.") {
    Set<string> expected = {
        "you are?",
        "you ARE?",
        "YOU are?",
        "YOU ARE?"
    };

    EXPECT_EQUAL(allEmphasesOf("You Are?"), expected);
}

PROVIDED_TEST("Provided Test: Stress Test: Recursion only branches on words.") {
    /* We're producing a string consisting of fifty copies of the * character. This tokenizes
     * into fifty separate stars. A star is the same whether or not you capitalize it - there
     * is no such thing as an "upper-case" or "lower-case" star. Therefore, your code should
     * not try to form two versions of the sentence, one with the star capitalized and one
     * without, because the two versions will end up being the same and the work to compute
     * both options will dramatically increase the runtime.
     *
     * For reference, if you do try branching the recursion and checking what happens both if
     * you capitalize the star and if you don't, you'll try exploring 2^50 different possible
     * capitalizations. That's 1,125,899,906,842,624 options, and even doing a billion of
     * these a second is going to take over two years to generate them all! And of course,
     * that's all wasted work, since there's only one way to capitalize this sentence, and
     * that's just to leave it as-is.
     *
     * If your code is hanging when this test is running, it likely means that your code is
     * trying to enumerate all of these options. See if you can edit your code so that, if
     * you're given a non-word token, you just leave it as-is and don't change anything.
     */
    string punctuation(50, '*'); // 50 copies of *

    /* The only emphasis is itself. */
    Set<string> expected = {
        punctuation
    };

    EXPECT_EQUAL(allEmphasesOf(punctuation), expected);
}
