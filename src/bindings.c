#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"

#include "unmarshaller.h"


static void
Unmarshaller_dealloc(UnmarshallerObject *self)
{
    Py_XDECREF(self->unix_fds);
    Py_XDECREF(self->buf);
//    Py_XDECREF(self->offset);
    Py_XDECREF(self->stream);
    Py_XDECREF(self->sock);
    Py_XDECREF(self->endian);
    Py_XDECREF(self->message);
    Py_TYPE(self)->tp_free((PyObject *) self);
}


static PyObject *
Unmarshaller_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    UnmarshallerObject *self;
    self = (UnmarshallerObject *) type->tp_alloc(type, 0);
    if (self != NULL) {
    	self->unix_fds = PyList_New(0);
        if (self->unix_fds == NULL) {
            Py_DECREF(self);
            return NULL;
        }

//		self->offset = PyLong_FromLong(0);
//        if (self->offset == NULL) {
//            Py_DECREF(self);
//            return NULL;
//        }


//        self->first = PyUnicode_FromString("");
//        if (self->first == NULL) {
//            Py_DECREF(self);
//            return NULL;
//        }
//        self->last = PyUnicode_FromString("");
//        if (self->last == NULL) {
//            Py_DECREF(self);
//            return NULL;
//        }
        self->offset = 0;
    }
    return (PyObject *) self;
}


static int
Unmarshaller_init(UnmarshallerObject *self, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"stream", "sock", NULL};
    PyObject *stream = NULL, *sock = NULL, *tmp;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|OO", kwlist,
                                     &stream, &sock))
        return -1;

    if (stream) {
        tmp = self->stream;
        Py_INCREF(stream);
        self->stream = stream;
        Py_XDECREF(tmp);
    }
    if (sock) {
        tmp = self->sock;
        Py_INCREF(sock);
        self->sock = sock;
        Py_XDECREF(tmp);
    }
    return 0;
}


static struct PyMemberDef Unmarshaller_members[] = {
    {"unix_fds", T_OBJECT_EX, offsetof(UnmarshallerObject, unix_fds), 0},
    {"sock", T_OBJECT_EX, offsetof(UnmarshallerObject, sock), 0, NULL},
    {"offset", T_INT, offsetof(UnmarshallerObject, offset), 0, NULL},
    {NULL}  /* Sentinel */
};


//static PyObject *
//Unmarshaller_unmarshall(UnmarshallerObject *self, PyObject *Py_UNUSED(ignored))
//{
////    if (self->first == NULL) {
////        PyErr_SetString(PyExc_AttributeError, "first");
////        return NULL;
////    }
////    if (self->last == NULL) {
////        PyErr_SetString(PyExc_AttributeError, "last");
////        return NULL;
////    }
//    Py_RETURN_NONE;
////    return PyUnicode_FromFormat("%S %S", self->first, self->last);
//}
//


static PyMethodDef Unmarshaller_methods[] = {
    {"unmarshall", (PyCFunction) Unmarshaller_unmarshall, METH_NOARGS,
     "Unmarshall incoming values"
    },
    {"read_sock", (PyCFunction) read_sock, METH_NOARGS, NULL},
    {NULL}  /* Sentinel */
};

static PyTypeObject UnmarshallerType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "dbus_next_unmarshaller.Unmarshaller",
    .tp_doc = "Unmarshaller class",
    .tp_basicsize = sizeof(UnmarshallerObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = Unmarshaller_new, //PyType_GenericNew
    .tp_init = (initproc) Unmarshaller_init,
    .tp_dealloc = (destructor) Unmarshaller_dealloc,
    .tp_members = Unmarshaller_members,
    .tp_methods = Unmarshaller_methods,
};


PyModuleDef dbus_next_unmarshaller_mod = {
    PyModuleDef_HEAD_INIT,
    .m_name = "dbus_next_unmarshaller",
    .m_doc = "This is faster unmarshaller module for dbus-next.",
    .m_size = -1,
//	dbus_next_unmarshaller_funcs,
//	NULL,
//	NULL,
//	NULL,
//	NULL
};

PyMODINIT_FUNC PyInit_dbus_next_unmarshaller(void) {
    PyObject *m;
    if (PyType_Ready(&UnmarshallerType) < 0)
        return NULL;

    m = PyModule_Create(&dbus_next_unmarshaller_mod);
    if (m == NULL)
        return NULL;

    Py_INCREF(&UnmarshallerType);
    if (PyModule_AddObject(m, "Unmarshaller", (PyObject *) &UnmarshallerType) < 0) {
        Py_DECREF(&UnmarshallerType);
        Py_DECREF(m);
        return NULL;
    }

	return m;
}
