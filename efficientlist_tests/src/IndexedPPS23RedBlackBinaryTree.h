// Copyright (C) Kamaledin Ghiasi-Shirazi, Ferdowsi Univerity of Mashhad, 2024 (1404 Hijri Shamsi)
//
// Author: Kamaledin Ghiasi-Shirazi, Mohammad Mahdi Rahneshin

#pragma once

#include <ostream>
#include <algorithm>

using namespace std;

template <class T>
class IndexedPPS23RedBlackBinaryTree
{
	typedef IndexedPPS23RBBinaryTreeNode<T> IPPS23RBBTN;
	friend class EfficientList<T>;

public:
	IndexedPPS23RedBlackBinaryTree(void)
	{
		mSize = 0;
		mGlobalLeftSize = 0;
		mNilSentinel = new IPPS23RBBTN();
		mRevInOrderEnd = new IPPS23RBBTN();
		mInOrderEnd = new IPPS23RBBTN();

		mRevInOrderEnd->mRightChild = mInOrderEnd;
		mRevInOrderEnd->mLeftChild = mNilSentinel;

		mInOrderEnd->mLeftChild = mNilSentinel;
		mInOrderEnd->mRightChild = mNilSentinel;

		mLastInOrderNode = mInOrderEnd;
		mFirstInOrderNode = mInOrderEnd;

		mRevInOrderEnd->mLeftSize = -1;
		mInOrderEnd->mLeftSize = 0;

		// b1RightInsertion = 0;
		// b2RightInsertion = 0;
		// b1LeftInsertion = 0;
		// b2LeftInsertion = 0;
		// a1RightInsertion = 0;
		// a2RightInsertion = 0;
		// a1LeftInsertion = 0;
		// a2LeftInsertion = 0;
	}

	// caution: don't interpret destructor as an ordinary function!
	~IndexedPPS23RedBlackBinaryTree(void)
	{
		DeleteSubtree(mRevInOrderEnd);
		delete mNilSentinel;
	}

	int size() { return mSize; }

	bool isEmpty()
	{
		return mInOrderEnd->mLeftChild == mNilSentinel;
	}

	bool hasLeftChild(IPPS23RBBTN *node) const { return node->mLeftChild != mNilSentinel; }

	bool hasRightChild(IPPS23RBBTN *node) const { return node->mRightChild != mNilSentinel; }

	void insertRootNode(T data)
	{
		IPPS23RBBTN *root;
		if (hasLeftChild(mInOrderEnd))
			throw std::runtime_error("Error: Root already exists.");
		root = new IPPS23RBBTN();
		root->mData = data;
		root->mParent = mInOrderEnd;
		mInOrderEnd->mLeftChild = root;

		root->mLeftChild = mNilSentinel;
		root->mRightChild = mNilSentinel;

		mLastInOrderNode = root;
		mFirstInOrderNode = root;

		mGlobalLeftSize = 0;
		mRevInOrderEnd->mLeftSize = -1;
		mInOrderEnd->mLeftSize = 0;
		mSize = 1;
		mInOrderEnd->mLeftSize++;
	}

	void insertLastInOrderNode(T data)
	{
		IPPS23RBBTN *childNode = new IPPS23RBBTN();
		childNode->mData = data;
		childNode->mParent = mLastInOrderNode;
		childNode->mLeftChild = mNilSentinel;
		childNode->mRightChild = mNilSentinel;
		mLastInOrderNode->mRightChild = childNode;
		mSize++;
		mLastInOrderNode = childNode;

		mInOrderEnd->mLeftSize++;
		put_bottom_up_pass(childNode);
	}

	void insertFirstInOrderNode(T data)
	{
		IPPS23RBBTN *childNode = new IPPS23RBBTN();
		childNode->mData = data;
		childNode->mParent = mFirstInOrderNode;
		childNode->mLeftChild = mNilSentinel;
		childNode->mRightChild = mNilSentinel;
		mFirstInOrderNode->mLeftChild = childNode;
		mSize++;
		mFirstInOrderNode = childNode;

		// update mGlobalLeftSize
		mGlobalLeftSize++;

		mFirstInOrderNode->mLeftSize -= mGlobalLeftSize;
		put_bottom_up_pass(childNode);
	}

