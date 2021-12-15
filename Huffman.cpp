/* Allen Wang
 * CS106B Winter 2021
 * Assignment 9
 * This file includes functions that use Huffman coding to compress and decompress files.
 */
#include "Huffman.h"
#include "GUI/SimpleTest.h"
#include "hashmap.h"
#include "vector.h"
#include "map.h"
#include "error.h"
#include "priorityqueue.h"
#include <string>
using namespace std;

Map<char, int> generateFrequencyMap(const string& str);
Map<char, Vector<int>> generateCodeMap(EncodingTreeNode* tree, const Vector<int>& code);

/**
 * Deallocates all nodes in a Huffman tree. We've provided this helper function
 * to you since we also use it in our test driver and figured you might want
 * to make use of it.
 */
void deleteTree(EncodingTreeNode* tree) {
    if (tree != nullptr) {
        deleteTree(tree->zero);
        deleteTree(tree->one);
        delete tree;
    }
}

// Generates a frequency map for each character in the string.
Map<char, int> generateFrequencyMap(const string& str) {
    int length = str.length();
    Map<char, int> frequencies = {};
    // Fills in frequency map of characters.
    for (int i = 0; i < length; i++) {
        if (!frequencies.containsKey(str[i])) {
            frequencies.put(str[i], 1);
        }
        else {
            int lastFreq = frequencies.get(str[i]);
            lastFreq++;
            frequencies.put(str[i], lastFreq);
        }
    }
    return frequencies;
}

/**
 * Constructs a Huffman coding tree for the given string, using the algorithm
 * described in lecture.
 *
 * If the input string does not contain at least two different characters,
 * this function should report an error.
 *
 * When assembling larger trees out of smaller ones, make sure to set the first
 * tree dequeued from the queue to be the zero subtree of the new tree and the
 * second tree as the one subtree.
 */
EncodingTreeNode* huffmanTreeFor(const string& str) {
    Map<char, int> frequencies = generateFrequencyMap(str);
    if (frequencies.size() < 2) {
        error("ERROR: FEWER THAN TWO DIFFERENT CHARACTERS");
    }
    PriorityQueue<EncodingTreeNode*> treeQueue;
    // Creates leaf nodes for each character.
    for (char character : frequencies) {
        EncodingTreeNode* node = new EncodingTreeNode{character, nullptr, nullptr};
        treeQueue.enqueue(node, frequencies.get(character));
    }
    // Combines two lowest priority trees into parent tree.
    while (treeQueue.size() > 1) {
        int leftPriority = treeQueue.peekPriority();
        EncodingTreeNode* leftChild = treeQueue.dequeue();
        int rightPriority = treeQueue.peekPriority();
        EncodingTreeNode* rightChild = treeQueue.dequeue();
        int prioritySum = leftPriority + rightPriority;
        EncodingTreeNode* parent = new EncodingTreeNode;
        parent->zero = leftChild;
        parent->one = rightChild;
        treeQueue.enqueue(parent, prioritySum);
    }
    EncodingTreeNode* result = treeQueue.dequeue();
    return result;
}

/**
 * Given a Queue<Bit> containing a compressed message and a tree that was used
 * to encode those bits, decodes the bits back to the original message.
 *
 * You can assume the input tree is not null and is not a single character;
 * those are edge cases you don't need to handle.
 *
 * You can assume the input Queue<Bit> is well-formed in that every character
 * was encoded correctly, there are no stray bits in the Queue, etc.
 */
string decodeText(Queue<Bit>& bits, EncodingTreeNode* tree) {
    string result = "";
    EncodingTreeNode* curr = tree;
    // Traverses tree and resets pointer/adds character at an end node.
    while (bits.size() > 0) {
        Bit nextBit = bits.dequeue();
        if (nextBit == 0) {
            curr = curr->zero;
            if (curr->zero == nullptr) {
                result += curr->ch;
                curr = tree;
            }
        }
        if (nextBit == 1) {
            curr = curr->one;
            if (curr->zero == nullptr) {
                result += curr->ch;
                curr = tree;
            }
        }
    }
    return result;
}

