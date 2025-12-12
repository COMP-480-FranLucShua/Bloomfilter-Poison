from ctypes import c_uint, c_void_p

from ._lib import lib

lib.rng_new.argtypes = [c_uint]
lib.rng_new.restype = c_void_p

lib.rng_destroy.argtypes = [c_void_p]
lib.rng_destroy.restype = c_void_p

class RandomNumberGenerator:
    """
    A thin wrapper around the C RNG object.
    """
    def __init__(self, seed: int):
        self._c_obj = lib.rng_new(c_uint(seed))
        self._closed = False

    def close(self):
        if not self._closed:
            if lib.rng_destroy(self._c_obj) != None:
                print("Failed to destroy RNG")
            self._c_obj = None
            self._closed = True

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc, tb):
        self.close()

    def __del__(self):
        # Last-resort safety net ONLY
        try:
            self.close()
        except Exception:
            pass