	// error if a left child already exists.
	void insertLeftChild(IPPS23RBBTN *parentNode, T data)
	{
		// Write your code here
		if (parentNode)
		{
			if (!hasLeftChild(parentNode))
			{
				IPPS23RBBTN *childNode = new IPPS23RBBTN();
				childNode->mData = data;
				childNode->mParent = parentNode;
				childNode->mLeftChild = mNilSentinel;
				childNode->mRightChild = mNilSentinel;
				parentNode->mLeftChild = childNode;
				mSize++;
				put_bottom_up_pass(childNode);
			}
			else
				throw std::runtime_error("Can't insert a left child for a node which already has a left child.");
		}
		else
			throw std::runtime_error("Invalid Pointer.");
	}

	// error if a right child already exists.
	void insertRightChild(IPPS23RBBTN *parentNode, T data)
	{
		// Write your code here
		if (parentNode)
		{
			if (!hasRightChild(parentNode))
			{
				IPPS23RBBTN *childNode = new IPPS23RBBTN();
				childNode->mData = data;
				childNode->mParent = parentNode;
				childNode->mLeftChild = mNilSentinel;
				childNode->mRightChild = mNilSentinel;
				parentNode->mRightChild = childNode;
				mSize++;
				put_bottom_up_pass(childNode);
			}
			else
				throw std::runtime_error("Can't insert a right child for a node which already has a right child.");
		}
		else
			throw std::runtime_error("Invalid Pointer.");
	}

	class Iterator
	{
	public:
		Iterator(IPPS23RBBTN *currentNode, IPPS23RBBTN *nil) : mCurrentNode(currentNode), mNilSentinel(nil) {}
		virtual T &operator*() { return mCurrentNode->mData; }
		virtual T *operator->() { return &mCurrentNode->mData; }
		virtual bool operator==(Iterator &itr) { return mCurrentNode == itr.mCurrentNode; }
		virtual bool operator!=(Iterator &itr) { return mCurrentNode != itr.mCurrentNode; }
		virtual void operator++() = 0;
		virtual void operator--() = 0;
		virtual IPPS23RBBTN *getBinaryTreeNode() { return mCurrentNode; }

	protected:
		IPPS23RBBTN *mCurrentNode;
		IPPS23RBBTN *mNilSentinel;
		friend class IndexedPPS23RedBlackBinaryTree<T>;
	};
	class InOrderIterator : public Iterator
	{
	public:
		InOrderIterator(IPPS23RBBTN *currentNode, IPPS23RBBTN *nil) : Iterator(currentNode, nil) {}
		virtual void operator++()
		{
			// Write your code here
			IPPS23RBBTN *theNode = this->mCurrentNode;
			if (theNode->mRightChild != this->mNilSentinel)
			{
				theNode = theNode->mRightChild;
				while (theNode->mLeftChild != this->mNilSentinel)
				{
					theNode = theNode->mLeftChild;
				}
			}
			else
			{
				IPPS23RBBTN *lastNode = theNode;
				do
				{
					lastNode = theNode;
					theNode = theNode->mParent;
				} while (theNode->mLeftChild != lastNode);
			}
			this->mCurrentNode = theNode;
		}
		virtual void operator--()
		{
			// Write your code here
			IPPS23RBBTN *theNode = this->mCurrentNode;
			if (theNode->mLeftChild != this->mNilSentinel)
			{
				theNode = theNode->mLeftChild;
				while (theNode->mRightChild != this->mNilSentinel)
				{
					theNode = theNode->mRightChild;
				}
			}
			else
			{
				IPPS23RBBTN *lastNode = theNode;
				do
				{
					lastNode = theNode;
					theNode = theNode->mParent;
				} while (theNode->mRightChild != lastNode);
			}
			this->mCurrentNode = theNode;
		}
	};

