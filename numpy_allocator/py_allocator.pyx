# distutils: language = c++

from numpy_allocator.allocator cimport init_mmap_file_allocator as c_init_mmap_file_allocator
def init_mmap_file_allocator():
    return <object> c_init_mmap_file_allocator()

from numpy_allocator.allocator cimport set_file_allocator_directory as c_set_file_allocator_directory
from libcpp.string cimport string

def set_file_allocator_directory(path):
    if isinstance(path, str):
        path = path.encode('utf-8')
    cdef string s = path
    c_set_file_allocator_directory(s.c_str())
