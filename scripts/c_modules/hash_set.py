import ctypes
from ctypes import c_void_p, c_uint32, c_uint8, c_size_t, POINTER

from ._lib import lib
from ._c_object import _C_Object

from .interfaces import SetInterface

HASH_FUNCTION = ctypes.CFUNCTYPE(c_uint32, POINTER(c_uint8), c_size_t, c_uint32)

lib.hset_new.argtypes = [c_size_t, c_uint32, HASH_FUNCTION]
lib.hset_new.restype = c_void_p

lib.hset_destroy.argtypes = [c_void_p]
lib.hset_destroy.restype = c_void_p

lib.murmur3_32.argtypes = [POINTER(c_uint8), c_size_t, c_uint32]
lib.murmur3_32.restype = c_uint32

class HashSet(_C_Object):
    """
    A thin wrapper around the C HashSet object.
    """
    def __init__(self, capacity: int, seed: int):
        hash_func = HASH_FUNCTION(("murmur3_32", lib))

        c_obj = lib.hset_new(c_size_t(capacity), c_uint32(seed), hash_func)
        super().__init__("hash-set", c_obj, lib.hset_destroy)
        self._interface = ctypes.byref(SetInterface.in_dll(lib, "hash_set_interface"))