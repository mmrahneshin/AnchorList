// Copyright (C) Kamaledin Ghiasi-Shirazi, Ferdowsi Univerity of Mashhad, 2024 (1404 Hijri Shamsi)
//
// Author: Kamaledin Ghiasi-Shirazi, Mohammad Mahdi Rahneshin

#pragma once

using namespace std;

template <class T>
class IndexedPPS23RedBlackBinaryTree
{
	typedef IndexedPPS23RBBinaryTreeNode<T> IPPS23RBBTN;
	friend class AnchorList<T>;

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
		// end_clock = clock();
		// removeTimeTaken += double(end_clock - start) / double(CLOCKS_PER_SEC);

		// start = clock();
		updateAfterDelete(mFirstInOrderNode);
		// end_clock = clock();
		// timeTaken += double(end_clock - start) / double(CLOCKS_PER_SEC);
	}
};
