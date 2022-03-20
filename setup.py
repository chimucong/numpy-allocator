from Cython.Distutils import build_ext
from distutils.extension import Extension
from distutils.core import setup
import numpy

setup(name='mine', description='Nothing',
      ext_modules=[Extension('py_allocator', ['py_allocator.pyx', 'allocator.c'],
                             include_dirs=[numpy.get_include()])],
      cmdclass={'build_ext': build_ext})
