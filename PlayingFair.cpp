/* File: PlayingFair.cpp
 *
 * TODO: Edit these comments to describe anything interesting or noteworthy in your implementation.
 *
 */
#include "PlayingFair.h"
#include "GUI/SimpleTest.h"
#include "error.h"
using namespace std;

/*
 This function takes in the integer n and returns the A-sequence string with length 2^n. An input of a negative integer
 returns the error message. The inputs of 0 and 1 are the base cases, returning "A" and "AB", respectively.
 The recursive steps evaluate the output string by appending the final half of the previous string, then the
 intitial half of the previous string, respectively, to the entire previous string.
 */
string aSequenceOfOrder(int n) {
    if (n < 0) {
        error("ERROR: Negative input");
    }
    if (n == 0) {
        return "A";
    }
    if (n == 1) {
        return "AB";
    }
    else {
        string prevSequence = aSequenceOfOrder(n-1);
        int mid = prevSequence.length() / 2;
        return (prevSequence + prevSequence.substr(mid, prevSequence.length() - 1) + prevSequence.substr(0, mid));
    }
}

/*
 This function takes in the integer n and returns the B-sequence string with length 2^n. An input of a negative integer
 returns the error message. The inputs of 0 and 1 are the base cases, returning "B" and "BA", respectively.
 The recursive steps evaluate the output string by appending the final half of the previous string, then the
 intital half of the previous string, respectively, to the entire previous string.
 */
string bSequenceOfOrder(int n) {
    /* TODO: Delete this line and the next two lines, then implement this function. */
    if (n < 0) {
        error("ERROR: Negative input");
    }
    if (n == 0) {
        return "B";
    }
    if (n == 1) {
        return "BA";
    }
    else {
        string prevSequence = bSequenceOfOrder(n - 1);
        int mid = prevSequence.length() / 2;
        return (prevSequence + prevSequence.substr(mid, prevSequence.length() - 1) + prevSequence.substr(0, mid));
    }
}



/* * * * * * Provided Test Cases * * * * * */

PROVIDED_TEST("Sequences of order 3 are correct.") {
    /* Some very basic checks. */
    EXPECT_EQUAL(aSequenceOfOrder(3), "ABBABAAB");
    EXPECT_EQUAL(bSequenceOfOrder(3), "BAABABBA");
}

PROVIDED_TEST("Only characters should be As and Bs.") {
    for (int i = 0; i < 10; i++) {
        for (char ch: aSequenceOfOrder(i)) {
            EXPECT(ch == 'A' || ch == 'B');
        }
    }
}

PROVIDED_TEST("A-sequence of positive order should have equal As and Bs.") {
    /* Exclude the sequence of order 0, which is just a single character. */
    for (int i = 1; i < 10; i++) {
        int as = 0;
        int bs = 0;
        for (char ch: aSequenceOfOrder(i)) {
            if (ch == 'A') as++;
            else bs++;
        }

        EXPECT_EQUAL(as, bs);
    }
}

PROVIDED_TEST("Triggers error on negative inputs.") {
    /* The EXPECT_ERROR macro expects the given expression to call error(). Remember that
     * you need to guard against invalid inputs.
     */
    EXPECT_ERROR(aSequenceOfOrder(-137));
    EXPECT_ERROR(bSequenceOfOrder(-137));
}


STUDENT_TEST("Base cases are correct.") {
    EXPECT_EQUAL(aSequenceOfOrder(0), "A");
    EXPECT_EQUAL(bSequenceOfOrder(0), "B");
    EXPECT_EQUAL(aSequenceOfOrder(1), "AB");
    EXPECT_EQUAL(bSequenceOfOrder(1), "BA");
}

STUDENT_TEST("Large cases are correct.") {
    EXPECT_EQUAL(aSequenceOfOrder(9), "ABBABAABBAABABBABAABABBAABBABAABBAABABBAABBABAABABBABAABBAABABBABAABABBAABBABAABABBABAABBAABABBAABBABAABBAABABBABAABABBAABBABAABBAABABBAABBABAABABBABAABBAABABBAABBABAABBAABABBABAABABBAABBABAABABBABAABBAABABBABAABABBAABBABAABBAABABBAABBABAABABBABAABBAABABBABAABABBAABBABAABABBABAABBAABABBAABBABAABBAABABBABAABABBAABBABAABABBABAABBAABABBABAABABBAABBABAABBAABABBAABBABAABABBABAABBAABABBAABBABAABBAABABBABAABABBAABBABAABBAABABBAABBABAABABBABAABBAABABBABAABABBAABBABAABABBABAABBAABABBAABBABAABBAABABBABAABABBAABBABAAB");
    EXPECT_EQUAL(aSequenceOfOrder(10), "ABBABAABBAABABBABAABABBAABBABAABBAABABBAABBABAABABBABAABBAABABBABAABABBAABBABAABABBABAABBAABABBAABBABAABBAABABBABAABABBAABBABAABBAABABBAABBABAABABBABAABBAABABBAABBABAABBAABABBABAABABBAABBABAABABBABAABBAABABBABAABABBAABBABAABBAABABBAABBABAABABBABAABBAABABBABAABABBAABBABAABABBABAABBAABABBAABBABAABBAABABBABAABABBAABBABAABABBABAABBAABABBABAABABBAABBABAABBAABABBAABBABAABABBABAABBAABABBAABBABAABBAABABBABAABABBAABBABAABBAABABBAABBABAABABBABAABBAABABBABAABABBAABBABAABABBABAABBAABABBAABBABAABBAABABBABAABABBAABBABAABBAABABBAABBABAABABBABAABBAABABBAABBABAABBAABABBABAABABBAABBABAABABBABAABBAABABBABAABABBAABBABAABBAABABBAABBABAABABBABAABBAABABBAABBABAABBAABABBABAABABBAABBABAABBAABABBAABBABAABABBABAABBAABABBABAABABBAABBABAABABBABAABBAABABBAABBABAABBAABABBABAABABBAABBABAABABBABAABBAABABBABAABABBAABBABAABBAABABBAABBABAABABBABAABBAABABBABAABABBAABBABAABABBABAABBAABABBAABBABAABBAABABBABAABABBAABBABAABBAABABBAABBABAABABBABAABBAABABBAABBABAABBAABABBABAABABBAABBABAABABBABAABBAABABBABAABABBAABBABAABBAABABBAABBABAABABBABAABBAABABBA");
}

STUDENT_TEST("B-sequence of positive order should have equal As and Bs.") {
    for (int i = 1; i < 10; i++) {
        int as = 0;
        int bs = 0;
        for (char ch: bSequenceOfOrder(i)) {
            if (ch == 'B') bs++;
            else as++;
        }

        EXPECT_EQUAL(as, bs);
    }
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








