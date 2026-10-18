#ifndef ANCHOR_LIST_H
#define ANCHOR_LIST_H
#ifdef __cplusplus
extern "C"
{
#endif
#include "indexed_PPS23RB_binaryTree.h"

    typedef struct AnchorList
    {
        IPPRBBT *mIPPS23RBbt;
    } AnchorList;

    AnchorList *anchor_list_new(void)
    {
        AnchorList *el = (AnchorList *)malloc(sizeof(AnchorList));
        if (!el)
        {
            return nullptr;
        }

        el->mIPPS23RBbt = ipps23rbbt_new();
        if (!el->mIPPS23RBbt)
        {
            return nullptr;
        }

        return el;
    }

    void anchor_list_free(AnchorList *el)
    {
        ipps23rbbt_free(el->mIPPS23RBbt);
        free(el);
    }

    static inline IPPS23RBBTN *getNodeWithUpdateLeftSize(IPPRBBT *mIPPS23RBbt, int idx, int status)
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

    void anchor_list_insert(AnchorList *el, int idx, int data)
    {
        IPPRBBT *mIPPS23RBbt = el->mIPPS23RBbt;
        if (idx < 0 || idx > mIPPS23RBbt->mSize)
        {
            return;
        }

        if (mIPPS23RBbt->mSize == 0)
        {
            ipps23rbbt_insert_root_node(mIPPS23RBbt, data);
            return;
        }

        if (idx == mIPPS23RBbt->mSize)
        {
            ipps23rbbt_insert_last_inorder_node(mIPPS23RBbt, data);
            return;
        }

        if (idx == 0)
        {
            ipps23rbbt_insert_first_inorder_node(mIPPS23RBbt, data);
            return;
        }

        IPPS23RBBTN *node = getNodeWithUpdateLeftSize(mIPPS23RBbt, idx, 1);
        node->mLeftSize++;

        if (!ipps23rbbt_has_left_child(node, mIPPS23RBbt->mNilSentinel))
        {
            ipps23rbbt_insert_left_child(mIPPS23RBbt, node, data);
        }
        else
        {
            node = node->mLeftChild;
            while (ipps23rbbt_has_right_child(node, mIPPS23RBbt->mNilSentinel))
                node = node->mRightChild;
            ipps23rbbt_insert_right_child(mIPPS23RBbt, node, data);
        }
    }

    void anchor_list_remove(AnchorList *el, int idx)
    {
        IPPRBBT *mIPPS23RBbt = el->mIPPS23RBbt;
        if (idx < 0 || idx >= mIPPS23RBbt->mSize)
        {
            return;
        }

        if (idx == mIPPS23RBbt->mSize - 1)
        {
            ipps23rbbt_delete_last_inorder_node(mIPPS23RBbt);
            return;
        }

        if (idx == 0)
        {
            ipps23rbbt_delete_first_inorder_node(mIPPS23RBbt);
            return;
        }

        IPPS23RBBTN *node = getNodeWithUpdateLeftSize(mIPPS23RBbt, idx, -1);
        ipps23rbbt_delete_node(mIPPS23RBbt, node);
    }

    int anchor_list_get_item(AnchorList *el, int idx)
    {
        IPPRBBT *mIPPS23RBbt = el->mIPPS23RBbt;
        if (idx < 0 || idx >= mIPPS23RBbt->mSize)
        {
            return -1;
        }
        IPPS23RBBTN *node = getNodeWithUpdateLeftSize(mIPPS23RBbt, idx, 0);

        return node->mData;
    }

    int anchor_list_size(AnchorList *el)
    {
        return el->mIPPS23RBbt->mSize;
    }

#ifdef __cplusplus
}
#endif
#endif
