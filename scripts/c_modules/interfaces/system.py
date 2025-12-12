import ctypes
from ctypes import (
    c_void_p,
    c_size_t,
    c_bool,
    c_double,
    POINTER,
)

# --- Function pointer types ---

QUERY_FN = ctypes.CFUNCTYPE(
    c_bool,              # bool return
    c_void_p,            # self
    c_void_p,            # key
    c_size_t,            # len
    POINTER(c_double),   # double *delay
    POINTER(c_bool),     # bool *fp
)

INSERT_FN = ctypes.CFUNCTYPE(
    None,                # void
    c_void_p,            # self
    c_void_p,            # key
    c_size_t,            # len
)

INSERT_ARRAY_FN = ctypes.CFUNCTYPE(
    None,                # void
    c_void_p,            # self
    POINTER(c_void_p),   # void **keys
    POINTER(c_size_t),   # size_t *lens
    c_size_t,            # array_size
)

QUERY_ARRAY_FN = ctypes.CFUNCTYPE(
    None,                # void
    c_void_p,            # self
    POINTER(c_void_p),   # void **keys
    POINTER(c_size_t),   # size_t *lens
    c_size_t,            # array_size
    POINTER(c_bool),     # bool *queries (nullable)
    POINTER(c_double),   # double *delays (nullable)
    POINTER(c_size_t),   # size_t *fp_count (nullable)
)

# --- ctypes Structure ---

class SystemInterface(ctypes.Structure):
    _fields_ = [
        ("query", QUERY_FN),
        ("insert", INSERT_FN),
        ("insert_array", INSERT_ARRAY_FN),
        ("query_array", QUERY_ARRAY_FN),
    ]