/**
 * Given a string and a Huffman encoding tree, encodes that text using the tree
 * and outputs a Queue<Bit> corresponding to the encoded representation.
 *
 * The input tree will not be null and will not consist of a single node; these
 * are edge cases you don't have to handle. The input tree will contain all
 * characters that make up the input string.
 */
Queue<Bit> encodeText(const string& str, EncodingTreeNode* tree) {
    Vector<int> code = {};
    Map<char, Vector<int>> pairs = generateCodeMap(tree, code);
    int length = str.length();
    Queue<Bit> out;
    for (int i = 0; i < length; i++) {
        Vector<int> code = pairs.get(str[i]);
        for (int i = 0; i < code.size(); i++) {
            out.enqueue(code[i]);
        }
    }
    return out;
}

// Generates a Huffman code given a tree and prefix.
Map<char, Vector<int>> generateCodeMap(EncodingTreeNode* tree, const Vector<int>& code) {
    EncodingTreeNode* curr = tree;
    if (curr->zero == nullptr && curr->one == nullptr) {
        return {{curr->ch, code}};
    }
    else {
        return (generateCodeMap(curr->zero, code + 0) + generateCodeMap(curr->one, code + 1));
    }
}

/**
 * Decodes the given Queue<Bit> and Queue<char> into a Huffman coding tree.
 *
 * You can assume that the input Queues are structured properly in that they
 * represent a legal encoding of a tree, that there aren't stray characters
 * or bits in them, etc.
 */
EncodingTreeNode* decodeTree(Queue<Bit>& bits, Queue<char>& leaves) {
    Bit currBit = bits.dequeue();
    if (currBit == 0) {
        char leaf = leaves.dequeue();
        EncodingTreeNode* leafNode = new EncodingTreeNode{leaf, nullptr, nullptr};
        return leafNode;
    }
    else {
        EncodingTreeNode* tree = new EncodingTreeNode;
        tree->zero = decodeTree(bits, leaves);
        tree->one = decodeTree(bits, leaves);
        return tree;
    }
}

/**
 * Encodes the given Huffman tree as a Queue<Bit> and Queue<char> in the manner
 * specified in the assignment handout.
 *
 * You can assume the input Queues are empty on entry to this function.
 *
 * You can assume that the Huffman tree provided to you is properly structured,
 * in that each internal node will have two children and only the characters in
 * the leaves matter, etc.
 */
void encodeTree(EncodingTreeNode* tree, Queue<Bit>& bits, Queue<char>& leaves) {
    if (tree->zero == nullptr && tree->one == nullptr) {
        leaves.enqueue(tree->ch);
        bits.enqueue(0);
        return;
    }
    else {
        bits.enqueue(1);
        encodeTree(tree->zero, bits, leaves);
        encodeTree(tree->one, bits, leaves);
    }
}

/**
 * Compresses the given text string using Huffman coding, producing as output
 * a HuffmanResult containing the encoded tree and message.
 *
 * Your implementation of this function should report an error if there are
 * fewer than two distinct characters in the input string.
 */
HuffmanResult compress(const string& text) {
    Queue<char> treeLeaves;
    Queue<Bit> treeBits;
    Map<char, int> frequencies = generateFrequencyMap(text);
    int length = frequencies.size();
    if (length < 2) {
        error("ERROR: INVALID STRING INPUT");
    }
    EncodingTreeNode* huffmanTree = huffmanTreeFor(text);
    Queue<Bit> message = encodeText(text, huffmanTree);
    encodeTree(huffmanTree, treeBits, treeLeaves);
    HuffmanResult result = {treeBits, treeLeaves, message};
    deleteTree(huffmanTree);
    return result;
}

/**
 * Decompresses the given HuffmanResult and returns the string it represents.
 *
 * Your implementation may change the file parameter however it sees fit. There
 * are no requirements about what it should look like after this function
 * returns.
 *
 * You can assume the input file is well-formed and was created by a correct
 * implementation of compress.
 */
