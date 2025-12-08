"""
mytypes.py
Centralized numeric type aliases for Python + Cython.

Python:
    i32, f64, etc. are normal built-in types.
Cython (compiled):
    they become fast C types.
"""

# ----------------------
# Default Python types
# ----------------------
i8  = int
i16 = int
i32 = int
i64 = int

u8  = int
u16 = int
u32 = int
u64 = int

f32 = float
f64 = float

b1  = bool          # single-bit C boolean

# Optional: typed memoryviews fallback
f32_view = list
f64_view = list
i32_view = list
i64_view = list

# ----------------------
# Try to upgrade to Cython types
# ----------------------
try:
    import cython
    if cython.compiled:
        # signed integers
        i8  = cython.char
        i16 = cython.short
        i32 = cython.int
        i64 = cython.longlong

        # unsigned integers
        u8  = cython.uchar
        u16 = cython.ushort
        u32 = cython.uint
        u64 = cython.ulonglong

        # floats
        f32 = cython.float
        f64 = cython.double

        # boolean
        b1 = cython.bint

        # memoryviews (typed array-like)
        f32_view = cython.float[:]
        f64_view = cython.double[:]
        i32_view = cython.int[:]
        i64_view = cython.longlong[:]

except ImportError:
    pass
