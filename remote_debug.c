#include <Python.h>
#include "frameobject.h"

static PyObject * install(PyObject *self, PyObject *args);
static PyObject * install_callback(PyObject *self, PyObject *args);
static PyObject * uninstall(PyObject *self, PyObject *args);
static PyObject * current_counter(PyObject *self, PyObject *args);

static PyMethodDef RemoteDebugMethods[] = {
	{"install",  install, METH_VARARGS, "Install remote debugging callbacks"},
	{"install_callback",  install_callback, METH_VARARGS, "Install remote debugging callbacks"},
	{"uninstall",  uninstall, METH_VARARGS, "Uninstall remote debugging callbacks"},
	{"current_counter",  current_counter, METH_VARARGS, "Return number of times the trace function is called"},
	{NULL, NULL, 0, NULL}        /* Sentinel */
};

unsigned long counter = 0;
unsigned long divisor = 1;
static PyObject *my_callback = NULL;


// https://github.com/python/cpython/blob/9907203a7b805630b1fe69e770164449b5d53cff/Python/sysmodule.c#L417-L459
// http://nedbatchelder.com/text/trace-function.html
static int
tracefunc(PyObject *self, PyFrameObject *frame,
                 int what, PyObject *arg)
{
    PyObject *type, *value, *traceback;
    if (what == PyTrace_CALL) {
        counter++;
        if (my_callback != NULL && counter == divisor) {


            PyObject* ret = PyObject_CallObject(my_callback, NULL);
            if (ret == NULL) {
                return -1;
            } else if (ret == Py_None) {
                Py_DECREF(ret);
            }

            counter = 0;
        }
    }
	return 0;
}

static PyObject *
install(PyObject *self, PyObject *args)
{
    PyEval_SetTrace(tracefunc, NULL);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
install_callback(PyObject *self, PyObject *args)
{
    PyObject *result = NULL;
    PyObject *temp;

    if (PyArg_ParseTuple(args, "Ol", &temp, &divisor)) {
        if (!PyCallable_Check(temp)) {
            PyErr_SetString(PyExc_TypeError, "parameter must be callable");
            return NULL;
        }
        Py_XINCREF(temp);         /* Add a reference to new callback */
        Py_XDECREF(my_callback);  /* Dispose of previous callback */
        my_callback = temp;       /* Remember new callback */
    }

    PyEval_SetTrace(tracefunc, NULL);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
uninstall(PyObject *self, PyObject *args)
{
    PyEval_SetTrace(NULL, NULL);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
current_counter(PyObject *self, PyObject *args)
{
    return Py_BuildValue("i", counter);
}

PyMODINIT_FUNC initremote_debug(void)
{
	(void) Py_InitModule("remote_debug", RemoteDebugMethods);
}
