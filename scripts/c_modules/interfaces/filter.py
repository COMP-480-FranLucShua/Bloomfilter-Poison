import ctypes
from ctypes import c_void_p, c_size_t, c_bool

INSERT_FN = ctypes.CFUNCTYPE(None, c_void_p, c_void_p, c_size_t)
QUERY_FN = ctypes.CFUNCTYPE(c_bool, c_void_p, c_void_p, c_size_t)
GET_NUM_HF_FN = ctypes.CFUNCTYPE(c_size_t, c_void_p)
GET_FILTER_FN = ctypes.CFUNCTYPE(c_void_p, c_void_p)

class FilterInterface(ctypes.Structure):
    _fields_ = [
        ("insert", INSERT_FN),
        ("query", QUERY_FN),
        ("get_num_hf", GET_NUM_HF_FN),
        ("get_filter", GET_FILTER_FN)
    ]