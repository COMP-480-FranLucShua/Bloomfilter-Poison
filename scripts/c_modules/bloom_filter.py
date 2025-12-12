import ctypes
from ctypes import c_void_p, c_size_t

from typing import Protocol, runtime_checkable

from ._lib import lib
from ._c_object import _C_Object

from .interfaces import GeneratorInterface, FilterInterface

lib.bfilter_new.argtypes = [c_size_t, c_size_t, c_void_p]
lib.bfilter_new.restype = c_void_p

lib.bfilter_destroy.argtypes = [c_void_p]
lib.bfilter_destroy.restype = c_void_p

@runtime_checkable
class Generator(Protocol):
    _interface: GeneratorInterface
    _c_obj: c_void_p

class BloomFilter(_C_Object):
    """
    A thin wrapper around the C bloom filter object.
    """
    def __init__(self, size: int, k: int, rng: Generator):
        assert isinstance(rng, Generator)

        c_obj = lib.bfilter_new(c_size_t(size), c_size_t(k), rng._interface, rng._c_obj)
        super().__init__("bloom-filter", c_obj, lib.bfilter_destroy)
        self._interface = ctypes.byref(FilterInterface.in_dll(lib, "bloom_filter_interface"))