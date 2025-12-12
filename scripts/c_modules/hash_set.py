from ctypes import c_void_p

from .lib import lib
from ._c_object import _C_Object


class HashSet(_C_Object):
    """
    A thin wrapper around the C HashSet object.
    """
    def __init__(self, capacity: int, seed: int)