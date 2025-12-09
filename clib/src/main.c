#include <stdio.h>
#include "hash_set.h"
#include "murmur_hash.h"

int main(int , char **) {
    HashSet *hset = hset_new(1, 0, murmur3_32);
    hset_dump_table(hset);

    // hset_print_int(hset);

    int item = 0;
    int item2 = 1;

    // int item2_dup = 1;

    int item3 = 3;
    int item4 = 4;

    hset_insert(hset, &item, 4);
    hset_insert(hset, &item2, 4);

    hset_dump_table(hset);
    hset_print_int(hset);

    printf("%d, %d, %d, %d\n", 
        hset_lookup(hset, &item, 4),
        hset_lookup(hset, &item2, 4),
        hset_lookup(hset, &item3, 4),
        hset_lookup(hset, &item4, 4)
    );

    hset_destroy(hset);
    return 0;
}