string decompress(HuffmanResult& file) {
    Queue<Bit> treeBits = file.treeBits;
    Queue<char> treeLeaves = file.treeLeaves;
    EncodingTreeNode* tree = decodeTree(treeBits, treeLeaves);
    Queue<Bit> messageBits = file.messageBits;
    string result = decodeText(messageBits, tree);
    deleteTree(tree);
    return result;
}



/* * * * * * Test Cases Below This Point * * * * * */
/* Utility function to test if a purported Huffman tree is indeed a Huffman tree.
 * Specifically, this checks that each internal node has either zero or two
 * children. There are other ways you could produce an invalid Huffman tree - for
 * example, by having uninitialized pointers or by linking in a cycle - but we
 * don't test for that here.
 */
bool isEncodingTree(EncodingTreeNode* tree) {
    /* The empty tree is not a Huffman tree. */
    if (tree == nullptr) return false;

    /* If we have one missing child, we should have two missing children. */
    if ((tree->zero == nullptr) != (tree->one == nullptr)) return false;

    /* If we have children at all, they need to be Huffman trees. */
    return tree->zero == nullptr || (isEncodingTree(tree->zero) && isEncodingTree(tree->one));
}

/* Utility function to test if two trees are equal. This is adapted from Section
 * Handout 8 and particularized to Huffman trees.
 */
bool areEqual(EncodingTreeNode* lhs, EncodingTreeNode* rhs) {
    /* Base case: If either is a leaf, both should be. */
    bool lhsLeaf = lhs->zero == nullptr && lhs->one == nullptr;
    bool rhsLeaf = rhs->zero == nullptr && rhs->one == nullptr;
    if (lhsLeaf || rhsLeaf) {
        return lhs->ch == rhs->ch && lhsLeaf == rhsLeaf;
    }

    /* Otherwise, they're both internal nodes. Check that they match. */
    return areEqual(lhs->zero, rhs->zero) && areEqual(lhs->one, rhs->one);
}

STUDENT_TEST("huffmanTreeFor works on example in short answer questions.") {
    //     This tree:
    //            []
    //          /    \
    //        B       []
    //              /    \
    //            A        C

    EncodingTreeNode* reference = new EncodingTreeNode {
            '\0',
            new EncodingTreeNode { 'B', nullptr, nullptr },
            new EncodingTreeNode { '\0',
            new EncodingTreeNode { 'A', nullptr, nullptr },
            new EncodingTreeNode { 'C', nullptr, nullptr }
}
};

    /* Ax2, Bx4, Cx3 */
    EncodingTreeNode* tree = huffmanTreeFor("AABBBBCCC");
    EXPECT(isEncodingTree(tree));
    EXPECT(areEqual(tree, reference));

    deleteTree(reference);
    deleteTree(tree);
}

STUDENT_TEST("decodeText works on example in short answer questions.") {
    /* This tree:
     *                 *
     *                / \
     *               A   *
     *                  / \
     *                 R   *
     *                    / \
     *                   *   B
     *                  / \
     *                 C   D
     *
     */
    EncodingTreeNode* tree = new EncodingTreeNode {
            '*',
            new EncodingTreeNode { 'A', nullptr, nullptr },
            new EncodingTreeNode { '*',
            new EncodingTreeNode { 'R', nullptr, nullptr },
            new EncodingTreeNode{ '*',
            new EncodingTreeNode{ '*',
            new EncodingTreeNode { 'C', nullptr, nullptr },
            new EncodingTreeNode { 'D', nullptr, nullptr }
},
            new EncodingTreeNode { 'B', nullptr, nullptr },
}
}
};

    /* What you get if you encode ABRACADABRA with this tree. */
    Queue<Bit> bits = { 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0 };

    EXPECT_EQUAL(decodeText(bits, tree), "ABRACADABRA");

    deleteTree(tree);
}