	//  void deleteLeftChild(IPPS23RBBTN *node)
	// {
	// 	IPPS23RBBTN *theParent = node;
	// 	IPPS23RBBTN *theNode = theParent->mLeftChild;
	// 	deleteNode(theNode, theParent);
	// }
	//  void deleteRightChild(IPPS23RBBTN *node)
	// {
	// 	IPPS23RBBTN *theParent = node;
	// 	IPPS23RBBTN *theNode = theParent->mRightChild;
	// 	deleteNode(theNode, theParent);
	// }

	InOrderIterator inOrderBegin()
	{
		// Write your code here
		InOrderIterator itr(mFirstInOrderNode, mNilSentinel);
		return itr;
	}

	InOrderIterator inOrderEnd()
	{
		return InOrderIterator(mInOrderEnd, mNilSentinel);
	}

	virtual InOrderIterator inOrderReverseBegin()
	{
		// Write your code here
		InOrderIterator itr(mLastInOrderNode, mNilSentinel);
		return itr;
	}

	virtual InOrderIterator inOrderReverseEnd()
	{
		return InOrderIterator(mRevInOrderEnd, mNilSentinel);
	}
	// int b1RightInsertion;
	// int b1LeftInsertion;
	// int b2RightInsertion;
	// int b2LeftInsertion;

	// int a1RightInsertion;
	// int a1LeftInsertion;
	// int a2RightInsertion;
	// int a2LeftInsertion;

	// ---- Invariant / property checker (for testing only) ----
	//
	// Walks the whole tree and verifies, in one pass:
	//   1. Black-height is the same on every root-to-nilSentinel path.
	//   2. No node has two red children simultaneously (the "Sibling"
	//      property of the pure 2-3 variant: a red node must not have a
	//      red sibling -- equivalent to forbidding two red children under
	//      one parent).
	//   3. mLeftSize is correct at every node, accounting for
	//      mGlobalLeftSize on exactly the nodes on the path to index 0
	//      (Section 3.2 of the paper: "actual leftSize" = stored + global
	//      only while we have never yet descended into a right child).
	//   4. mFirstInOrderNode / mLastInOrderNode really are the leftmost /
	//      rightmost real nodes.
	//   5. mSize matches the actual number of real nodes found.
	//   6. Parent pointers agree with child pointers both ways.
	//
	// Returns true iff every check passes. If `err` is non-null, a
	// human-readable description of the *first* violation found is written
	// to it (checking stops at the first violation per subtree branch, but
	// multiple independent violations across branches may all be
	// accumulated -- this is a test tool, not hot-path code, so it favors
	// clarity over early-exit speed).
	bool checkInvariants(std::ostream *err = nullptr) const
	{
		bool ok = true;
		CheckResult result = checkNode(mInOrderEnd->mLeftChild, /*onSpine=*/true, ok, err);

		if (result.count != mSize)
		{
			ok = false;
			if (err) *err << "mSize mismatch: mSize=" << mSize
						  << " actual node count=" << result.count << "\n";
		}

		// root color must be black (root property)
		if (mInOrderEnd->mLeftChild != mNilSentinel &&
			mInOrderEnd->mLeftChild->mColor != IPPS23RBBTN::BLACK)
		{
			ok = false;
			if (err) *err << "root is not black\n";
		}

		// firstInOrder / lastInOrder correctness
		IPPS23RBBTN *walk = mInOrderEnd->mLeftChild;
		if (walk == mNilSentinel)
		{
			if (mFirstInOrderNode != mInOrderEnd || mLastInOrderNode != mInOrderEnd)
			{
				ok = false;
				if (err) *err << "empty tree but first/lastInOrder not reset to mInOrderEnd\n";
			}
		}
		else
		{
			IPPS23RBBTN *leftmost = walk;
			while (leftmost->mLeftChild != mNilSentinel) leftmost = leftmost->mLeftChild;
			if (leftmost != mFirstInOrderNode)
			{
				ok = false;
				if (err) *err << "mFirstInOrderNode does not point to the actual leftmost node\n";
			}
			IPPS23RBBTN *rightmost = walk;
			while (rightmost->mRightChild != mNilSentinel) rightmost = rightmost->mRightChild;
			if (rightmost != mLastInOrderNode)
			{
				ok = false;
				if (err) *err << "mLastInOrderNode does not point to the actual rightmost node\n";
			}
		}

		return ok;
	}

private:
	//

