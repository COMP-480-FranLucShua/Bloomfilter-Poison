from mmh3 import hash
import cython
from cython.cimports.libc.stdlib import malloc, free


@cython.cclass
class BloomFilter:
    num_hashes: cython.uint
    num_buckets: cython.uint
    filter: cython.p_uchar

    def __init__(self, num_hashes: cython.uint, num_buckets: cython.uint):
        self.num_hashes: cython.uint = num_hashes
        self.num_buckets: cython.uint = num_buckets

        self.filter: cython.p_uchar = cython.cast(
            cython.p_uchar, 
            malloc(self.num_buckets * cython.sizeof(cython.uchar))
        )

    @cython.ccall
    def insert(self, key: bytes):
        hash_idx: cython.uint = 0
        unbounded_bucket: cython.int
        bounded_bucket: cython.uint

        while hash_idx < self.num_hashes:
            unbounded_bucket = hash(key, hash_idx)
            bounded_bucket = unbounded_bucket % self.num_buckets
            self.filter[bounded_bucket] = 1
            hash_idx += 1

    @cython.ccall
    def query(self, key: bytes) -> cython.bint:
        hash_idx: cython.uint = 0
        unbounded_bucket: cython.int
        bounded_bucket: cython.uint

        while hash_idx < self.num_hashes:
            unbounded_bucket = hash(key, hash_idx)
            bounded_bucket = unbounded_bucket % self.num_buckets
            hash_idx += 1
            if self.filter[bounded_bucket] == 0:
                return False
        return True

