from ctypes import c_void_p

class _C_Object:
    """
    C Object superclass
    """
    def __init__(self, name: str, c_obj: c_void_p, destructor):
        self._name = name
        self._c_obj = c_obj
        self._destructor = destructor
        self._closed = False
    
    def close(self):
        if not self._closed:
            if self._destructor(self._c_obj) != None:
                raise RuntimeError(f"Failed to destroy {self._name}")
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