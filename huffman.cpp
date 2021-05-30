/* This program creates a huffman tree which provides lossless compression of data or characters.
 */
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
 * This function iteratively run throught the messageBits and creates the decoded text using the leafs of the
 * given tree.
 */
string decodeText(EncodingTreeNode* tree, Queue<Bit>& messageBits) {
    string text = "";
    EncodingTreeNode* temp = tree;
    // Accomodates for the changing messageBits size
    int messageSize = messageBits.size();
    for (int i = 0; i < messageSize; i++) {
        Bit bitValue = messageBits.dequeue();
        if (bitValue == 1) {
            temp = temp->one;
            if (temp->isLeaf()) {
                text += temp->getChar();
                // Makes temp equal to tree to return to the root of the tree
                temp = tree;
            }
        } else {
            temp = temp->zero;
            if (temp->isLeaf()) {
                text += temp->getChar();
                temp = tree;
            }
        }
    }
    return text;
}

/**
 * Reconstruct encoding tree from flattened form Queue<Bit> and Queue<char>.
 *
 * You can assume that the queues are well-formed and represent
 * a valid encoding tree.
 *
 * Recursively runs through the tree until all the Bits in treeShape have been visited. If the next Bit is a 1, then we recursively
 * run through the tree until we arrive at a 0, or a leaf, and add, or return, it to the tree.
 */
