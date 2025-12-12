from ctypes import c_void_p, c_size_t

from ._lib import lib
from ._c_object import _C_Object

from .rng import RandomNumberGenerator

lib.bfilter_new.argtypes = [c_size_t, c_size_t, c_void_p]
lib.bfilter_new.restype = c_void_p

lib.bfilter_destroy.argtypes = [c_void_p]
lib.bfilter_destroy.restype = c_void_p

class BloomFilter(_C_Object):
    """
    A thin wrapper around the C RNG object.
    """
    def __init__(self, size: int, k: int, rng: RandomNumberGenerator):
        c_obj = lib.bfilter_new(c_size_t(size), c_size_t(k), rng._c_obj)
        super().__init__("bloom-filter", c_obj, lib.bfilter_destroy)