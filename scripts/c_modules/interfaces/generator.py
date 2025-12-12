import ctypes
from ctypes import (
    c_void_p,
    c_uint32,
    c_size_t,
    c_double,
    POINTER,
)

# Function pointer types
GEN_INTEGER_FN = ctypes.CFUNCTYPE(
    c_uint32,
    c_void_p
)

GEN_INTEGERS_FN = ctypes.CFUNCTYPE(
    POINTER(c_uint32),
    c_void_p,
    POINTER(c_uint32),
    c_size_t
)

GEN_DOUBLE_FN = ctypes.CFUNCTYPE(
    c_double,
    c_void_p
)

GEN_CHOICE_FN = ctypes.CFUNCTYPE(
    POINTER(c_uint32),
    c_void_p,
    POINTER(c_uint32),
    c_size_t,
    c_size_t
)

class GeneratorInterface(ctypes.Structure):
    _fields_ = [
        ("gen_integer", GEN_INTEGER_FN),
        ("gen_integers", GEN_INTEGERS_FN),
        ("gen_integers_unique", GEN_INTEGERS_FN),
        ("gen_double", GEN_DOUBLE_FN),
        ("gen_distribution", GEN_DOUBLE_FN),
        ("gen_choice", GEN_CHOICE_FN),
    ]