STUDENT_TEST("encodeText works on example in short answer questions.") {
    /* This tree:
     *                 *
     *                / \
     *               A   *
     *                  / \
     *                 R   *
     *                    / \
     *                   *   B
     *                  / \
     *                 C   D
     *
     */
    EncodingTreeNode* tree = new EncodingTreeNode {
            '*',
            new EncodingTreeNode { 'A', nullptr, nullptr },
            new EncodingTreeNode { '*',
            new EncodingTreeNode { 'R', nullptr, nullptr },
            new EncodingTreeNode{ '*',
            new EncodingTreeNode{ '*',
            new EncodingTreeNode { 'C', nullptr, nullptr },
            new EncodingTreeNode { 'D', nullptr, nullptr }
},
            new EncodingTreeNode { 'B', nullptr, nullptr },
}
}
};

    /* What you get if you encode ABRACADABRA with this tree. */
    Queue<Bit> expected = { 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0 };

    EXPECT_EQUAL(encodeText("ABRACADABRA", tree), expected);

    deleteTree(tree);
}

STUDENT_TEST("decodeTree works on example in short answer questions.") {
    //                        []
    //                     /      \
    //                   []        []
    //                 /    \    /    \
    //               []      C  L      V
    //              /  \
    //            M     D
    Queue<Bit>  bits   = { 1, 1, 1, 0, 0, 0, 1, 0, 0 };
    Queue<char> leaves = { 'M', 'D', 'C', 'L', 'V' };

    EncodingTreeNode* tree = decodeTree(bits, leaves);
    EXPECT(isEncodingTree(tree));

    EncodingTreeNode* expected = new EncodingTreeNode {
            '*',
            new EncodingTreeNode { '*',
            new EncodingTreeNode{ '*',
            new EncodingTreeNode { 'M', nullptr, nullptr },
            new EncodingTreeNode { 'D', nullptr, nullptr }
},
            new EncodingTreeNode { 'C', nullptr, nullptr },
},
            new EncodingTreeNode{ '*',
            new EncodingTreeNode { 'L', nullptr, nullptr },
            new EncodingTreeNode { 'V', nullptr, nullptr }
},
};

    EXPECT(areEqual(tree, expected));

    deleteTree(tree);
    deleteTree(expected);
}

STUDENT_TEST("encodeTree works on example in short answer questions.") {
    EncodingTreeNode* tree = new EncodingTreeNode {
            '*',
            new EncodingTreeNode { '*',
            new EncodingTreeNode{ '*',
            new EncodingTreeNode { 'M', nullptr, nullptr },
            new EncodingTreeNode { 'D', nullptr, nullptr }
},
            new EncodingTreeNode { 'C', nullptr, nullptr },
},
            new EncodingTreeNode{ '*',
            new EncodingTreeNode { 'L', nullptr, nullptr },
            new EncodingTreeNode { 'V', nullptr, nullptr }
},
};

    Queue<Bit>  bits;
    Queue<char> leaves;

    encodeTree(tree, bits, leaves);

    Queue<Bit>  expectedBits   = { 1, 1, 1, 0, 0, 0, 1, 0, 0 };
    Queue<char> expectedLeaves = { 'M', 'D', 'C', 'L', 'V' };

    EXPECT_EQUAL(bits,   expectedBits);
    EXPECT_EQUAL(leaves, expectedLeaves);

    deleteTree(tree);
}

STUDENT_TEST("Can decompress a small sample file (v2).") {
    HuffmanResult file = {
        { 1, 1, 1, 0, 0, 0, 0 },
        { 'D', 'B', 'N', 'A' },
        { 1, 0, 0, 1, 1, 0, 1, 1, 0,
          1, 1, 1, 0, 0, 1, 1, 0, 1,
          0, 0, 0, 1, 0, 1, 1 },
    };

    EXPECT_EQUAL(decompress(file), "ABANANAABANDANA");
}

