from cpython.ref cimport PyObject
cdef extern from "allocator.h":
    cdef PyObject *init_mmap_file_allocator()
    cdef void set_file_allocator_directory(const char *);