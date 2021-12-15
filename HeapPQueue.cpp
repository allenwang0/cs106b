/* Allen Wang
 * CS106B Winter 2021
 * Assignment 6 Part 2
 * This file contains functions that implement a class that holds a binary heap.
 */

#include "HeapPQueue.h"
#include "vector.h"
using namespace std;


// Initializes the heap.
const int kInitialSize = 2;
HeapPQueue::HeapPQueue() {
    currSize = 0;
    maxSize = kInitialSize;
    heap         = new DataPoint[maxSize];
}

// Cleans up heap and deallocates memory.
HeapPQueue::~HeapPQueue() {
    delete[] heap;
}


// Switches the elements at the two indices.
void HeapPQueue::swap(int firstIndex, int secondIndex) {
    DataPoint currPoint = heap[firstIndex];
    heap[firstIndex] = heap[secondIndex];
    heap[secondIndex] = currPoint;
}

// Returns the parent of a child node.
int HeapPQueue::parent(int& child) {
    int parentIndex = ((child - 1) / 2);
    if (child != 0) {
        return parentIndex;
    }
    return -1;
}

// Allocates more memory and shifts data to a new larger heap.
void HeapPQueue::grow() {
    maxSize *= 2;
    DataPoint* newHeap = new DataPoint[maxSize];
    for (int i = 0; i < maxSize / 2; i++) {
        newHeap[i] = heap[i];
    }
    delete[] heap;
    heap = newHeap;
}

// Returns the left child of a parent node.
int HeapPQueue::leftChild(int& parent) {
    int leftIndex = (parent * 2 + 1);
    if (leftIndex < currSize) {
        return leftIndex;
    }
    return -1;
}

// Returns the right child of a parent node.
int HeapPQueue::rightChild(int& parent) {
    int rightIndex = (parent * 2 + 2);
    if (rightIndex < currSize) {
        return rightIndex;
    }
    return -1;
}

// Adds DataPoint in its correct position.
void HeapPQueue::enqueue(const DataPoint& data) {
    if (currSize == maxSize) {
        grow();
    }
    heap[currSize] = data;
    currSize++;
    int prevIndex = currSize - 1;
    int parentIndex = parent(prevIndex);

    // If the current element is not in the right place, swap the element with its parent.
    while (heap[prevIndex].weight < heap[parentIndex].weight && parentIndex >= 0) {
        swap(prevIndex, parentIndex);

        // Updates the two indices;
        prevIndex = parentIndex;
        parentIndex = parent(prevIndex);
    }
}

// Returns heap size.
int HeapPQueue::size() const {
    return currSize;
}

// Returns but does not remove smallest value in heap.
DataPoint HeapPQueue::peek() const {
    if (isEmpty()) {
        error("EMPTY ARRAY");
    }
    return heap[0];
}

// Shifts largest DataPoint down into its correct position.
void HeapPQueue::bubbleDown(int& index) {
    int leftIndex = leftChild(index);
    int rightIndex = rightChild(index);
    // Determines larger child.
    if (leftIndex >= 0 && rightIndex >= 0 && heap[leftIndex].weight > heap[rightIndex].weight) {
        leftIndex = rightIndex;
    }
    // Swap until smaller than parent.
    if (leftIndex > 0 && heap[index].weight > heap[leftIndex].weight) {
        swap(leftIndex, index);
        bubbleDown(leftIndex);
    }
}

// Returns and removes smallest value in heap.
DataPoint HeapPQueue::dequeue() {
    if (isEmpty()) {
        error("EMPTY ARRAY");
    }

    if (currSize == 1) {
        currSize--;
        return heap[0];
    }

    DataPoint top = peek();
    // Swap rightmost node with top then remove top.
    swap(0, size() - 1);
    currSize--;
    // Shift rightmost to correct position.
    int firstElem = 0;
    bubbleDown(firstElem);
    return top;
}

// Returns whether heap is empty.
bool HeapPQueue::isEmpty() const {
    return (currSize == 0);
}

/* This function is purely for you to use during testing. You can have it do whatever
 * you'd like, including nothing. We won't call this function during grading, so feel
 * free to fill it with whatever is most useful to you!
 *
 * This function prints out the contents of the heap.
 */
void HeapPQueue::printDebugInfo() {
    for (int i = 0; i < currSize; i++) {
        cout << heap[i] << endl;
    }
}


/* * * * * * Test Cases Below This Point * * * * * */

STUDENT_TEST("Stress test: cycle 1,000,000 elems") {
    HeapPQueue pq;
    int n = 1000000;
    for (int i = 0; i < n; i++) {
        pq.enqueue({ "", randomInteger(0, 100000) });
    }
    EXPECT_EQUAL(pq.size(), n);

    for (int i = 0; i < n; i++) {
        pq.dequeue();
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);

    for (int i = 0; i < n; i++) {
        pq.enqueue({ "", randomInteger(0, 100000) });
    }
    EXPECT_EQUAL(pq.size(), n);
}














