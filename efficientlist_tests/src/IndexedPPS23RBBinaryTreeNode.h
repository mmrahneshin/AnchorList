// Copyright (C) Kamaledin Ghiasi-Shirazi, Ferdowsi Univerity of Mashhad, 2024 (1404 Hijri Shamsi)
// Author: Kamaledin Ghiasi-Shirazi, Mohammad Mahdi Rahneshin

#pragma once
template <class T>
class EfficientList;

template <class T>
class IndexedPPS23RedBlackBinaryTree;

template <class T>
class IndexedPPS23RBBinaryTreeNode
{
private:
	enum COLOR
	{
		RED = 0,
		BLACK = 1
	};

	IndexedPPS23RBBinaryTreeNode(void)
	{
		this->mLeftChild = 0;
		this->mRightChild = 0;
		this->mParent = 0;
		this->mLeftSize = 0;
		this->mColor = BLACK;
	}

	virtual ~IndexedPPS23RBBinaryTreeNode(void)
	{
	}

	friend class IndexedPPS23RedBlackBinaryTree<T>;
	friend class EfficientList<T>;

	enum COLOR mColor;
	T mData;
	int mLeftSize;
	IndexedPPS23RBBinaryTreeNode *mLeftChild;
	IndexedPPS23RBBinaryTreeNode *mRightChild;
	IndexedPPS23RBBinaryTreeNode *mParent;
};
