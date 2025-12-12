import ctypes
from ctypes import (
    c_void_p,
    c_size_t,
    POINTER,
)

# Function pointer types
SAMPLE_FN = ctypes.CFUNCTYPE(
    c_void_p,                 # void*
    c_void_p,                 # self
    POINTER(c_size_t)         # size_t*
)

SAMPLE_ARRAY_FN = ctypes.CFUNCTYPE(
    None,                     # void
    c_void_p,                 # self
    POINTER(c_void_p),        # void** samples
    POINTER(c_size_t),        # size_t* sample_lens
    c_size_t                  # array_size
)

LENGTH_FN = ctypes.CFUNCTYPE(
    c_size_t,                 # size_t
    c_void_p                  # self
)

class SamplerInterface(ctypes.Structure):
    _fields_ = [
        ("sample", SAMPLE_FN),
        ("sample_array", SAMPLE_ARRAY_FN),
        ("length", LENGTH_FN),
    ]