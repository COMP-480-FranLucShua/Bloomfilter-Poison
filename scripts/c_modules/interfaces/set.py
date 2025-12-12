import ctypes
from ctypes import (
    c_void_p,
    c_size_t,
    c_bool,
)

# Function pointer types
QUERY_FN = ctypes.CFUNCTYPE(
    c_bool,     # bool
    c_void_p,   # self
    c_void_p,   # key
    c_size_t    # len
)

INSERT_FN = ctypes.CFUNCTYPE(
    None,       # void
    c_void_p,   # self
    c_void_p,   # key
    c_size_t    # len
)

REMOVE_FN = ctypes.CFUNCTYPE(
    None,       # void
    c_void_p,   # self
    c_void_p,   # key
    c_size_t    # len
)

LENGTH_FN = ctypes.CFUNCTYPE(
    c_size_t,   # size_t
    c_void_p    # self
)

class SetInterface(ctypes.Structure):
    _fields_ = [
        ("query", QUERY_FN),
        ("insert", INSERT_FN),
        ("remove", REMOVE_FN),
        ("length", LENGTH_FN),
    ]