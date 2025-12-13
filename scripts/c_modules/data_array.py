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
    
    @staticmethod
    def from_array(strings):
        # Encode strings to bytes
        encoded = [s.encode("utf-8") for s in strings]

        # Allocate C-owned buffers (important!)
        buffers = [
            ctypes.create_string_buffer(b) for b in encoded
        ]

        n = len(buffers)

        # Allocate fixed-size arrays
        ptr_array = (ctypes.c_void_p * n)()
        len_array = (ctypes.c_size_t * n)()

        # Populate arrays
        for i, buf in enumerate(buffers):
            ptr_array[i] = ctypes.cast(buf, ctypes.c_void_p)
            len_array[i] = len(buf.value)

        data_array = DataArray(n)
        data_array._ptr_array = ptr_array
        data_array._len_array = len_array

        return data_array
        

    
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
