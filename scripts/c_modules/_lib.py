from pathlib import Path
import ctypes

lib_path = Path(__file__).resolve().parent / "clib.so"
lib = ctypes.CDLL(lib_path)