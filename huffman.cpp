#include "bits.h"
#include "treenode.h"
#include "huffman.h"
#include "map.h"
#include "vector.h"
#include "priorityqueue.h"
#include "strlib.h"
#include "testing/SimpleTest.h"
using namespace std;

/**
 * Given a Queue<Bit> containing the compressed message bits and the encoding tree
 * used to encode those bits, decode the bits back to the original message text.
 *
 * You can assume that tree is a well-formed non-empty encoding tree and
 * messageBits queue contains a valid sequence of encoded bits.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
string decodeText(EncodingTreeNode* tree, Queue<Bit>& messageBits) {
    /* TODO: Implement this function. */
    return "";
}

/**
 * Reconstruct encoding tree from flattened form Queue<Bit> and Queue<char>.
 *
 * You can assume that the queues are well-formed and represent
 * a valid encoding tree.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
EncodingTreeNode* unflattenTree(Queue<Bit>& treeShape, Queue<char>& treeLeaves) {
    /* TODO: Implement this function. */
    return nullptr;
}

/**
 * Decompress the given EncodedData and return the original text.
 *
 * You can assume the input data is well-formed and was created by a correct
 * implementation of compress.
 *
 * Your implementation may change the data parameter however you like. There
 * are no requirements about what it should look like after this function
 * returns.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
string decompress(EncodedData& data) {
    /* TODO: Implement this function. */
    return "";
}

/**
 * Constructs an optimal Huffman coding tree for the given text, using
 * the algorithm described in lecture.
 *
 * Reports an error if the input text does not contain at least
 * two distinct characters.
 *
 * When assembling larger trees out of smaller ones, make sure to set the first
 * tree dequeued from the queue to be the zero subtree of the new tree and the
 * second tree as the one subtree.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
EncodingTreeNode* buildHuffmanTree(string text) {
    /* TODO: Implement this function. */
    return nullptr;
}

/**
 * Given a string and an encoding tree, encode the text using the tree
 * and return a Queue<Bit> of the encoded bit sequence.
 *
 * You can assume tree is a valid non-empty encoding tree and contains an
 * encoding for every character in the text.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
Queue<Bit> encodeText(EncodingTreeNode* tree, string text) {
    /* TODO: Implement this function. */
    return {};
}

/**
 * Flatten the given tree into a Queue<Bit> and Queue<char> in the manner
 * specified in the assignment writeup.
 *
 * You can assume the input queues are empty on entry to this function.
 *
 * You can assume tree is a valid well-formed encoding tree.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
void flattenTree(EncodingTreeNode* tree, Queue<Bit>& treeShape, Queue<char>& treeLeaves) {
    /* TODO: Implement this function. */
}

/**
 * Compress the input text using Huffman coding, producing as output
 * an EncodedData containing the encoded message and flattened
 * encoding tree used.
 *
 * Reports an error if the message text does not contain at least
 * two distinct characters.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
EncodedData compress(string messageText) {
    /* TODO: Implement this function. */
    return {};
}

/* * * * * * Testing Helper Functions Below This Point * * * * * */

EncodingTreeNode* createExampleTree() {
    /* Example encoding tree used in multiple test cases:
     *                *
     *              /   \
     *             T     *
     *                  / \
     *                 *   E
     *                / \
     *               R   S
     */
    EncodingTreeNode* charT = new EncodingTreeNode('T');
    EncodingTreeNode* charR = new EncodingTreeNode('R');
    EncodingTreeNode* charS = new EncodingTreeNode('S');
    EncodingTreeNode* charE = new EncodingTreeNode('E');
    EncodingTreeNode* parRS = new EncodingTreeNode(charR, charS);
    EncodingTreeNode* parE = new EncodingTreeNode(parRS, charE);
    EncodingTreeNode* par = new EncodingTreeNode(charT, parE);
    return par;
}

void deallocateTree(EncodingTreeNode* t) {
      if (t->isLeaf() == false) {
        if (t->zero != nullptr) deallocateTree(t->zero);
        if (t->one != nullptr) deallocateTree(t->one);
      }
      delete t;
}

bool areEqual(EncodingTreeNode* a, EncodingTreeNode* b) {
    if (a->isLeaf() == true && b->isLeaf() != true)
        return false;
    if (a->isLeaf() != true && b->isLeaf() == true)
        return false;
    else if (a->isLeaf() == true && b->isLeaf() == true) {
        if (a->getChar() != b->getChar()) {
            return false;
        }
    }
    else {
        if (a->zero != nullptr && b->zero != nullptr)
            areEqual(a->zero, b->zero);
        if (a->one != nullptr && b->one != nullptr)
            areEqual(a->one, b->one);
        if (a->zero == nullptr && b->zero != nullptr)
            return false;
        if (a->zero != nullptr && b->zero == nullptr)
            return false;
        if (a->one == nullptr && b->one != nullptr)
            return false;
        if (a->one != nullptr && b->one == nullptr)
            return false;
    }
    return true;
}

