/* Allen Wang
 * CS 106B
 * Assignment 3 Problem 4
 */
#include "ShiftScheduling.h"
#include "GUI/SimpleTest.h"
#include "vector.h"
#include "error.h"
using namespace std;

Set<Shift> highestValueScheduleForRec(const Set<Shift>& chosenShifts, const Set<Shift>& maybeShifts, int maxHours);
int scheduleValue(Set<Shift> Schedule);

// Takes in a set of shifts and maximum hours worked and returns which legal set of shifts has the highest value.
Set<Shift> highestValueScheduleFor(const Set<Shift>& shifts, int maxHours) {
    if (maxHours < 0) {
        error("ERROR: NEGATIVE NUMBER OF HOURS");
    }
    if (shifts.isEmpty()) {
        return {};
    }
    Set<Shift> chosenShifts = {};
    Set<Shift> maybeShifts = shifts;
    return highestValueScheduleForRec(chosenShifts, maybeShifts, maxHours);
}

// Takes in a shifts chosen, shifts to be chosen, and the maximum hours then returns which legal set of shifts has the highest value.
Set<Shift> highestValueScheduleForRec(const Set<Shift>& chosenShifts, const Set<Shift>& maybeShifts, int maxHours) {
    // Returns if the number of hours exceeds the worker's maximum hours or there are no more shifts to consider.
    if (maxHours == 0 || maybeShifts.size() == 0) {
        return chosenShifts;
    }
    Shift currShift = maybeShifts.first();
    // Loops through already chosen string and if the current shift overlaps with any of the already chosen shifts,
    // breaks then run through the recursion again.
    for (Shift chosenShift: chosenShifts) {
        if (overlapsWith(currShift, chosenShift)) {
            return highestValueScheduleForRec(chosenShifts, maybeShifts - currShift, maxHours);
        }
    }
    // If the considered shift exceeds the number of hours left, recurse through all the possiblities that do not include that shift.
    if (lengthOf(currShift) > maxHours) {
        return highestValueScheduleForRec(chosenShifts, maybeShifts - currShift, maxHours);
    }
    else {
        // Creates the set of shifts that include the current shift.
        Set<Shift> withSchedule = highestValueScheduleForRec(chosenShifts + currShift, maybeShifts - currShift, maxHours - lengthOf(currShift));
        // Creates the set of of shifts that do not include the current shift.
        Set<Shift> withoutSchedule = highestValueScheduleForRec(chosenShifts, maybeShifts - currShift, maxHours);
        // Returns whichever schedule (include or exclude) that has the greater value.
        if (scheduleValue(withSchedule) > scheduleValue(withoutSchedule)) {
            return withSchedule;
        }
        return withoutSchedule;
    }
}

// Takes in a set of shifts then returns the value of that set.
int scheduleValue(Set<Shift> schedule) {
    int schedValue = 0;
    for (Shift currShift: schedule) {
        schedValue += valueOf(currShift);
    }
    return schedValue;
}








/* * * * * * Test Cases * * * * * */
/*See below
 */




/* * * * * * Test cases from the starter files below this point. * * * * * */

/* This nice utility function lets you call highestValueScheduleFor, passing in
 * a Vector of shifts rather than a Set. This makes it a bit easier to test things.
 * You shouldn't need this function outside of these test cases.
 */
Set<Shift> asSet(const Vector<Shift>& shifts) {
    Set<Shift> result;
    for (Shift s: shifts) {
        result += s;
    }
    return result;
}

STUDENT_TEST("Picks correct shfit if one shift provides more value than the rest of the shifts combined.") {
    Vector<Shift> shifts = {
        { Day::MONDAY,     4, 18, 99 },  // Mon  4AM - 6PM, value 99 *
        { Day::TUESDAY,    12, 16, 5 },  // Mon 12PM -  4PM, value 0
        { Day::WEDNESDAY,  16, 20, 5 },  // Mon  4PM -  8PM, value 0
        { Day::THURSDAY,    8, 14, 5 },  // Mon  8AM -  2PM, value 0
        { Day::FRIDAY,     14, 20, 5 },  // Mon  2PM -  8PM, value 0
    };

    /* Get back the solution. */
    Set<Shift> computedSolution = highestValueScheduleFor(asSet(shifts), 15);

    /* Form the correct answer. It's the starred entries. */
    Set<Shift> actualSolution = {
        shifts[0]
    };

    EXPECT(computedSolution == actualSolution);
}