EncodingTreeNode* unflattenTree(Queue<Bit>& treeShape, Queue<char>& treeLeaves) {
    // Assume tree is initially empty.
    EncodingTreeNode* unFlatTree = nullptr;
    if (treeShape.size() > 0) {
        if (treeShape.dequeue() == 1) {
            // Create a parent
            unFlatTree = new EncodingTreeNode(unflattenTree(treeShape, treeLeaves), unflattenTree(treeShape, treeLeaves));
        } else {
            // Return child
            return new EncodingTreeNode(treeLeaves.dequeue());
        }
    }
    return unFlatTree;
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
 * Usign the previously implemented functions we can find the message from the data by unflattening the tree of
 * data and decoding the message of that unflatted tree. The new tree created must be allocated within the function as it is
 * initialized inside the function, not inputed.
 */
string decompress(EncodedData& data) {
    EncodingTreeNode* unFlatTree = unflattenTree(data.treeShape, data.treeLeaves);
    string message = decodeText(unFlatTree, data.messageBits);
    deallocateTree(unFlatTree);
    return message;
}

/* This recursive helper function counts the frequency of the this specific parent given the children to have the
 * priority queue organize this frequency.
 */
int textFrequency(EncodingTreeNode* parent, Map<char, int> letterMap) {
    if (parent->isLeaf()) {
        return letterMap[parent->ch];
    } else {
        return textFrequency(parent->one, letterMap) + textFrequency(parent->zero, letterMap);
    }
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
 * This function maps the text, with keys as its characters and values as their respective frequency.
 * This map is then added to a priority queue, organizing the letter by their frequency. The
 * tree is then built by removing two nodes ,from this priority queue, at a time, as there are can only
 * be two children or no children,  and having a parent node of those two. This parent node is added to a
 * the priority queue and and organized until all the children have been put into the queue of size one.
 */
EncodingTreeNode* buildHuffmanTree(string text) {
    PriorityQueue<EncodingTreeNode*> treeQueue;
    Map<char, int> letterMap;
    // Create frequency map
    for (int i = 0; i < text.size(); i++) {
        if (letterMap.containsKey(text[i])) {
            letterMap[text[i]] += 1;
        } else {
            letterMap[text[i]] = 1;
        }
    }
    // Add letters and organize them by frequency using priority queue
    for (char letter : letterMap) {
        treeQueue.enqueue(new EncodingTreeNode(letter), letterMap[letter]);
    }
    // Add children to parent and organize by priority using priority queue
    while (treeQueue.size() >= 2) {
        EncodingTreeNode* leftNode = treeQueue.dequeue();
        EncodingTreeNode* rightNode = treeQueue.dequeue();
        EncodingTreeNode* parent = new EncodingTreeNode(leftNode, rightNode);
        int totFrequency = textFrequency(parent, letterMap);
        treeQueue.enqueue(parent, totFrequency);
    }
    // Only one parent, with all the rest of the tree will remain, hence we only return the one value in the priority queue
    return treeQueue.dequeue();
}

/* This helper function traverses through the tree, adding the locations of the leaf nodes to the letterMap
 */
void traverse(EncodingTreeNode* &tree, string &location, Map<char, string> &letterMap) {
    EncodingTreeNode* tempTree = tree;
    // Add character key and its location value
    if (tempTree->isLeaf()) {
        letterMap[tempTree->getChar()] = location;
    } else {
        tempTree = tempTree->zero;
        location += '0';
        traverse(tempTree, location, letterMap);
        tempTree = tree;
        location.erase(location.size() - 1, 1);
        tempTree = tempTree->one;
        location += '1';
        traverse(tempTree, location, letterMap);
        location.erase(location.size() - 1, 1);
    }
}

/**
 * Given a string and an encoding tree, encode the text using the tree
 * and return a Queue<Bit> of the encoded bit sequence.
 *
 * You can assume tree is a valid non-empty encoding tree and contains an
 * encoding for every character in the text.
 *
 * Using the map with the keys as the leaf nodes, and the values as their locations relative to the tree,
 * we create the text by accessing the location of each character in order and add it to a queue until
 * all the letters have been looped through and the locations all enqueued.
 */
Queue<Bit> encodeText(EncodingTreeNode* tree, string text) {
    Map<char, string> letterMap;
    string location = "";
    // Create map of letters and their locations
    traverse(tree, location, letterMap);
    Queue<Bit> encoded;
    for (char letter: text) {
        for (int i = 0; i < letterMap[letter].size(); i++)
            // Enqueue each bit of location of that respective letter
            encoded.enqueue(charToInteger(letterMap[letter][i]));
    }
    return encoded;
}

/**
 * Flatten the given tree into a Queue<Bit> and Queue<char> in the manner
 * specified in the assignment writeup.
 *
 * You can assume the input queues are empty on entry to this function.
 *
 * You can assume tree is a valid well-formed encoding tree.
 *
 * Create a flat version of the tree by traversing through the tree and adding a 0 if there is a leaf and a 1
 * if not, going from the left node to the right node recursively.
 */
void flattenTree(EncodingTreeNode* tree, Queue<Bit>& treeShape, Queue<char>& treeLeaves) {
    EncodingTreeNode* tempTree = tree;
    if (tempTree->isLeaf()) {
        treeShape.enqueue(0);
        treeLeaves.enqueue(tempTree->ch);
    } else {
        // Enqueue bit 1 as this tree root was not a leaf
        treeShape.enqueue(1);
        flattenTree(tempTree->zero, treeShape, treeLeaves);
        flattenTree(tempTree->one, treeShape, treeLeaves);
    }
}

/**
 * Compress the input text using Huffman coding, producing as output
 * an EncodedData containing the encoded message and flattened
 * encoding tree used.
 *
 * Reports an error if the message text does not contain at least
 * two distinct characters.
 *
 * A message larger than two charaters can be run. If so, we creat a flat tree of the newly created Huffman tree
 * and new queues of tree shapes and tree leaves. With this, we can know where the tree leaves are and what the
 * tree shape is. We then add this new tree leaf and shape data into the data tree and add the message of the given
 * text using the tree.
 */
EncodedData compress(string messageText) {
    if (messageText.size() < 2)
        error("Input to be compressed shoudl contain at least two distinct characters to be Huffman-encodable.");
    EncodedData tree;
    Queue<Bit> treeShape;
    Queue<char> treeLeaves;
    EncodingTreeNode* huffmanTree = buildHuffmanTree(messageText);
    flattenTree(huffmanTree, treeShape, treeLeaves);
    tree.messageBits = encodeText(huffmanTree, messageText);
    tree.treeLeaves = treeLeaves;
    tree.treeShape = treeShape;
    deallocateTree(huffmanTree);
    return tree;
}

/* * * * * * Testing Helper Functions Below This Point * * * * * */

/* Manually create the leaf nodes and parent nodes of the given example tree
 */
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

/* Run through each node, in the zero and one direction, deleting each node as it traverses.
 */
void deallocateTree(EncodingTreeNode* t) {
      if (t->isLeaf() == false) {
        if (t->zero != nullptr) deallocateTree(t->zero);
        if (t->one != nullptr) deallocateTree(t->one);
      }
      delete t;
}

/* Check that the two trees are identical.
 */
bool areEqual(EncodingTreeNode* a, EncodingTreeNode* b) {
    // Check if one of the equal positioned nodes is a leaf and the other it not
    if (a->isLeaf() && !b->isLeaf())
        return false;
    if (!a->isLeaf() && b->isLeaf())
        return false;
    // Check if the equal positioned leaves have different characters assigned to them
    else if (a->isLeaf() && b->isLeaf()) {
        if (a->getChar() != b->getChar()) {
            return false;
        }
    }
    else {
        // Recurse through the trees as long as they are not nullptrs
        if (a->zero != nullptr && b->zero != nullptr)
            areEqual(a->zero, b->zero);
        if (a->one != nullptr && b->one != nullptr)
            areEqual(a->one, b->one);
        // If one of the two is a nullptr and the other isnt, return false
        if (a->zero == nullptr && b->zero != nullptr)
            return !(a->zero != nullptr && b->zero == nullptr);
        if (a->one == nullptr && b->one != nullptr)
            return !(a->one != nullptr && b->one == nullptr);
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

STUDENT_TEST("decodeText, small example encoding tree") {
    EncodingTreeNode* tree = createExampleTree(); // see diagram above
    EXPECT(tree != nullptr);

    Queue<Bit> messageBits = { 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1}; // TREES
    EXPECT_EQUAL(decodeText(tree, messageBits), "TREES");

    messageBits = { 0 }; // T
    EXPECT_EQUAL(decodeText(tree, messageBits), "T");

    messageBits = { 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0}; // SETER
    EXPECT_EQUAL(decodeText(tree, messageBits), "SETER");

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
