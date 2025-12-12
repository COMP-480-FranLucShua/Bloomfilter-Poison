#ifndef SET_H
#define SET_H

typedef struct {
    bool (*query)(void *self, void *key, size_t len);
    void (*insert)(void *self, void *key, size_t len);
    void (*remove)(void *self, void *key, size_t len);
    size_t (*length)(void *self);
} Set;

#endif