STUDENT_TEST("Can compress a small sample file (v2).") {
    HuffmanResult file = compress("humuhumunukunukuapuaa");
    Queue<Bit>  treeBits    = { 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0 };
    Queue<char> treeChars   = { 'u', 'h', 'p', 'n', 'a', 'm', 'k' };
    Queue<Bit>  messageBits = { 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0,
                                1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1,
                                1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0 };

    EXPECT_EQUAL(file.treeBits, treeBits);
    EXPECT_EQUAL(file.treeLeaves, treeChars);
    EXPECT_EQUAL(file.messageBits, messageBits);
}


/* * * * * Provided Tests Below This Point * * * * */
#include <limits>

/* Utility function to return a string of all possible characters. */
string pangrammaticString() {
    string result;

    char ch = numeric_limits<char>::min();
    result += ch;
    do {
        ch++;
        result += ch;
    } while (ch != numeric_limits<char>::max());

    return result;
}

/* Utility function that makes an inefficient but still valid encoding tree
 * for the given characters.
 */
EncodingTreeNode* strandTreeFor(const string& text, size_t index = 0) {
    if (index == text.size()) error("No characters provided to strandTreeFor.");

    /* We always get a leaf node. */
    EncodingTreeNode* leaf = new EncodingTreeNode {
            text[index], nullptr, nullptr
};

    /* Last character? If so, that's all. */
    if (index + 1 == text.size()) return leaf;

    /* Otherwise, build a larger tree. */
    else return new EncodingTreeNode {
        ' ', leaf, strandTreeFor(text, index + 1)
    };
}

PROVIDED_TEST("huffmanTreeFor reports errors on invalid inputs.") {
    EXPECT_ERROR(huffmanTreeFor(""));    // No characters
    EXPECT_ERROR(huffmanTreeFor("a"));   // One character
    EXPECT_ERROR(huffmanTreeFor("aaa")); // One character
}

PROVIDED_TEST("huffmanTreeFor builds tree for two characters.") {
    EncodingTreeNode* reference = new EncodingTreeNode {
            ' ', new EncodingTreeNode {'a', nullptr, nullptr}, new EncodingTreeNode {'b', nullptr, nullptr}
};

    EncodingTreeNode* tree = huffmanTreeFor("aaabbbb");
    EXPECT(isEncodingTree(tree));
    EXPECT(areEqual(tree, reference));

    deleteTree(reference);
    deleteTree(tree);
}

PROVIDED_TEST("huffmanTreeFor works on the full range of characters.") {
    /* Get a string of all possible characters, then pair them off and see what we find. */
    string allChars = pangrammaticString();
    for (size_t i = 0; i < allChars.size(); i += 2) {
        string toEncode;
        toEncode += allChars[i];
        toEncode += allChars[i + 1];
        toEncode += allChars[i + 1];

        EncodingTreeNode* reference = new EncodingTreeNode {
                ' ',
                new EncodingTreeNode {allChars[i], nullptr, nullptr},
                new EncodingTreeNode {allChars[i + 1], nullptr, nullptr}
    };

    EncodingTreeNode* tree = huffmanTreeFor(toEncode);
    EXPECT(isEncodingTree(tree));
    EXPECT(areEqual(tree, reference));

    deleteTree(reference);
    deleteTree(tree);
}
}

PROVIDED_TEST("huffmanTreeFor uses cumulative weights (v1).") {
    /* This tree:
     *                 *
     *                / \
     *               *   D
     *              / \
     *             C   *
     *                / \
     *               A   B
     */
    EncodingTreeNode* reference = new EncodingTreeNode {
            '*',
            new EncodingTreeNode { '!',
            new EncodingTreeNode { 'C', nullptr, nullptr },
            new EncodingTreeNode { '?',
            new EncodingTreeNode { 'A', nullptr, nullptr },
            new EncodingTreeNode { 'B', nullptr, nullptr }
}
},
            new EncodingTreeNode { 'D', nullptr, nullptr }
};

    /* Ax2, Bx3, Cx4, Dx10 */
    EncodingTreeNode* tree = huffmanTreeFor("AABBBCCCCDDDDDDDDDD");
    EXPECT(isEncodingTree(tree));
    EXPECT(areEqual(tree, reference));

    deleteTree(reference);
    deleteTree(tree);
}