	IPPS23RBBTN *mRevInOrderEnd;
	IPPS23RBBTN *mInOrderEnd;

	IPPS23RBBTN *mNilSentinel;
	IPPS23RBBTN *mLastInOrderNode;
	IPPS23RBBTN *mFirstInOrderNode;
	int mGlobalLeftSize;
	int mSize;

	void DeleteSubtree(IPPS23RBBTN *node)
	{
		if (hasLeftChild(node))
			DeleteSubtree(node->mLeftChild);
		if (hasRightChild(node))
			DeleteSubtree(node->mRightChild);
		delete node;
	}

	// Recursive helper for checkInvariants(): a single O(n) traversal that
	// returns both the real-node count and the black-height of the subtree
	// rooted at `node`, checking every invariant along the way. (An earlier
	// draft measured black-height with a separate sub-traversal per node,
	// making the whole check O(n^2) -- fine for small fuzz-test trees, but
	// unnecessarily slow; this version is linear.)
	struct CheckResult { int count; int blackHeight; };

	CheckResult checkNode(IPPS23RBBTN *node, bool onSpine, bool &ok, std::ostream *err) const
	{
		if (node == mNilSentinel) return {0, 0};

		CheckResult left = checkNode(node->mLeftChild, onSpine, ok, err);
		CheckResult right = checkNode(node->mRightChild, false, ok, err);

		// (3) mLeftSize correctness, with the globalLeftSize adjustment
		// applying only while still on the path to index 0.
		int expectedStored = onSpine ? (left.count - mGlobalLeftSize) : left.count;
		if (node->mLeftSize != expectedStored)
		{
			ok = false;
			if (err) *err << "mLeftSize mismatch at a node: stored=" << node->mLeftSize
						  << " expected=" << expectedStored
						  << " (onSpine=" << onSpine << ", globalLeftSize=" << mGlobalLeftSize << ")\n";
		}

		// (2) no two red children under one parent (2-3 "Sibling" property)
		bool leftReal = hasLeftChild(node), rightReal = hasRightChild(node);
		bool leftRed = leftReal && node->mLeftChild->mColor == IPPS23RBBTN::RED;
		bool rightRed = rightReal && node->mRightChild->mColor == IPPS23RBBTN::RED;
		if (leftRed && rightRed)
		{
			ok = false;
			if (err) *err << "node has two red children (Sibling property violated)\n";
		}

		// (6) parent pointer consistency
		if (leftReal && node->mLeftChild->mParent != node)
		{
			ok = false;
			if (err) *err << "left child's mParent does not point back to this node\n";
		}
		if (rightReal && node->mRightChild->mParent != node)
		{
			ok = false;
			if (err) *err << "right child's mParent does not point back to this node\n";
		}

		// (1) black-height consistency
		if (left.blackHeight != right.blackHeight)
		{
			ok = false;
			if (err) *err << "black-height mismatch: left=" << left.blackHeight
						  << " right=" << right.blackHeight << "\n";
		}

		int bh = std::max(left.blackHeight, right.blackHeight) +
				 (node->mColor == IPPS23RBBTN::BLACK ? 1 : 0);
		return {left.count + right.count + 1, bh};
	}

	inline void updateAfterRR(IPPS23RBBTN *node)
	{
		// IndexedBT updateAfterRR(node);
		IPPS23RBBTN *A = node->mLeftChild;
		node->mLeftSize += A->mLeftSize + 1; // B->mLeftSize = [C->mLeftSize + 1](is B->mLeftSize) + A->mLeftSize + 1

		// PPS23RB updateAfterRR(node);
		typename IPPS23RBBTN::COLOR c = node->mColor;
		node->mColor = node->mLeftChild->mColor;
		node->mLeftChild->mColor = c;
	}

