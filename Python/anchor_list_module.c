#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "anchor_list.h"

// Define Python wrapper object
typedef struct
{
    PyObject_HEAD AnchorList *el;
} AnchorListObject;

// Deallocation
static void AnchorList_dealloc(AnchorListObject *self)
{
    anchor_list_free(self->el);
    Py_TYPE(self)->tp_free((PyObject *)self);
}

// Constructor
static PyObject *AnchorList_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    AnchorListObject *self;
    self = (AnchorListObject *)type->tp_alloc(type, 0);
    if (self != NULL)
    {
        self->el = anchor_list_new();
        if (self->el == NULL)
        {
            Py_DECREF(self);
            return NULL;
        }
    }
    return (PyObject *)self;
}

// insert method
static PyObject *AnchorList_insert(AnchorListObject *self, PyObject *args)
{
    Py_ssize_t idx;
    PyObject *data;
    if (!PyArg_ParseTuple(args, "nO", &idx, &data))
        return NULL;

    anchor_list_insert(self->el, idx, data);
    Py_RETURN_NONE;
}

// remove method
static PyObject *AnchorList_remove(AnchorListObject *self, PyObject *args)
{
    Py_ssize_t idx;
    if (!PyArg_ParseTuple(args, "n", &idx))
        return NULL;

    anchor_list_remove(self->el, idx);
    Py_RETURN_NONE;
}

// __getitem__
static PyObject *AnchorList_getitem(AnchorListObject *self, Py_ssize_t idx)
{
    return anchor_list_get_item(self->el, idx); // Already returns PyObject*
}

// __len__
static Py_ssize_t AnchorList_len(AnchorListObject *self)
{
    return anchor_list_size(self->el);
}

// Method table
static PyMethodDef AnchorList_methods[] = {
    {"insert", (PyCFunction)AnchorList_insert, METH_VARARGS, "Insert at index"},
    {"remove", (PyCFunction)AnchorList_remove, METH_VARARGS, "Remove at index"},
    {NULL} // Sentinel
};

// Sequence protocol
static PySequenceMethods AnchorList_sequence_methods = {
    (lenfunc)AnchorList_len, // __len__
    0,                          // __add__, etc.
    0,
    (ssizeargfunc)AnchorList_getitem, // __getitem__
    0,
    0,
    0,
    0,
    0,
    0};

// Type object
static PyTypeObject AnchorListType = {
    PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "anchor_list.AnchorList",
    .tp_basicsize = sizeof(AnchorListObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "AnchorList backed by custom RBTree",
    .tp_new = AnchorList_new,
    .tp_dealloc = (destructor)AnchorList_dealloc,
    .tp_methods = AnchorList_methods,
    .tp_as_sequence = &AnchorList_sequence_methods,
};

// Module definition
static PyModuleDef anchor_list_module = {
    PyModuleDef_HEAD_INIT,
    "anchor_list",
    "Anchor list using a custom red-black tree",
    -1,
    NULL, NULL, NULL, NULL, NULL};

// Module init
PyMODINIT_FUNC PyInit_anchor_list(void)
{
    if (PyType_Ready(&AnchorListType) < 0)
        return NULL;

    PyObject *m = PyModule_Create(&anchor_list_module);
    if (!m)
        return NULL;

    Py_INCREF(&AnchorListType);
    PyModule_AddObject(m, "AnchorList", (PyObject *)&AnchorListType);
    return m;
}