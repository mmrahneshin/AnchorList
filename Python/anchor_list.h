#ifndef ANCHOR_LIST_H
#define ANCHOR_LIST_H

#include <Python.h> // For PyObject*
#include "indexed_PPS23RB_binaryTree.h"

typedef struct AnchorList
{
    IPPRBBT *mIPPS23RBbt;
} AnchorList;

AnchorList *anchor_list_new(void);
void anchor_list_free(AnchorList *self);
void anchor_list_insert(AnchorList *el, Py_ssize_t idx, PyObject *data);
void anchor_list_remove(AnchorList *el, Py_ssize_t idx);

PyObject *anchor_list_get_item(AnchorList *el, Py_ssize_t idx);
Py_ssize_t anchor_list_size(AnchorList *el);
#endif
