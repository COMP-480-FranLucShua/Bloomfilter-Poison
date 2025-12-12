import ctypes
from ctypes import c_void_p, c_uint32, c_uint8, c_size_t, POINTER

from .lib import lib
from ._c_object import _C_Object

HASH_FUNCTION = ctypes.CFUNCTYPE(c_uint32, POINTER(c_uint8), c_size_t, c_uint32)

lib.hset_new.argtypes = [c_size_t, c_uint32, HASH_FUNCTION]
lib.hset_new.restype = c_void_p

lib.hset_destroy.argtypes [c_void_p]
lib.hset_destroy.restype = c_void_p

class HashSet(_C_Object):
    """
    A thin wrapper around the C HashSet object.
    """
    def __init__(self, capacity: int, seed: int):
        pass