PROVIDED_TEST("huffmanTreeFor uses cumulative weights (v2).") {
    /*
     *          *
     *       /     \
     *      *       *
     *     / \     / \
     *    D   E   F   *
     *               / \
     *              C   *
     *                 / \
     *                A   B
     */
    EncodingTreeNode* reference =new EncodingTreeNode {
            ' ',
            new EncodingTreeNode {
            ' ',
            new EncodingTreeNode{ 'D', nullptr, nullptr },
            new EncodingTreeNode{ 'E', nullptr, nullptr }
},
            new EncodingTreeNode {
            ' ',
            new EncodingTreeNode{ 'F', nullptr, nullptr },
            new EncodingTreeNode {
            ' ',
            new EncodingTreeNode{ 'C', nullptr, nullptr },
            new EncodingTreeNode{
            ' ',
            new EncodingTreeNode{ 'A', nullptr, nullptr },
            new EncodingTreeNode{ 'B', nullptr, nullptr },
}
}
}
};

    /* Ax2, Bx3, Cx4, Dx6, Ex7, Fx8 */
    EncodingTreeNode* tree = huffmanTreeFor("AABBBCCCCDDDDDDEEEEEEEFFFFFFFF");
    EXPECT(isEncodingTree(tree));

    EXPECT(areEqual(tree, reference));

    deleteTree(reference);
    deleteTree(tree);
}

PROVIDED_TEST("decodeText works on small sample.") {
    /* This tree:
     *                 *
     *                / \
     *               O   *
     *                  / \
     *                 *   N
     *                / \
     *               M   S
     */
    EncodingTreeNode* tree = new EncodingTreeNode {
            '*',
            new EncodingTreeNode { 'O', nullptr, nullptr },
            new EncodingTreeNode { '*',
            new EncodingTreeNode{ '*',
            new EncodingTreeNode { 'M', nullptr, nullptr },
            new EncodingTreeNode { 'S', nullptr, nullptr }
},
            new EncodingTreeNode{ 'N', nullptr, nullptr }
}
};

    /* What you get if you encode MONSOON with this tree. */
    Queue<Bit> bits = { 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1 };

    EXPECT_EQUAL(decodeText(bits, tree), "MONSOON");

    deleteTree(tree);
}

PROVIDED_TEST("Can decode all char values.") {
    /* All possible characters. */
    string allChars = pangrammaticString();

    /* Try decoding all pairs of adjacent characters. */
    for (size_t i = 0; i < allChars.size(); i += 2) {
        string expected;
        expected += allChars[i];
        expected += allChars[i + 1];
        expected += allChars[i + 1];

        EncodingTreeNode* tree = new EncodingTreeNode {
                ' ',
                new EncodingTreeNode {allChars[i], nullptr, nullptr},
                new EncodingTreeNode {allChars[i + 1], nullptr, nullptr}
    };

    /* Decode the bitstream 011, which should map back to the expected
         * string.
         */
    Queue<Bit> bits = { 0, 1, 1 };
    EXPECT_EQUAL(decodeText(bits, tree), expected);

    deleteTree(tree);
}
}

PROVIDED_TEST("encodeText works on small sample.") {
    /* This tree:
     *                 *
     *                / \
     *               O   *
     *                  / \
     *                 *   N
     *                / \
     *               M   S
     */
    EncodingTreeNode* tree = new EncodingTreeNode {
            '*',
            new EncodingTreeNode { 'O', nullptr, nullptr },
            new EncodingTreeNode { '*',
            new EncodingTreeNode{ '*',
            new EncodingTreeNode { 'M', nullptr, nullptr },
            new EncodingTreeNode { 'S', nullptr, nullptr }
},
            new EncodingTreeNode{ 'N', nullptr, nullptr }
}
};

    /* What you get if you encode MONSOON with this tree. */
    Queue<Bit> expected = { 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1 };

    EXPECT_EQUAL(encodeText("MONSOON", tree), expected);

    deleteTree(tree);
}

