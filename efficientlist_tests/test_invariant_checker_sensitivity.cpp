// test_invariant_checker_sensitivity.cpp
//
// A test suite is only as trustworthy as its ability to actually FAIL when
// something is wrong. This file deliberately corrupts the tree's internal
// state in four different ways (one per invariant category) and asserts
// that checkInvariants() catches each one. Without this, checkInvariants()
// passing on all the "normal" fuzz tests could just mean the checker itself
// is a no-op that always returns true.
//
// This uses the `#define private public` trick to reach internal fields
// from outside the class, kept in a separate translation unit from the
// main test suite so the real library code is never compiled with altered
// access specifiers.

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <sstream>

// Only the library headers are compiled with private->public; doctest and
// the standard library must be included BEFORE this macro is defined, or
// their own use of `private:` gets corrupted too (this bit me on the first
// attempt: including doctest.h while the macro was active broke <sstream>'s
// internal access specifiers and failed to compile).
#define private public
#include "src/IndexedPPS23RBBinaryTreeNode.h"
#include "src/IndexedPPS23RedBlackBinaryTree.h"
#include "src/EfficientList.h"
#undef private

using Tree = IndexedPPS23RedBlackBinaryTree<int>;
using Node = IndexedPPS23RBBinaryTreeNode<int>;

TEST_CASE("sanity: checkInvariants() passes on an untouched tree") {
    EfficientList<int> list;
    for (int i = 0; i < 200; ++i) list.insert(list.size(), i);
    for (int i = 0; i < 100; ++i) list.insert(0, i + 1000);
    CHECK(list.checkInvariants());
}

TEST_CASE("sensitivity: corrupted mLeftSize is caught") {
    EfficientList<int> list;
    for (int i = 0; i < 50; ++i) list.insert(list.size(), i);
    REQUIRE(list.checkInvariants());

    // reach into the tree and corrupt one node's cached left-subtree size
    Tree *tree = list.mIPPS23RBbt;
    Node *root = tree->mInOrderEnd->mLeftChild;
    REQUIRE(root != tree->mNilSentinel);
    root->mLeftSize += 7; // wrong on purpose

    std::ostringstream err;
    bool ok = list.checkInvariants(&err);
    CHECK_FALSE(ok);
    CHECK(err.str().find("mLeftSize mismatch") != std::string::npos);
}

TEST_CASE("sensitivity: two red children (Sibling property) is caught") {
    EfficientList<int> list;
    for (int i = 0; i < 50; ++i) list.insert(list.size(), i);
    REQUIRE(list.checkInvariants());

    Tree *tree = list.mIPPS23RBbt;
    Node *root = tree->mInOrderEnd->mLeftChild;
    REQUIRE(root != tree->mNilSentinel);
    // force both children red, regardless of what they were
    if (tree->hasLeftChild(root)) root->mLeftChild->mColor = Node::RED;
    if (tree->hasRightChild(root)) root->mRightChild->mColor = Node::RED;

    std::ostringstream err;
    bool ok = list.checkInvariants(&err);
    CHECK_FALSE(ok);
    CHECK(err.str().find("two red children") != std::string::npos);
}

TEST_CASE("sensitivity: wrong mFirstInOrderNode is caught") {
    EfficientList<int> list;
    for (int i = 0; i < 50; ++i) list.insert(list.size(), i);
    REQUIRE(list.checkInvariants());

    Tree *tree = list.mIPPS23RBbt;
    Node *root = tree->mInOrderEnd->mLeftChild;
    tree->mFirstInOrderNode = root; // almost certainly not the real leftmost node

    std::ostringstream err;
    bool ok = list.checkInvariants(&err);
    CHECK_FALSE(ok);
    CHECK(err.str().find("leftmost") != std::string::npos);
}

TEST_CASE("sensitivity: broken black-height is caught") {
    EfficientList<int> list;
    // enough elements that the tree has real depth for this to bite
    for (int i = 0; i < 500; ++i) list.insert(list.size(), i);
    REQUIRE(list.checkInvariants());

    Tree *tree = list.mIPPS23RBbt;
    Node *root = tree->mInOrderEnd->mLeftChild;
    // walk down the left spine a few levels and flip a black node to red
    // (or vice versa) to unbalance the black-height on one side
    Node *n = root;
    for (int i = 0; i < 3 && tree->hasLeftChild(n); ++i) n = n->mLeftChild;
    n->mColor = (n->mColor == Node::RED) ? Node::BLACK : Node::RED;

    std::ostringstream err;
    bool ok = list.checkInvariants(&err);
    // flipping one node's color can trip EITHER the black-height check or
    // the sibling/root-color check depending on where in the tree it
    // landed -- either is an acceptable catch, so just require *some*
    // failure was detected, and print what was found for a human to check.
    INFO("checkInvariants report: ", err.str());
    CHECK_FALSE(ok);
}
