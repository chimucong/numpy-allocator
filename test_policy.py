import py_allocator
import numpy as np
py_allocator.set_my_data_policy()


get_handler_name = np.core.multiarray.get_handler_name
get_handler_version = np.core.multiarray.get_handler_version
orig_policy_name = get_handler_name()

arr = np.ndarray(shape=(2, 2), dtype=float)

print(get_handler_name(arr))
print(get_handler_version(arr))
