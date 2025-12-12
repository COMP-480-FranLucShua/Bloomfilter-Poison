import ctypes
from ctypes import c_void_p, c_size_t

from .._c_object import _C_Object

ATTACK_FN = ctypes.CFUNCTYPE(None, c_void_p, c_size_t)

class AttackerInterface(ctypes.Structure):
    _fields_ = [
        ("attack", ATTACK_FN)
    ]