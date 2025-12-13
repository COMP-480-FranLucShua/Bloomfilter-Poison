import ctypes
from ctypes import c_char_p, c_void_p, c_size_t


class StringArray:
    """
    Python wrapper representing an array of C strings with:
      - char **      (c_strings)
      - void **      (c_ptrs)
      - size_t *     (c_lens)

    Ownership semantics:
      - The StringArray always owns the ARRAYS
      - It may or may not own the underlying string memory
    """

    __slots__ = (
        "_size",
        "_buffers",     # owned buffers (or None)
        "c_strings",    # char **
        "c_ptrs",       # void **
        "c_lens",       # size_t *
        "_owner",
    )

    # ------------------------------------------------------------
    # 1. Initialize from Python array-like of strings (owns data)
    # ------------------------------------------------------------
    def __init__(self, strings=None, *, size=None):
        """
        Either:
          - strings: iterable[str]  → owns string memory
          - size=n                 → empty array, no ownership of data
        """

        if strings is not None and size is not None:
            raise ValueError("Specify either strings or size, not both")
        
        self._owner = None

        # ---------- Case 1: from Python strings ----------
        if strings is not None:
            encoded = [s.encode("utf-8") for s in strings]
            self._size = len(encoded)

            # Own the string memory
            self._buffers = [
                ctypes.create_string_buffer(b) for b in encoded
            ]

            self._allocate_arrays()

            for i, buf in enumerate(self._buffers):
                char_ptr = ctypes.cast(buf, c_char_p)
                void_ptr = ctypes.cast(buf, c_void_p)

                self.c_strings[i] = char_ptr
                self.c_ptrs[i] = void_ptr
                self.c_lens[i] = len(buf.value)

        # ---------- Case 3: empty array of size n ----------
        else:
            if size is None:
                raise ValueError("Must provide strings or size")

            self._size = int(size)
            self._buffers = None  # does NOT own data

            self._allocate_arrays()

            # initialize to NULL / 0
            for i in range(self._size):
                self.c_strings[i] = None
                self.c_ptrs[i] = None
                self.c_lens[i] = 0

    # ------------------------------------------------------------
    # Internal helper
    # ------------------------------------------------------------
    def _allocate_arrays(self):
        """Allocate the fixed-size C arrays."""
        self.c_strings = (c_char_p * self._size)()
        self.c_ptrs = (c_void_p * self._size)()
        self.c_lens = (c_size_t * self._size)()

    # ------------------------------------------------------------
    # 2. Slicing support (shares underlying string memory)
    # ------------------------------------------------------------
    def __getitem__(self, idx):
        if isinstance(idx, slice):
            indices = range(*idx.indices(self._size))
            return self._slice(indices)
        elif isinstance(idx, int):
            if idx < 0:
                idx += self._size
            if not (0 <= idx < self._size):
                raise IndexError
            return self._slice([idx])
        else:
            raise TypeError("Invalid index type")

    def _slice(self, indices):
        """
        New StringArray:
          - owns its ARRAYS
          - does NOT own string memory
          - pointers alias original memory
        """
        new = StringArray(size=len(indices))
        new._buffers = None  # explicitly does NOT own memory
        new._owner = self

        for i, src_idx in enumerate(indices):
            new.c_strings[i] = self.c_strings[src_idx]
            new.c_ptrs[i] = self.c_ptrs[src_idx]
            new.c_lens[i] = self.c_lens[src_idx]

        return new

    # ------------------------------------------------------------
    # Utility
    # ------------------------------------------------------------
    def __len__(self):
        return self._size

    def to_python(self):
        """Convert back to Python strings (safe, read-only)."""
        out = []
        for i in range(self._size):
            if not self.c_ptrs[i]:
                out.append(None)
            else:
                raw = ctypes.string_at(self.c_ptrs[i], self.c_lens[i])
                out.append(raw.decode("utf-8"))
        return out