	inline IPPS23RBBTN *RR(IPPS23RBBTN *A)
	{
		// write your code here
		IPPS23RBBTN *B = A->mRightChild;
		A->mRightChild = B->mLeftChild;

		if (hasLeftChild(B))
		{
			B->mLeftChild->mParent = A;
		}

		if (A->mParent->mLeftChild == A)
		{
			A->mParent->mLeftChild = B;
		}
		else
		{
			A->mParent->mRightChild = B;
		}

		B->mParent = A->mParent;
		A->mParent = B;
		B->mLeftChild = A;

		updateAfterRR(B);

		return B;
	}

	inline void updateAfterLL(IPPS23RBBTN *node)
	{
		// IndexedBT updateAfterLL(node);
		IPPS23RBBTN *A = node->mRightChild;
		A->mLeftSize -= node->mLeftSize + 1;

		// PPS23RB updateAfterLL(node);
		typename IPPS23RBBTN::COLOR c = node->mColor;
		node->mColor = node->mRightChild->mColor;
		node->mRightChild->mColor = c;
	}

	inline IPPS23RBBTN *LL(IPPS23RBBTN *A)
	{
		// write your code here
		IPPS23RBBTN *B = A->mLeftChild;
		A->mLeftChild = B->mRightChild;

		if (hasRightChild(B))
		{
			B->mRightChild->mParent = A;
		}
		if (A->mParent->mLeftChild == A)
		{
			A->mParent->mLeftChild = B;
		}
		else
		{
			A->mParent->mRightChild = B;
		}

		B->mParent = A->mParent;
		A->mParent = B;
		B->mRightChild = A;

		updateAfterLL(B);
		return B;
	}

	inline void put_bottom_up_pass(IPPS23RBBTN *deficientNode)
	{
		IPPS23RBBTN *parentNode, *siblingNode;
		while (true) // deficientNode->mColor is certainly BLACK
		{
			parentNode = deficientNode->mParent;
			if (parentNode->mColor == IPPS23RBBTN::RED)
			{
				if (parentNode == parentNode->mParent->mLeftChild)
				{
					if (deficientNode == parentNode->mRightChild) // Rule (b.2)
					{
						deficientNode = parentNode;
						parentNode = this->RR(parentNode);
						// b2LeftInsertion++;
					}
					parentNode = this->LL(parentNode->mParent); // Rule (b.1)
																// b1LeftInsertion++;
				}
				else
				{
					if (deficientNode == parentNode->mLeftChild) // Rule (b.2)
					{
						deficientNode = parentNode;
						parentNode = this->LL(parentNode);
						// b2RightInsertion++;
					}
					parentNode = this->RR(parentNode->mParent); // Rule (b.1)
																// b1RightInsertion++;
				}
			}
			siblingNode = (deficientNode == parentNode->mLeftChild) ? parentNode->mRightChild : parentNode->mLeftChild;

			if (siblingNode->mColor == IPPS23RBBTN::RED) // Rule (a.2)
			{
				// if (deficientNode == parentNode->mLeftChild)
				// {
				// 	a2LeftInsertion++;
				// }
				// else
				// {
				// 	a2RightInsertion++;
				// }
				siblingNode->mColor = IPPS23RBBTN::BLACK;
				deficientNode = parentNode;
			}
			else if (parentNode->mColor == IPPS23RBBTN::BLACK)
				break;
		}
		if (deficientNode != this->mInOrderEnd->mLeftChild)
		{
			deficientNode->mColor = IPPS23RBBTN::RED; // Rule (a.1)
													  // if (deficientNode == parentNode->mLeftChild)
													  // {
													  // 	a1LeftInsertion++;
													  // }
													  // else
													  // {
													  // 	a1RightInsertion++;
													  // }
		} // x->mColor is certainly BLACK
	}

