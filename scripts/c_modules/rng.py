from ctypes import c_uint, c_void_p

from ._lib import lib
from ._c_object import _C_Object

lib.rng_new.argtypes = [c_uint]
lib.rng_new.restype = c_void_p

lib.rng_destroy.argtypes = [c_void_p]
lib.rng_destroy.restype = c_void_p

class RandomNumberGenerator(_C_Object):
    """
    A thin wrapper around the C RNG object.
    """
    def __init__(self, seed: int):
        c_obj = lib.rng_new(c_uint(seed))
        super().__init__("rng", c_obj, lib.rng_destroy)