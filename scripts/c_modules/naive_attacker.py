import ctypes
from ctypes import c_void_p, POINTER, c_size_t

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

lib.naive_attacker_create.argtypes = [POINTER(SystemInterface),
                                      c_void_p,
                                      POINTER(SamplerInterface),
                                      c_void_p]
lib.naive_attacker_create.restype = c_void_p

lib.naive_attacker_destroy.argtypes = [c_void_p]
lib.naive_attacker_destroy.restype = None

class NaiveAttacker(_C_Object):
    def __init__(self, sys: System, sampler: Sampler):
        assert isinstance(sys, System)
        assert isinstance(sampler, Sampler)

        self.sampler = sampler

        c_obj = lib.naive_attacker_create(sys._interface, sys._c_obj,
                                          sampler._interface, sampler._c_obj)
        
        super().__init__("naive-attacker", c_obj, lib.naive_attacker_destroy)
        self._interface = ctypes.pointer(AttackerInterface.in_dll(lib, "naive_attacker_interface"))
    
    def attack(self, attack_size: int):
        print("attacking")
        self._interface.contents.attack(self._c_obj, c_size_t(attack_size))
        print("attacked")