PROVIDED_TEST("Pick only shift if you have time for it.") {
    Set<Shift> shifts = {
        { Day::MONDAY, 9, 17, 1000 },  // Monday, 9AM - 5PM, value is 1000
    };

    EXPECT(highestValueScheduleFor(shifts, 24).size() == 1);
    EXPECT(highestValueScheduleFor(shifts, 24) == shifts);
}

PROVIDED_TEST("Don't pick only shift if you don't have time for it.") {
    Set<Shift> shifts = {
        { Day::MONDAY, 9, 17, 1000 },  // Monday, 9AM - 5PM, value is 1000
    };

    EXPECT(highestValueScheduleFor(shifts, 3).isEmpty());
}

PROVIDED_TEST("Don't pick overlapping shifts.") {
    Vector<Shift> shifts = {
        { Day::MONDAY, 9, 17, 1000 },  // Monday,  9AM - 5PM, value is 1000
        { Day::MONDAY, 8, 18, 2000 },  // Monday, 10AM - 6PM, value is 2000
    };

    EXPECT(highestValueScheduleFor(asSet(shifts), 100).size() == 1);
    EXPECT(highestValueScheduleFor(asSet(shifts), 100).contains(shifts[1]));
}

PROVIDED_TEST("Passes the example from the handout.") {
    Vector<Shift> shifts = {
        { Day::MONDAY,     8, 12, 27 },  // Mon  8AM - 12PM, value 27 *
        { Day::MONDAY,    12, 16, 28 },  // Mon 12PM -  4PM, value 28 *
        { Day::MONDAY,    16, 20, 25 },  // Mon  4PM -  8PM, value 25 *
        { Day::MONDAY,     8, 14, 39 },  // Mon  8AM -  2PM, value 39
        { Day::MONDAY,    14, 20, 31 },  // Mon  2PM -  8PM, value 31
        { Day::TUESDAY,    8, 12,  7 },  // Tue  8AM - 12PM, value  7
        { Day::TUESDAY,   12, 16,  7 },  // Tue 12PM -  4PM, value  7
        { Day::TUESDAY,   16, 20, 11 },  // Tue  4PM -  8PM, value 11
        { Day::TUESDAY,    8, 14, 10 },  // Tue  8AM -  2PM, value 10
        { Day::TUESDAY,   14, 20,  8 },  // Tue  2PM -  8PM, value  8
        { Day::WEDNESDAY,  8, 12, 10 },  // Wed  8AM - 12PM, value 10
        { Day::WEDNESDAY, 12, 16, 11 },  // Wed 12PM -  4PM, value 11
        { Day::WEDNESDAY, 16, 20, 13 },  // Wed  4PM -  8PM, value 13
        { Day::WEDNESDAY,  8, 14, 19 },  // Wed  8AM -  2PM, value 19
        { Day::WEDNESDAY, 14, 20, 25 },  // Wed  2PM -  8PM, value 25 *
    };

    /* Get back the solution. */
    Set<Shift> computedSolution = highestValueScheduleFor(asSet(shifts), 20);

    /* Form the correct answer. It's the starred entries. */
    Set<Shift> actualSolution = {
        shifts[0], shifts[1], shifts[2], shifts[14]
    };

    EXPECT(computedSolution == actualSolution);
}

PROVIDED_TEST("Handles no shifts.") {
    EXPECT(highestValueScheduleFor({}, 137).isEmpty());
}

PROVIDED_TEST("Reports an error with negative hours.") {
    /* From the handout */
    Vector<Shift> shifts = {
        { Day::MONDAY,     8, 12, 27 },  // Mon  8AM - 12PM, value 27
        { Day::MONDAY,    12, 16, 28 },  // Mon 12PM -  4PM, value 28
        { Day::MONDAY,    16, 20, 25 },  // Mon  4PM -  8PM, value 25
        { Day::MONDAY,     8, 14, 39 },  // Mon  8AM -  2PM, value 39
        { Day::MONDAY,    14, 20, 31 },  // Mon  2PM -  8PM, value 31
        { Day::TUESDAY,    8, 12,  7 },  // Tue  8AM - 12PM, value  7
        { Day::TUESDAY,   12, 16,  7 },  // Tue 12PM -  4PM, value  7
        { Day::TUESDAY,   16, 20, 11 },  // Tue  4PM -  8PM, value 11
        { Day::TUESDAY,    8, 14, 10 },  // Tue  8AM -  2PM, value 10
        { Day::TUESDAY,   14, 20,  8 },  // Tue  2PM -  8PM, value  8
        { Day::WEDNESDAY,  8, 12, 10 },  // Wed  8AM - 12PM, value 10
        { Day::WEDNESDAY, 12, 16, 11 },  // Wed 12PM -  4PM, value 11
        { Day::WEDNESDAY, 16, 20, 13 },  // Wed  4PM -  8PM, value 13
        { Day::WEDNESDAY,  8, 14, 19 },  // Wed  8AM -  2PM, value 19
        { Day::WEDNESDAY, 14, 20, 25 },  // Wed  2PM -  8PM, value 25
    };

    /* Should be an error. */
    EXPECT_ERROR(highestValueScheduleFor(asSet(shifts), -1));

    /* Still an error even if there are no shifts. */
    EXPECT_ERROR(highestValueScheduleFor({}, -1));
}

