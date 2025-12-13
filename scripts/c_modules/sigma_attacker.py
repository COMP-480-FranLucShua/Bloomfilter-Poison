import ctypes
from ctypes import c_void_p, POINTER, c_size_t, c_double

from ._lib import lib
from ._c_object import _C_Object

from .interfaces import SystemInterface, AttackerInterface, FilterInterface

from typing import Protocol, runtime_checkable

@runtime_checkable
class System(Protocol):
    _interface: SystemInterface
    _c_obj: c_void_p

@runtime_checkable
class Filter(Protocol):
    _interface: FilterInterface
    _c_obj: c_void_p

lib.sigma_attacker_create.argtypes = [POINTER(SystemInterface),
                                      c_void_p,
                                      POINTER(FilterInterface),
                                      c_void_p]
lib.sigma_attacker_create.restype = c_void_p

lib.sigma_attacker_destroy.argtypes = [c_void_p]
lib.sigma_attacker_destroy.restype = None

class TimingAttacker(_C_Object):
    def __init__(self, sys: System, filter: Filter, training_proportion: float):
        assert isinstance(sys, System)
        assert isinstance(filter, Filter)

        c_obj = lib.sigma_attacker_create(sys._interface, sys._c_obj,
                                          filter._interface, filter._c_obj,
                                          c_double(training_proportion))
        
        super().__init__("sigma-attacker", c_obj, lib.sigma_attacker_destroy)
        self._interface = ctypes.pointer(AttackerInterface.in_dll(lib, "sigma_attacker_interface"))
    
    def attack(self, attack_size: int):
        self._interface.contents.attack(self._c_obj, c_size_t(attack_size))