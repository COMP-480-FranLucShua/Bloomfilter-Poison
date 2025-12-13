import ctypes
import numpy as np


class StringArray:
    """
    Docstring for Dataset

    wrapper for the underlying C types for the string dataset
    """

    def __init__(self, strings):
        """
        Docstring for __init__
        
        :param self: Description
        :param keys: array of strings
        """
        self.encoded = [s.encode("utf-8") for s in strings]
        self.c_strings = (ctypes.c_char_p * len(self.encoded))(*self.encoded)

    def __len__(self):
        return len(self.encoded)