PROVIDED_TEST("Handles zero free hours.") {
    /* From the handout */
    Vector<Shift> shifts = {
        { Day::MONDAY,     8, 12, 27 },  // Mon  8AM - 12PM, value 27
        { Day::MONDAY,    12, 16, 28 },  // Mon 12PM -  4PM, value 28
        { Day::MONDAY,    16, 20, 25 },  // Mon  4PM -  8PM, value 25
        { Day::MONDAY,     8, 14, 39 },  // Mon  8AM -  2PM, value 39
        { Day::MONDAY,    14, 20, 31 },  // Mon  2PM -  8PM, value 31
        { Day::TUESDAY,    8, 12,  7 },  // Tue  8AM - 12PM, value  7
        { Day::TUESDAY,   12, 16,  7 },  // Tue 12PM -  4PM, value  7
        { Day::TUESDAY,   16, 20, 11 },  // Tue  4PM -  8PM, value 11
        { Day::TUESDAY,    8, 14, 10 },  // Tue  8AM -  2PM, value 10
        { Day::TUESDAY,   14, 20,  8 },  // Tue  2PM -  8PM, value  8
        { Day::WEDNESDAY,  8, 12, 10 },  // Wed  8AM - 12PM, value 10
        { Day::WEDNESDAY, 12, 16, 11 },  // Wed 12PM -  4PM, value 11
        { Day::WEDNESDAY, 16, 20, 13 },  // Wed  4PM -  8PM, value 13
        { Day::WEDNESDAY,  8, 14, 19 },  // Wed  8AM -  2PM, value 19
        { Day::WEDNESDAY, 14, 20, 25 },  // Wed  2PM -  8PM, value 25
    };

    /* Shouldn't be an error if time is zero - that means we just don't pick anything. */
    EXPECT(highestValueScheduleFor(asSet(shifts), 0).isEmpty());
}

PROVIDED_TEST("Stress test: Don't generate shift combinations with overlapping shifts.") {
    /* All of these shifts overlap one another. If you try producing all combinations
     * of these shifts and only check at the end whether they're valid, you'll be
     * checking 2^100 ~= 10^30 combinations of shifts, which will take so long the
     * sun will have burnt out before you're finished.
     *
     * Instead, as you're going through your decision tree and building up your shifts,
     * make sure not to include any shifts that clearly conflict with something you
     * picked earlier.
     */
    Set<Shift> trickySet;
    for (int i = 0; i < 100; i++) {
        trickySet += Shift{ Day::MONDAY, 1, 2, i };
    }
    EXPECT_EQUAL(trickySet.size(), 100);

    auto result = highestValueScheduleFor(trickySet, 1);
    EXPECT_EQUAL(result.size(), 1);
}

PROVIDED_TEST("Stress test: Don't generate shift combinations that exceed time limits.") {
    /* Here's a collection of one shift per hour of the week. Your worker has exactly
     * one hour free. If you try all possible combinations of these shifts, ignoring time
     * constraints, you will have to check over 2^100 = 10^30 combinations, which will
     * take longer than the length of the known universe to process.
     *
     * Instead, as you're exploring the decision tree to generate shift combinations,
     * make sure not to add shifts that would exceed the time limit.
     */
    Set<Shift> trickySet;
    for (Day day: { Day::SUNDAY,
                    Day::MONDAY,
                    Day::TUESDAY,
                    Day::WEDNESDAY,
                    Day::THURSDAY,
                    Day::FRIDAY,
                    Day::SATURDAY}) {
        for (int start = 0; start < 24; start++) {
            trickySet += Shift{ day, start, start + 1, 10 };
        }
    }
    EXPECT_EQUAL(trickySet.size(), 7 * 24);

    auto result = highestValueScheduleFor(trickySet, 1);
    EXPECT_EQUAL(result.size(), 1);
}