PROVIDED_TEST("Can encode all char values.") {
    /* All possible characters. */
    string allChars = pangrammaticString();

    /* Try encoding all pairs of adjacent characters. */
    for (size_t i = 0; i < allChars.size(); i += 2) {
        string toEncode;
        toEncode += allChars[i];
        toEncode += allChars[i + 1];
        toEncode += allChars[i + 1];

        EncodingTreeNode* tree = new EncodingTreeNode {
                ' ',
                new EncodingTreeNode {allChars[i], nullptr, nullptr},
                new EncodingTreeNode {allChars[i + 1], nullptr, nullptr}
    };

    /* See what bits we get back. We should get 011, since the first
         * character has code 0 and the second has code 1.
         */
    Queue<Bit> bits = encodeText(toEncode, tree);
    Queue<Bit> expected = { 0, 1, 1 };

    EXPECT_EQUAL(bits, expected);

    deleteTree(tree);
}
}

PROVIDED_TEST("decodeText undoes encodeText on range of sample strings.") {
    Vector<string> testCases = {
        "THAT THAT IS IS THAT THAT IS NOT IS NOT IS THAT IT IT IS",
        "AABAAABBABAAABAAAA",
        ":-) :-D XD <(^_^)>",
        pangrammaticString(),
    };

    for (string test: testCases) {
        /* Use a silly encoding scheme, but one that works regardless of what
         * characters are provided.
         */
        EncodingTreeNode* tree = strandTreeFor(test);
        EXPECT(isEncodingTree(tree));

        Queue<Bit> bits = encodeText(test, tree);
        string result = decodeText(bits, tree);

        EXPECT_EQUAL(test.size(), result.size());
        EXPECT_EQUAL(test, result);

        deleteTree(tree);
    }
}

PROVIDED_TEST("Can decode an example tree.") {
    /* This encodes this tree:
     *
     *                 *
     *                / \
     *               *   C
     *              / \
     *             A   B
     */
    Queue<Bit>  bits   = { 1, 1, 0, 0, 0 };
    Queue<char> leaves = { 'A', 'B', 'C' };

    EncodingTreeNode* tree = decodeTree(bits, leaves);
    EXPECT(isEncodingTree(tree));

    /* Confirm this is the right tree. */
    EncodingTreeNode* expected = new EncodingTreeNode {
            '*',
            new EncodingTreeNode {
            '*',
            new EncodingTreeNode { 'A', nullptr, nullptr },
            new EncodingTreeNode { 'B', nullptr, nullptr },
},
            new EncodingTreeNode { 'C', nullptr, nullptr }
};

    EXPECT(areEqual(tree, expected));

    deleteTree(tree);
    deleteTree(expected);
}

PROVIDED_TEST("Can decode trees using all possible char values.") {
    /* All possible characters. */
    string allChars = pangrammaticString();

    /* Try encoding all pairs of adjacent characters. */
    for (size_t i = 0; i < allChars.size(); i += 2) {
        EncodingTreeNode* expected = new EncodingTreeNode {
                ' ',
                new EncodingTreeNode {allChars[i], nullptr, nullptr},
                new EncodingTreeNode {allChars[i + 1], nullptr, nullptr}
    };
    Queue<Bit>  treeBits   = { 1, 0, 0 };
    Queue<char> treeLeaves = { allChars[i], allChars[i + 1] };

    EncodingTreeNode* tree = decodeTree(treeBits, treeLeaves);
    EXPECT(isEncodingTree(tree));
    EXPECT(areEqual(tree, expected));

    deleteTree(tree);
    deleteTree(expected);
}
}

PROVIDED_TEST("Can encode an example tree.") {
    /* Build an encoding tree for "ABBCCCC." It should look like this:
     *
     *                 *
     *                / \
     *               *   C
     *              / \
     *             A   B
     *
     * This will compress down to
     *
     *        11000
     *        ABC
     */
    EncodingTreeNode* tree = huffmanTreeFor("ABBCCCC");

    Queue<Bit>  bits;
    Queue<char> leaves;

    encodeTree(tree, bits, leaves);

    Queue<Bit>  expectedBits   = { 1, 1, 0, 0, 0 };
    Queue<char> expectedLeaves = { 'A', 'B', 'C' };

    EXPECT_EQUAL(bits,   expectedBits);
    EXPECT_EQUAL(leaves, expectedLeaves);

    deleteTree(tree);
}