	inline void remove_bottom_up_pass(IPPS23RBBTN *x)
	{
		while (x != this->mInOrderEnd->mLeftChild && x->mColor == IPPS23RBBTN::BLACK)
		{
			x = applyParitySeekingRules(x);
		}
		x->mColor = IPPS23RBBTN::BLACK; // Parity-seeking Rule (a)
	}

	inline IPPS23RBBTN *applyParitySeekingRules(IPPS23RBBTN *x)
	{
		IPPS23RBBTN *p = x->mParent;
		IPPS23RBBTN *y = x == p->mLeftChild ? p->mRightChild : p->mLeftChild;
		if (y->mColor == IPPS23RBBTN::RED) // Parity-seeking Rule (c): Rotate to have a black sibling
			if (x == p->mLeftChild)
				this->RR(p);
			else
				this->LL(p);
		else
		{ // Parity-seeking Rule (b): Make the sibling red
			y->mColor = IPPS23RBBTN::RED;
			x = p;
			x = fixUp(x, y);
		}
		return x;
	}

	inline IPPS23RBBTN *fixUp(IPPS23RBBTN *x, IPPS23RBBTN *z)
	{
		if (z->mLeftChild->mColor == IPPS23RBBTN::RED || z->mRightChild->mColor == IPPS23RBBTN::RED)
		{
			if (z == x->mRightChild)
			{
				if (z->mRightChild->mColor == IPPS23RBBTN::BLACK) // Fixing rule (a)
					this->LL(z);
				x = this->RR(x); // Fixing rule (b)
			}
			else
			{
				if (z->mLeftChild->mColor == IPPS23RBBTN::BLACK) // Fixing rule (a)
					this->RR(z);
				x = this->LL(x); // Fixing rule (b)
			}
			x->mLeftChild->mColor = IPPS23RBBTN::BLACK;	 // Fixing rule (c)
			x->mRightChild->mColor = IPPS23RBBTN::BLACK; // Fixing rule (c)
			x = this->mInOrderEnd->mLeftChild;			 // terminate the bottom up pass
		}
		return x;
	}

	inline void updateAfterDelete(IPPS23RBBTN *parentNode)
	{
		if ((hasLeftChild(parentNode) && !hasRightChild(parentNode)) ||
			(!hasLeftChild(parentNode) && hasRightChild(parentNode)))
		{
			this->mNilSentinel->mParent = parentNode;
			remove_bottom_up_pass(this->mNilSentinel);
			this->mNilSentinel->mParent = 0;
		}
	}

	inline IPPS23RBBTN *getActualDeleteNode(IPPS23RBBTN *theNode)
	{
		IPPS23RBBTN *actualDeleteNode = theNode;
		if (hasLeftChild(theNode))
		{
			theNode->mLeftSize--;
			actualDeleteNode = theNode->mLeftChild;
			while (hasRightChild(actualDeleteNode))
			{
				actualDeleteNode = actualDeleteNode->mRightChild;
			}
			theNode->mData = actualDeleteNode->mData;
			if (hasLeftChild(actualDeleteNode))
			{
				actualDeleteNode->mData = actualDeleteNode->mLeftChild->mData;
				actualDeleteNode->mLeftSize--;
				actualDeleteNode = actualDeleteNode->mLeftChild;
			}
			if (actualDeleteNode == mFirstInOrderNode)
			{
				// mGlobalLeftSize--;
				mFirstInOrderNode = actualDeleteNode->mParent;
			}
		}
		else if (hasRightChild(theNode))
		{
			actualDeleteNode = theNode->mRightChild;
			if (actualDeleteNode == mLastInOrderNode)
			{
				mLastInOrderNode = theNode;
			}
			theNode->mData = actualDeleteNode->mData;
		}

		return actualDeleteNode;
	}