/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("Newly-created heap is empty.") {
    HeapPQueue pq;

    EXPECT(pq.isEmpty());
    EXPECT(pq.size() == 0);
}

PROVIDED_TEST("Enqueue / dequeue single element") {
    HeapPQueue pq;
    DataPoint point = { "enqueue me!", 4 };
    pq.enqueue(point);
    EXPECT_EQUAL(pq.size(), 1);
    EXPECT_EQUAL(pq.isEmpty(), false);

    EXPECT_EQUAL(pq.dequeue(), point);
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);

    pq.enqueue(point);
    EXPECT_EQUAL(pq.size(), 1);
    EXPECT_EQUAL(pq.isEmpty(), false);

    EXPECT_EQUAL(pq.dequeue(), point);
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Dequeue / peek on empty heap throws error") {
    HeapPQueue pq;

    EXPECT(pq.isEmpty());
    EXPECT_ERROR(pq.dequeue());
    EXPECT_ERROR(pq.peek());
}

PROVIDED_TEST("Enqueue elements in sorted order.") {
    HeapPQueue pq;
    for (int i = 0; i < 10; i++) {
        pq.enqueue({ "elem" + to_string(i), i });
    }

    EXPECT_EQUAL(pq.size(), 10);
    for (int i = 0; i < 10; i++) {
        EXPECT_EQUAL(pq.peek(), { "elem" + to_string(i), i });
        EXPECT_EQUAL(pq.dequeue(), { "elem" + to_string(i), i });
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Enqueue many elements in sorted order.") {
    HeapPQueue pq;
    for (int i = 0; i < 10000; i++) {
        pq.enqueue({ "elem" + to_string(i), i });
    }

    EXPECT_EQUAL(pq.size(), 10000);
    for (int i = 0; i < 10000; i++) {
        EXPECT_EQUAL(pq.peek(), { "elem" + to_string(i), i });
        EXPECT_EQUAL(pq.dequeue(), { "elem" + to_string(i), i });
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Enqueue elements in reverse-sorted order.") {
    HeapPQueue pq;
    for (int i = 10; i >= 0; i--) {
        pq.enqueue({ "elem" + to_string(i), i });
    }

    EXPECT_EQUAL(pq.size(), 11);
    for (int i = 0; i <= 10; i++) {
        EXPECT_EQUAL(pq.peek(), { "elem" + to_string(i), i });
        EXPECT_EQUAL(pq.dequeue(), { "elem" + to_string(i), i });
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Enqueue many elements in reverse-sorted order.") {
    HeapPQueue pq;
    for (int i = 10000; i >= 0; i--) {
        pq.enqueue({ "elem" + to_string(i), i });
    }

    EXPECT_EQUAL(pq.size(), 10001);
    for (int i = 0; i <= 10000; i++) {
        auto removed = pq.dequeue();
        DataPoint expected = {
            "elem" + to_string(i), i
        };
        EXPECT_EQUAL(removed, expected);
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Insert ascending and descending sequences.") {
    HeapPQueue pq;
    for (int i = 0; i < 20; i++) {
        pq.enqueue({ "a" + to_string(i), 2 * i });
    }
    for (int i = 19; i >= 0; i--) {
        pq.enqueue({ "b" + to_string(i), 2 * i + 1 });
    }

    EXPECT_EQUAL(pq.size(), 40);
    for (int i = 0; i < 40; i++) {
        auto removed = pq.dequeue();
        EXPECT_EQUAL(removed.weight, i);
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Insert large ascending and descending sequences.") {
    HeapPQueue pq;
    for (int i = 0; i < 20000; i++) {
        pq.enqueue({ "a" + to_string(i), 2 * i });
    }
    for (int i = 19999; i >= 0; i--) {
        pq.enqueue({ "b" + to_string(i), 2 * i + 1 });
    }

    EXPECT_EQUAL(pq.size(), 40000);
    for (int i = 0; i < 40000; i++) {
        auto removed = pq.dequeue();
        EXPECT_EQUAL(removed.weight, i);
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Insert random permutation.") {
    Vector<DataPoint> sequence = {
        { "A", 0 },
        { "D", 3 },
        { "F", 5 },
        { "G", 6 },
        { "C", 2 },
        { "H", 7 },
        { "I", 8 },
        { "B", 1 },
        { "E", 4 },
        { "J", 9 },
    };

    HeapPQueue pq;
    for (DataPoint elem: sequence) {
        pq.enqueue(elem);
    }

    EXPECT_EQUAL(pq.size(), sequence.size());

    for (int i = 0; i < 10; i++) {
        auto removed = pq.dequeue();
        DataPoint expected = {
            string(1, 'A' + i), i
        };
        EXPECT_EQUAL(removed, expected);
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Insert duplicate elements.") {
    HeapPQueue pq;
    for (int i = 0; i < 20; i++) {
        pq.enqueue({ "a" + to_string(i), i });
    }
    for (int i = 19; i >= 0; i--) {
        pq.enqueue({ "b" + to_string(i), i });
    }

    EXPECT_EQUAL(pq.size(), 40);
    for (int i = 0; i < 20; i++) {
        auto one = pq.dequeue();
        auto two = pq.dequeue();

        EXPECT_EQUAL(one.weight, i);
        EXPECT_EQUAL(two.weight, i);
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Insert many duplicate elements.") {
    HeapPQueue pq;
    for (int i = 0; i < 20000; i++) {
        pq.enqueue({ "a" + to_string(i), i });
    }
    for (int i = 19999; i >= 0; i--) {
        pq.enqueue({ "b" + to_string(i), i });
    }

    EXPECT_EQUAL(pq.size(), 40000);
    for (int i = 0; i < 20000; i++) {
        auto one = pq.dequeue();
        auto two = pq.dequeue();

        EXPECT_EQUAL(one.weight, i);
        EXPECT_EQUAL(two.weight, i);
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Handles data points with empty string name.") {
    HeapPQueue pq;
    for (int i = 0; i < 10; i++) {
        pq.enqueue({ "" , i });
    }
    EXPECT_EQUAL(pq.size(), 10);
    for (int i = 0; i < 10; i++) {
        EXPECT_EQUAL(pq.dequeue(), { "", i });
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT(pq.isEmpty());
}

PROVIDED_TEST("Handles many data points with empty string name.") {
    HeapPQueue pq;
    for (int i = 0; i < 10000; i++) {
        pq.enqueue({ "" , i });
    }
    EXPECT_EQUAL(pq.size(), 10000);
    for (int i = 0; i < 10000; i++) {
        EXPECT_EQUAL(pq.dequeue(), { "", i });
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT(pq.isEmpty());
}

PROVIDED_TEST("Handles data points with negative weights.") {
    HeapPQueue pq;
    for (int i = -10; i < 10; i++) {
        pq.enqueue({ "" , i });
    }
    EXPECT_EQUAL(pq.size(), 20);
    for (int i = -10; i < 10; i++) {
        EXPECT_EQUAL(pq.dequeue().weight, i);
    }
}

PROVIDED_TEST("Handles many data points with negative weights.") {
    HeapPQueue pq;
    for (int i = -10000; i < 10000; i++) {
        pq.enqueue({ "" , i });
    }
    EXPECT_EQUAL(pq.size(), 20000);
    for (int i = -10000; i < 10000; i++) {
        EXPECT_EQUAL(pq.dequeue().weight, i);
    }
}

PROVIDED_TEST("Interleave enqueues and dequeues.") {
    HeapPQueue pq;
    int n = 100;
    for (int i = n / 2; i < n; i++) {
        pq.enqueue({"", i});
    }
    EXPECT_EQUAL(pq.size(), n / 2);
    for (int i = n / 2; i < n; i++) {
        EXPECT_EQUAL(pq.dequeue().weight, i);
    }
    EXPECT_EQUAL(pq.size(), 0);

    for (int i = 0; i < n / 2; i++) {
        pq.enqueue({"", i});
    }
    EXPECT_EQUAL(pq.size(), n / 2);
    for (int i = 0; i < n / 2; i++) {
        EXPECT_EQUAL(pq.dequeue().weight, i);
    }
    EXPECT_EQUAL(pq.size(), 0);
}

PROVIDED_TEST("Interleave many enqueues and dequeues.") {
    HeapPQueue pq;
    int n = 10000;
    for (int i = n / 2; i < n; i++) {
        pq.enqueue({"", i});
    }
    EXPECT_EQUAL(pq.size(), n / 2);
    for (int i = n / 2; i < n; i++) {
        EXPECT_EQUAL(pq.dequeue().weight, i);
    }
    EXPECT_EQUAL(pq.size(), 0);

    for (int i = 0; i < n / 2; i++) {
        pq.enqueue({"", i});
    }
    EXPECT_EQUAL(pq.size(), n / 2);
    for (int i = 0; i < n / 2; i++) {
        EXPECT_EQUAL(pq.dequeue().weight, i);
    }
    EXPECT_EQUAL(pq.size(), 0);
}

PROVIDED_TEST("Stress test: cycle 250,000 elems (should take at most a few seconds)") {
    HeapPQueue pq;
    int n = 250000;
    for (int i = 0; i < n; i++) {
        pq.enqueue({ "", randomInteger(0, 100000) });
    }
    EXPECT_EQUAL(pq.size(), n);

    for (int i = 0; i < n; i++) {
        pq.dequeue();
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);

    for (int i = 0; i < n; i++) {
        pq.enqueue({ "", randomInteger(0, 100000) });
    }
    EXPECT_EQUAL(pq.size(), n);
}