PROVIDED_TEST("Can encode trees using all possible char values.") {
    /* All possible characters. */
    string allChars = pangrammaticString();

    /* Try encoding all pairs of adjacent characters. */
    for (size_t i = 0; i < allChars.size(); i += 2) {
        EncodingTreeNode* tree = new EncodingTreeNode {
                ' ',
                new EncodingTreeNode {allChars[i], nullptr, nullptr},
                new EncodingTreeNode {allChars[i + 1], nullptr, nullptr}
    };

    /* See what we get back. It should be the bitstring 100 (root with
         * two children) and the two leaves, in order.
         */
    Queue<Bit>  treeBits;
    Queue<char> treeLeaves;

    Queue<Bit>  expectedBits = { 1, 0, 0 };
    Queue<char> expectedLeaves = { allChars[i], allChars[i + 1] };

    encodeTree(tree, treeBits, treeLeaves);
    EXPECT_EQUAL(treeBits, expectedBits);
    EXPECT_EQUAL(treeLeaves, expectedLeaves);

    deleteTree(tree);
}
}

PROVIDED_TEST("decodeTree undoes encodeTree on sample strings.") {
    /* Make a Huffman tree for the string of all characters. */
    EncodingTreeNode* sourceTree = huffmanTreeFor(pangrammaticString());
    EXPECT(isEncodingTree(sourceTree));

    /* Encode, then decode it. */
    Queue<Bit>  bits;
    Queue<char> leaves;
    encodeTree(sourceTree, bits, leaves);

    EncodingTreeNode* resultTree = decodeTree(bits, leaves);
    EXPECT(isEncodingTree(resultTree));
    EXPECT(areEqual(sourceTree, resultTree));

    deleteTree(sourceTree);
    deleteTree(resultTree);
}

PROVIDED_TEST("Can decompress a small sample file.") {
    HuffmanResult file = {
        { 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0 },
        { 'u', 'k', 'p', 'n', 'a', 'm', 'h' },
        { 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1,
          0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0,
          0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0 }
    };

    EXPECT_EQUAL(decompress(file), "humuhumunukunukuapuaa");
}

PROVIDED_TEST("Compress reports errors on bad inputs.") {
    EXPECT_ERROR(compress(""));
    EXPECT_ERROR(compress("A"));
    EXPECT_ERROR(compress("AAAA"));
}

PROVIDED_TEST("Can compress a small sample file.") {
    HuffmanResult file = compress("ABANANAABANDANA");
    Queue<Bit>  treeBits    = { 1, 1, 1, 0, 0, 0, 0 };
    Queue<char> treeChars   = { 'D', 'B', 'N', 'A' };
    Queue<Bit>  messageBits = { 1, 0, 0, 1, 1, 0, 1, 1, 0,
                                1, 1, 1, 0, 0, 1, 1, 0, 1,
                                0, 0, 0, 1, 0, 1, 1 };

    EXPECT_EQUAL(file.treeBits, treeBits);
    EXPECT_EQUAL(file.treeLeaves, treeChars);
    EXPECT_EQUAL(file.messageBits, messageBits);
}

PROVIDED_TEST("Compress undoes decompress on a range of strings.") {
    Vector<string> testCases = {
        "THAT THAT IS IS THAT THAT IS NOT IS NOT IS THAT IT IT IS",
        "AABAAABBABAAABAAAA",
        ":-) :-D XD <(^_^)>",
        pangrammaticString(),
    };

    for (string test: testCases) {
        HuffmanResult file = compress(test);
        string result = decompress(file);

        EXPECT_EQUAL(result.size(), test.size());
        EXPECT_EQUAL(test, result);
    }
}
