
from cpython.ref cimport PyObject
cdef extern from "allocator.h":
    cdef PyObject *set_secret_data_policy()