from numpy_allocator import init_mmap_file_allocator, set_file_allocator_directory
import numpy as np
from numpy.core.multiarray import get_handler_name, get_handler_version
import os

arr1 = np.ndarray(shape=(1024, 1024, 1024), dtype=float)
print('handler name:', get_handler_name(arr1),
      ',version:', get_handler_version(arr1))

init_mmap_file_allocator()
set_file_allocator_directory(os.path.expanduser('~'))

arr2 = np.ndarray(shape=(1024, 1024, 1024), dtype=float)
print('handler name:', get_handler_name(arr2),
      ',version:', get_handler_version(arr2))
