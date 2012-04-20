#include <Python.h>
#include "structmember.h"


#define atomic_int_add(ptr, val) __sync_add_and_fetch(ptr, val)
#define atomic_int_sub(ptr, val) __sync_sub_and_fetch(ptr, val)


typedef struct {
    PyObject_HEAD
    int number;
} AtomicInt;


static void
AtomicInt_dealloc(AtomicInt* self)
{
    self->ob_type->tp_free((PyObject*)self);
}


static PyObject*
AtomicInt_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    AtomicInt *self;

    self = (AtomicInt *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->number = 0;
    }

    return (PyObject *)self;
}


static int
AtomicInt_init(AtomicInt *self, PyObject *args)
{
    if (! PyArg_ParseTuple(args, "i", &self->number))
        return -1;
    return 0;
}


static PyMemberDef AtomicInt_members[] = {
    {"number", T_INT, offsetof(AtomicInt, number), 0, "atomic number"},
    {NULL}  /* Sentinel */
};


PyObject*
AtomicInt_increment(AtomicInt *self)
{
    Py_INCREF(self);
    atomic_int_add(&self->number, 1);
    Py_DECREF(self);
    return Py_BuildValue("");
}


PyObject*
AtomicInt_increment_by(AtomicInt *self, PyObject *args)
{
    int value;
    if (! PyArg_ParseTuple(args, "i", &value))
        return NULL;
    Py_INCREF(self);
    atomic_int_add(&self->number, value);
    Py_DECREF(self);
    return Py_BuildValue("");
}


PyObject*
AtomicInt_decrement_by(AtomicInt *self, PyObject *args)
{
    int value;
    if (! PyArg_ParseTuple(args, "i", &value))
        return NULL;
    Py_INCREF(self);
    atomic_int_sub(&self->number, value);
    Py_DECREF(self);
    return Py_BuildValue("");
}


static PyObject*
AtomicInt_repr(AtomicInt* obj)
{
    return PyString_FromFormat("AtomicInt(%d)", obj->number);
}

static PyObject*
AtomicInt_str(AtomicInt* obj)
{
    return PyString_FromFormat("%d", obj->number);
}


static PyMethodDef AtomicInt_methods[] = {
    {"increment", (PyCFunction)AtomicInt_increment, METH_NOARGS,
     "Atomically increment an integer"},
    {"increment_by", (PyCFunction)AtomicInt_increment_by, METH_VARARGS,
     "Atomically increment an integer by a value"},
    {"decrement_by", (PyCFunction)AtomicInt_decrement_by, METH_VARARGS,
     "Atomically decrement an integer by a value"},
    {NULL}
};


static PyTypeObject AtomicIntType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "atomic.AtomicInt",             /*tp_name*/
    sizeof(AtomicInt),             /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)AtomicInt_dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    AtomicInt_repr,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    AtomicInt_str,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "Atomic Int objects",           /* tp_doc */
    0,		               /* tp_traverse */
    0,		               /* tp_clear */
    0,		               /* tp_richcompare */
    0,		               /* tp_weaklistoffset */
    0,		               /* tp_iter */
    0,		               /* tp_iternext */
    AtomicInt_methods,             /* tp_methods */
    AtomicInt_members,             /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)AtomicInt_init,      /* tp_init */
    0,                         /* tp_alloc */
    AtomicInt_new,                 /* tp_new */
};


static PyMethodDef module_methods[] = {
    {NULL}
};


PyMODINIT_FUNC
initatomic(void)
{
    PyObject* m = NULL;

    if (PyType_Ready(&AtomicIntType) < 0)
        return;
    m = Py_InitModule3("atomic", module_methods, "why are we here");
    if (m == NULL)
	return;
    
    Py_INCREF(&AtomicIntType);
    PyModule_AddObject(m, "AtomicInt", (PyObject *)&AtomicIntType);
}
