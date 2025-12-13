import ctypes
from ctypes import c_void_p, POINTER, c_size_t, c_double

from ._lib import lib
from ._c_object import _C_Object

from .interfaces import SystemInterface, AttackerInterface, SamplerInterface

from typing import Protocol, runtime_checkable

@runtime_checkable
class System(Protocol):
    _interface: SystemInterface
    _c_obj: c_void_p

@runtime_checkable
class Sampler(Protocol):
    _interface: SamplerInterface
    _c_obj: c_void_p

lib.timing_attacker_create.argtypes = [POINTER(SystemInterface),
                                      c_void_p,
                                      POINTER(SamplerInterface),
                                      c_void_p,
                                      c_double]
lib.timing_attacker_create.restype = c_void_p

lib.timing_attacker_destroy.argtypes = [c_void_p]
lib.timing_attacker_destroy.restype = None

class TimingAttacker(_C_Object):
    def __init__(self, sys: System, sampler: Sampler, training_proportion: float):
        assert isinstance(sys, System)
        assert isinstance(sampler, Sampler)

        c_obj = lib.timing_attacker_create(sys._interface, sys._c_obj,
                                          sampler._interface, sampler._c_obj,
                                          c_double(training_proportion))
        
        super().__init__("timing-attacker", c_obj, lib.timing_attacker_destroy)
        self._interface = ctypes.pointer(AttackerInterface.in_dll(lib, "timing_attacker_interface"))
    
    def attack(self, attack_size: int):
        self._interface.contents.attack(self._c_obj, c_size_t(attack_size))