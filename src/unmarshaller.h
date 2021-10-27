#define MAX_UNIX_FDS 16

typedef struct {
    PyObject_HEAD
    /* Type-specific fields go here. */
    PyObject *unix_fds;
    PyObject *buf;
//    PyObject *offset;
    PyObject *stream;
    PyObject *sock;
    PyObject *endian;
    PyObject *message;
    int offset;
} UnmarshallerObject;


PyObject *
Unmarshaller_unmarshall(UnmarshallerObject *self, PyObject *Py_UNUSED(ignored));

PyObject *read_sock(UnmarshallerObject *self, int length);
