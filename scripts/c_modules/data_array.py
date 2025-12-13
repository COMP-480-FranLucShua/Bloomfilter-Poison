import ctypes
from ctypes import POINTER, c_void_p, c_size_t

class DataArray:
    """
    Docstring for DataArray

    owns: 
    void pointer array for data
    size_t array for data lengths
    """

    __slots__ = (
        "_n",
        "_ptr_array",
        "_len_array",
        "_buffers",
    )

    def __init__(self, n: int):
        self._n = n

        # Fixed-size arrays
        self._ptr_array = (ctypes.c_void_p * n)()
        self._len_array = (ctypes.c_size_t * n)()

        # Hold backing buffers to prevent GC
        self._buffers = [None] * n
    
    def get_strs(self):
        return self._voidpp_to_strings(self._ptr_array, self._len_array, self._n)
    
    def _voidpp_to_strings(
        self,
        ptrs,
        lens,
        n: int,
        encoding: str = "utf-8",
    ):
        """
        Convert (void **, size_t *) → list[str]
        """
        result = []

        for i in range(n):
            ptr = ptrs[i]
            length = lens[i]

            if not ptr:
                result.append(None)
                continue

            # Cast void* → char*
            char_ptr = ctypes.cast(ptr, ctypes.POINTER(ctypes.c_char))

            # Read exactly `length` bytes
            raw = ctypes.string_at(char_ptr, length)

            # Decode to Python string
            result.append(raw.decode(encoding))

        return result
    
    def clear(self):
        """
        Explicitly release buffers.
        """
        for i in range(self._n):
            self._buffers[i] = None
            self._ptr_array[i] = None
            self._len_array[i] = 0


    def __del__(self):
        self.clear()
