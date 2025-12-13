import ctypes
from ctypes import c_void_p, POINTER, c_char_p, c_size_t

from .str_array import StringArray
from .interfaces import GeneratorInterface, SamplerInterface

from typing import Protocol, runtime_checkable

@runtime_checkable
class Generator(Protocol):
    _interface: GeneratorInterface
    _c_obj: c_void_p

from ._lib import lib
from ._c_object import _C_Object

from .data_array import DataArray

lib.str_sampler_create.argtypes = [POINTER(c_char_p),
                                   c_size_t,
                                   POINTER(GeneratorInterface),
                                   c_void_p]
lib.str_sampler_create.restype = c_void_p

lib.str_sampler_destroy.argtypes = [c_void_p]
lib.str_sampler_destroy.restype = None

class StringSampler(_C_Object):
    def __init__(self, strings: StringArray, rng: Generator):
        assert isinstance(rng, Generator)

        c_obj = lib.str_sampler_create(strings.c_strings, 
                                       c_size_t(len(strings)),
                                       rng._interface,
                                       rng._c_obj)
        
        super().__init__("string-sampler", c_obj, lib.str_sampler_destroy)
        self._interface = ctypes.pointer(SamplerInterface.in_dll(lib, "str_sampler_interface"))
    
    def sample_array(self, sample_size: int):
        data_array = DataArray(sample_size)

        self._interface.contents.sample_array(self._c_obj, data_array._ptr_array, data_array._len_array, c_size_t(sample_size))

        return data_array

