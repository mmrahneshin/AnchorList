// Copyright (C) Kamaledin Ghiasi-Shirazi, Ferdowsi Univerity of Mashhad, 2024 (1404 Hijri Shamsi)
//
// Author: Kamaledin Ghiasi-Shirazi, Mohammad Mahdi Rahneshin

#pragma once

template <class T>
class EfficientList
{
public:
	typedef IndexedPPS23RedBlackBinaryTree<T> BT;
	typedef IndexedPPS23RBBinaryTreeNode<T> IPPS23RBBTN;

	EfficientList(void)
	{
		mIPPS23RBbt = new BT();
	}

	~EfficientList(void)
	{
		delete mIPPS23RBbt;
	}

	void insert(const int &idx, const T &data)
	{
		if (idx < 0 || idx > mIPPS23RBbt->size())
		{
			throw std::runtime_error("out_of_range");
		}

		if (mIPPS23RBbt->size() == 0)
		{
			mIPPS23RBbt->insertRootNode(data);
			return;
		}

		if (idx == mIPPS23RBbt->size())
		{
			mIPPS23RBbt->insertLastInOrderNode(data);
			return;
		}

		if (idx == 0)
		{
			mIPPS23RBbt->insertFirstInOrderNode(data);
			return;
		}

		IPPS23RBBTN *node = getNodeWithUpdateLeftSize(idx, 1);
		node->mLeftSize += 1;

		if (!mIPPS23RBbt->hasLeftChild(node))
		{
			mIPPS23RBbt->insertLeftChild(node, data);
		}
		else
		{
			node = node->mLeftChild;
			while (mIPPS23RBbt->hasRightChild(node))
				node = node->mRightChild;
			mIPPS23RBbt->insertRightChild(node, data);
		}
	}
	// double getTimeTaken()
	// {
	// 	return mIPPS23RBbt->timeTaken;
	// }

	// double getRemoveTimeTaken()
	// {
	// 	return mIPPS23RBbt->removeTimeTaken;
	// }

	T &operator[](const int &idx)
	{
		if (idx < 0 || idx >= mIPPS23RBbt->size())
		{
			throw std::runtime_error("out_of_range");
		}
		IPPS23RBBTN *node = getNodeWithUpdateLeftSize(idx, 0);

		return node->mData;
	}

	void remove(const int &idx)
	{
		if (idx < 0 || idx >= mIPPS23RBbt->size())
		{
			throw std::runtime_error("out_of_range");
		}

		if (idx == mIPPS23RBbt->size() - 1)
		{
			mIPPS23RBbt->deleteLastInOrderNode();
			return;
		}

		if (idx == 0)
		{
			mIPPS23RBbt->deleteFirstInOrderNode();
			return;
		}

		// cout << "#################" << endl;
		IPPS23RBBTN *node = getNodeWithUpdateLeftSize(idx, -1);

		mIPPS23RBbt->deleteNode(node);
	}

	int size() const
	{
		return mIPPS23RBbt->size();
	};

	// Exposes the underlying tree's invariant checker for testing/fuzzing.
	// See IndexedPPS23RedBlackBinaryTree::checkInvariants() for what it verifies.
	bool checkInvariants(std::ostream *err = nullptr) const
	{
		return mIPPS23RBbt->checkInvariants(err);
	}

	//  int depth() const
	// {
	// 	return mIPPS23RBbt->depthCalc(mIPPS23RBbt->getRootNode(), 1);
	// };
	// void drawTree()
	// {
	// 	mIPPS23RBbt->draw(cout);
	// }
	// void iterateInOrder()
	// {
	// 	typename BT::InOrderIterator itrInOrder = mIPPS23RBbt->inOrderBegin();
	// 	typename BT::InOrderIterator endInOrder = mIPPS23RBbt->inOrderEnd();
	// 	while (itrInOrder != endInOrder)
	// 	{
	// 		cout << "left size: " << itrInOrder.getBinaryTreeNode()->mLeftSize
	// 			 << " / value: " << *itrInOrder << "   ";
	// 		++itrInOrder;
	// 	}
	// 	cout << endl;
	// }

private:
	inline IPPS23RBBTN *getNodeWithUpdateLeftSize(int idx, int status)
	{
		IPPS23RBBTN *node = mIPPS23RBbt->mInOrderEnd;
		node->mLeftSize += status;

		node = node->mLeftChild;
		int leftOnly = 1;
		int leftSize = node->mLeftSize + mIPPS23RBbt->mGlobalLeftSize;
		while (leftSize != idx)
		{
			if (idx < leftSize)
			{
				node->mLeftSize += status;
				node = node->mLeftChild;
			}
			else
			{
				leftOnly = 0;
				idx = idx - leftSize - 1;
				node = node->mRightChild;
			}
			leftSize = node->mLeftSize + (mIPPS23RBbt->mGlobalLeftSize * leftOnly);
		}
		return node;
	}

	BT *mIPPS23RBbt;
};
