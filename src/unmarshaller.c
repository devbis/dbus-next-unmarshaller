#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "unmarshaller.h"


PyObject *
Unmarshaller_unmarshall(UnmarshallerObject *self, PyObject *Py_UNUSED(ignored))
{
//    if (self->first == NULL) {
//        PyErr_SetString(PyExc_AttributeError, "first");
//        return NULL;
//    }
//    if (self->last == NULL) {
//        PyErr_SetString(PyExc_AttributeError, "last");
//        return NULL;
//    }
    Py_RETURN_NONE;
//    return PyUnicode_FromFormat("%S %S", self->first, self->last);
}

PyObject *read_sock(UnmarshallerObject *self, int length)
{
	if (self->sock == Py_None) {
		PyObject * read_meth = PyObject_GetAttrString(self->stream, "read");
		PyObject * result = PyObject_CallFunction(read_meth, "");
		return result;
	}
	uint16_t unix_fd_list[1024];

	PyObject *socket_mod = PyImport_ImportModule("socket");
	PyObject *recvmsg = PyObject_GetAttrString(self->sock, "recvmsg");
	PyObject *CMSG_LEN = PyObject_GetAttrString(socket_mod, "CMSG_LEN");

	PyObject *cmsg_len_args = Py_BuildValue("(i)", MAX_UNIX_FDS * sizeof(unix_fd_list[0]));
	PyObject *recvmsg_args = Py_BuildValue(
		"(iO)",
		length,
		PyObject_Call(
			CMSG_LEN,
			cmsg_len_args,
			NULL
		)
	);
	PyObject *result = PyObject_Call(
		recvmsg,
		recvmsg_args,
		NULL
	);
	err = PyErr_Occurred();
	if (err != NULL) {
		PyObject *ptype, *pvalue, *ptraceback;
		PyErr_Fetch(&ptype, &pvalue, &ptraceback);
		//		BlockingIOError
	}
	Py_XDECREF(recvmsg);
	Py_XDECREF(CMSG_LEN);

	return result;
}

/*

def read_sock(length):
	'''reads from the socket, storing any fds sent and handling errors
	from the read itself'''
	if self.sock is not None:
		unix_fd_list = array.array("i")

		try:
			msg, ancdata, *_ = self.sock.recvmsg(
				length, socket.CMSG_LEN(MAX_UNIX_FDS * unix_fd_list.itemsize))
		except BlockingIOError:
			raise MarshallerStreamEndError()

		for level, type_, data in ancdata:
			if not (level == socket.SOL_SOCKET and type_ == socket.SCM_RIGHTS):
				continue
			unix_fd_list.frombytes(data[:len(data) - (len(data) % unix_fd_list.itemsize)])
			self.unix_fds.extend(list(unix_fd_list))

		return msg
	else:
		return self.stream.read(length)


*/
