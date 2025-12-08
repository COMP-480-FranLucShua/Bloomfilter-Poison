from setuptools import setup
from Cython.Build import cythonize

setup(
    package_dir={"": "src"},
    packages=["bloom_filter"],

    ext_modules=cythonize(
        "src/bloom_filter/bloom_filter.py",
        annotate=True,
    ),
)