	// If a degree-1 node is deleted, it is replaced by its subtree.
	void deleteNode(IPPS23RBBTN *theNode)
	{
		IPPS23RBBTN *actualDeleteNode = getActualDeleteNode(theNode);

		IPPS23RBBTN *theParent = deleteNodeAndGetParent(actualDeleteNode);
		updateAfterDelete(theParent);
	}

	inline IPPS23RBBTN *deleteNodeAndGetParent(IPPS23RBBTN *theNode)
	{
		IPPS23RBBTN *theParent = theNode->mParent;

		if (theParent->mRightChild == theNode)
			theParent->mRightChild = mNilSentinel;
		else
			theParent->mLeftChild = mNilSentinel;

		delete theNode;
		mSize--;
		return theParent;
	}

	void deleteLastInOrderNode()
	{
		// start = clock();
		// if (mLastInOrderNode->mLeftChild == mFirstInOrderNode)
		// {
		// 	mFirstInOrderNode->mData = mFirstInOrderNode->mData;
		// 	deleteFirstInOrderNode();
		// 	return;
		// }

		IPPS23RBBTN *actualDeleteNode = getActualDeleteNode(mLastInOrderNode);

		mInOrderEnd->mLeftSize--;

		IPPS23RBBTN *theParent = deleteNodeAndGetParent(actualDeleteNode);
		mLastInOrderNode = theParent;

		// FIX (found by checkInvariants() fuzzing): if the tree is now
		// empty, mFirstInOrderNode was never reset -- it's left pointing
		// at the just-freed node. theParent == mInOrderEnd exactly when
		// the deleted node was the root (i.e. the tree is now empty), so
		// this check is equivalent to "if (mSize == 0)" without needing
		// an extra branch on mSize.
		if (theParent == mInOrderEnd)
		{
			mFirstInOrderNode = mInOrderEnd;
		}

		// end_clock = clock();
		// removeTimeTaken += double(end_clock - start) / double(CLOCKS_PER_SEC);

		// start = clock();
		updateAfterDelete(mLastInOrderNode);
		// end_clock = clock();
		// timeTaken += double(end_clock - start) / double(CLOCKS_PER_SEC);
	}

	void deleteFirstInOrderNode()
	{
		// start = clock();

		// if (mFirstInOrderNode->mRightChild == mLastInOrderNode)
		// {
		// 	mFirstInOrderNode->mData = mLastInOrderNode->mData;
		// 	deleteLastInOrderNode();
		// 	return;
		// }
		IPPS23RBBTN *actualDeleteNode = getActualDeleteNode(mFirstInOrderNode);
		if (actualDeleteNode != mFirstInOrderNode)
		{
			// IPPS23RBBTN *iNode = mFirstInOrderNode;
			// while (iNode->mParent)
			// {
			// 	if (iNode == iNode->mParent->mLeftChild)
			// 		iNode->mParent->mLeftSize -= 1;
			// 	iNode = iNode->mParent;
			// }
			mFirstInOrderNode->mLeftSize++;
		}

		mGlobalLeftSize--;

		IPPS23RBBTN *theParent = deleteNodeAndGetParent(actualDeleteNode);
		mFirstInOrderNode = theParent;

		// Defensive symmetric guard (see the analogous fix in
		// deleteLastInOrderNode()): today this function is only ever
		// reached via EfficientList::remove() when idx==0 AND size>1
		// (a size==1 removal is caught by the idx==size()-1 check first),
		// so theParent==mInOrderEnd can't currently happen here -- but if
		// that dispatch order ever changes, this keeps the invariant safe
		// instead of silently leaving mLastInOrderNode dangling.
		if (theParent == mInOrderEnd)
		{
			mLastInOrderNode = mInOrderEnd;
		}
		// end_clock = clock();
		// removeTimeTaken += double(end_clock - start) / double(CLOCKS_PER_SEC);

		// start = clock();
		updateAfterDelete(mFirstInOrderNode);
		// end_clock = clock();
		// timeTaken += double(end_clock - start) / double(CLOCKS_PER_SEC);
	}
};