/* * * * * * Test Cases Below This Point * * * * * */

STUDENT_TEST("areEqual check") {
    EncodingTreeNode* tree0 = createExampleTree();
    EncodingTreeNode* tree1 = createExampleTree();
    // Compare both equal trees
    EXPECT(areEqual(tree0, tree1));
    EncodingTreeNode* charT = new EncodingTreeNode('T');
    EncodingTreeNode* single0 = new EncodingTreeNode(charT, 0);
    EncodingTreeNode* empty = new EncodingTreeNode(0, 0);
    // Compare singleton tree to empty tree
    EXPECT(!areEqual(single0, empty));
    EncodingTreeNode* charT1 = new EncodingTreeNode('T');
    EncodingTreeNode* single1 = new EncodingTreeNode(charT1, 0);
    // Compare singleton trees
    EXPECT(areEqual(single1, single0));
    // Compare larger tree to singleton tree
    EXPECT(!areEqual(tree0, single0));
    EncodingTreeNode* charE = new EncodingTreeNode('E');
    EncodingTreeNode* charR = new EncodingTreeNode('R');
    EncodingTreeNode* charS = new EncodingTreeNode('S');
    EncodingTreeNode* parRS = new EncodingTreeNode(charR, charS);
    EncodingTreeNode* parE = new EncodingTreeNode(parRS, charE);
    EncodingTreeNode* subTree = new EncodingTreeNode(0, parE);
    // Compare large tree to entire right branch
    EXPECT(!areEqual(tree0, subTree));
    deallocateTree(tree0);
    deallocateTree(tree1);
    deallocateTree(single0);
    deallocateTree(single1);
    deallocateTree(empty);
    deallocateTree(subTree);
}

STUDENT_TEST("decodeText, small example encoding tree") {
    EncodingTreeNode* tree = createExampleTree(); // see diagram above
    EXPECT(tree != nullptr);

    deallocateTree(tree);
}

/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("decodeText, small example encoding tree") {
    EncodingTreeNode* tree = createExampleTree(); // see diagram above
    EXPECT(tree != nullptr);

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(decodeText(tree, messageBits), "E");

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(decodeText(tree, messageBits), "SET");

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1}; // STREETS
    EXPECT_EQUAL(decodeText(tree, messageBits), "STREETS");

    deallocateTree(tree);
}

PROVIDED_TEST("unflattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  treeShape  = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeLeaves = { 'T', 'R', 'S', 'E' };
    EncodingTreeNode* tree = unflattenTree(treeShape, treeLeaves);

    EXPECT(areEqual(tree, reference));

    deallocateTree(tree);
    deallocateTree(reference);
}

PROVIDED_TEST("decompress, small example input") {
    EncodedData data = {
        { 1, 0, 1, 1, 0, 0, 0 }, // treeShape
        { 'T', 'R', 'S', 'E' },  // treeLeaves
        { 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1 } // messageBits
    };

    EXPECT_EQUAL(decompress(data), "TRESS");
}

PROVIDED_TEST("buildHuffmanTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    EncodingTreeNode* tree = buildHuffmanTree("STREETTEST");
    EXPECT(areEqual(tree, reference));

    deallocateTree(reference);
    deallocateTree(tree);
}

PROVIDED_TEST("encodeText, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(encodeText(reference, "E"), messageBits);

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(encodeText(reference, "SET"), messageBits);

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1 }; // STREETS
    EXPECT_EQUAL(encodeText(reference, "STREETS"), messageBits);

    deallocateTree(reference);
}

PROVIDED_TEST("flattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  expectedShape  = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> expectedLeaves = { 'T', 'R', 'S', 'E' };

    Queue<Bit>  treeShape;
    Queue<char> treeLeaves;
    flattenTree(reference, treeShape, treeLeaves);

    EXPECT_EQUAL(treeShape,  expectedShape);
    EXPECT_EQUAL(treeLeaves, expectedLeaves);

    deallocateTree(reference);
}

PROVIDED_TEST("compress, small example input") {
    EncodedData data = compress("STREETTEST");
    Queue<Bit>  treeShape   = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeChars   = { 'T', 'R', 'S', 'E' };
    Queue<Bit>  messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0 };

    EXPECT_EQUAL(data.treeShape, treeShape);
    EXPECT_EQUAL(data.treeLeaves, treeChars);
    EXPECT_EQUAL(data.messageBits, messageBits);
}

PROVIDED_TEST("Test end-to-end compress -> decompress") {
    Vector<string> inputs = {
        "HAPPY HIP HOP",
        "Nana Nana Nana Nana Nana Nana Nana Nana Batman"
        "Research is formalized curiosity. It is poking and prying with a purpose. – Zora Neale Hurston",
    };

    for (string input: inputs) {
        EncodedData data = compress(input);
        string output = decompress(data);

        EXPECT_EQUAL(input, output);
    }
}
