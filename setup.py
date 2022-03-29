from distutils.extension import Extension
from distutils.core import setup
import numpy

USE_CYTHON = False
try:
    import Cython
    USE_CYTHON = True
except:
    pass

ext = '.pyx' if USE_CYTHON else '.cpp'

extensions = [
    Extension(
        'numpy_allocator.allocator_ext',
        ['numpy_allocator/py_allocator'+ext, 'numpy_allocator/allocator.c'],
        include_dirs=[numpy.get_include(), 'numpy_allocator'],
        libraries=['tcmalloc_minimal_non_override', 'logging',
                   'sysinfo', 'maybe_threads', 'spinlock', 'stdc++'],
        library_dirs=['numpy_allocator/libs'],
    )
]

if USE_CYTHON:
    from Cython.Build import cythonize
    extensions = cythonize(extensions,
    compiler_directives={'language_level': "3"})

print('extensions:', extensions)
setup(
    name = 'numpy_allocator',
    version = '0.0.1',
    description = 'Numpy allocator',
    ext_modules = extensions,
    packages = ['numpy_allocator'],
    package_data = {'': ['**/*.so', '**/*.a', '**/*.h', '**/*.c']},
    zip_safe = False,
)
