import ctypes
from ctypes import c_void_p, c_double, POINTER

from typing import Protocol, runtime_checkable

from .interfaces import SystemInterface, GeneratorInterface, FilterInterface, SetInterface

from .data_array import DataArray

from ._lib import lib
from ._c_object import _C_Object

@runtime_checkable
class Generator(Protocol):
    _interface: GeneratorInterface
    _c_obj: c_void_p

@runtime_checkable
class Filter(Protocol):
    _interface: FilterInterface
    _c_obj: c_void_p

@runtime_checkable
class Set(Protocol):
    _interface: SetInterface
    _c_obj: c_void_p

lib.sys_new.argtypes = [POINTER(SetInterface), c_void_p,
                        POINTER(FilterInterface), c_void_p,
                        POINTER(GeneratorInterface), c_void_p,
                        c_double]
lib.sys_new.restype = c_void_p

lib.sys_destroy.argtypes = [c_void_p]
lib.sys_destroy.restype = c_void_p


class SystemEmulator(_C_Object):
    def __init__(self, set: Set, filter: Filter, rng: Generator, delay: float):
        assert isinstance(set, Set)
        assert isinstance(filter, Filter)
        assert isinstance(rng, Generator)

        c_obj = lib.sys_new(set._interface, set._c_obj,
                            filter._interface, filter._c_obj,
                            rng._interface, rng._c_obj,
                            c_double(delay))
        
        super().__init__("system-emulator", c_obj, lib.sys_destroy)
        self._interface = ctypes.byref(SystemInterface.in_dll(lib, "system_emulator_interface"))
    
    def query_array(self, array: DataArray):
        """
        Docstring for query_array
        
        :param self: Description
        :param keys: keys is an array of strings
        """
        pass

    def insert_array(self, array: DataArray):
        """
        Docstring for insert_array
        
        :param self: Description
        :param keys: an array of strings
        """
        pass