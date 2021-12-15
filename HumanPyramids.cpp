/* Allen Wang
 * CS 106B
 * Assignment 3 Problem 2
 */
#include "HumanPyramids.h"
#include "GUI/SimpleTest.h"
#include "map.h"
using namespace std;

// Takes in row, column, and height of the pyramid, and populates a map linking position (vector of integers) to weight (double.)

double weightOnBackOfRec(int row, int col, int pyramidHeight, Map<Vector<int>, double>& weightValues) {
    if (row == 0) {
        return 0;
    }
    else if (weightValues.containsKey({row, col})) {
        return weightValues[{row, col}];
    }
    else {
        // Returns an error if the person is out of bounds.
        if (row > pyramidHeight - 1 || col > pyramidHeight - 1 || row < 0 || col < 0) {
            error("ERROR: INVALID INPUT");
        }
        // Computes weight held by people on the leftmost side of the pyramid.
        if (col == 0) {
            weightValues.put({row, col}, 80 + weightOnBackOfRec(row - 1, col, pyramidHeight, weightValues) / 2);
        }
        // Computes weight held by people on the rightmost side of the pyramid.
        else if (col == row) {
            weightValues.put({row, col}, 80 + weightOnBackOfRec(row - 1, col - 1, pyramidHeight, weightValues) / 2);
        }
        // Computes weight held by people in the middle.
        else {
            weightValues.put({row, col}, 160 + weightOnBackOfRec(row - 1, col, pyramidHeight, weightValues) / 2 +
                             weightOnBackOfRec(row - 1, col - 1, pyramidHeight, weightValues) / 2);
        }
        return weightValues.get({row, col});
    }
}

// Takes in a row, column, and height of pyramid, and returns the weight that particular person is holding.
double weightOnBackOf(int row, int col, int pyramidHeight) {
    Map<Vector<int>, double> weightValues;
    return weightOnBackOfRec(row, col, pyramidHeight, weightValues);
}




/* * * * * * Test Cases * * * * * */

/* TODO: Add your own tests here. You know the drill - look for edge cases, think about
 * very small and very large cases, etc.
 */

STUDENT_TEST("Checks person in row 1.") {
    /* Person M is located at row 1, column 1. */
    EXPECT_EQUAL(weightOnBackOf(0, 0, 1), 0);
}

STUDENT_TEST("Check Person M from the handout.") {
    /* Person M is located at row 5, column 3. */
    EXPECT_EQUAL(weightOnBackOf(4, 2, 5), 500);
}






/* * * * * * Test cases from the starter files below this point. * * * * * */

PROVIDED_TEST("Check Person E from the handout.") {
    /* Person E is located at row 2, column 1. */
    EXPECT_EQUAL(weightOnBackOf(2, 1, 5), 240);
}

PROVIDED_TEST("Function reports errors in invalid cases.") {
    EXPECT_ERROR(weightOnBackOf(-1, 0, 10));
    EXPECT_ERROR(weightOnBackOf(10, 10, 5));
    EXPECT_ERROR(weightOnBackOf(-1, 10, 20));
}

PROVIDED_TEST("Stress test: Memoization is implemented.") {

    /* This will take a LONG time to complete if memoization isn't implemented.
     * We're talking "heat death of the universe" amounts of time. :-)
     */
    EXPECT(weightOnBackOf(100, 50, 200) >= 10000);
}
