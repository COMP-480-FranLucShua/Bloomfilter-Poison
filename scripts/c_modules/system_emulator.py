import ctypes
from ctypes import c_void_p, c_double

lib = ctypes.CDLL("./clib.so")

# lib.sys_new.argtypes = [c_void_p, c_]