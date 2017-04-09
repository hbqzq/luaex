/** 
 * Copyright (c) 2014 rxi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */

#ifndef MAP_H
#define MAP_H

#include <string.h>

#define MAP_VERSION "0.1.0"

struct lmap_node_t;
typedef struct lmap_node_t lmap_node_t;

typedef struct {
  lmap_node_t **buckets;
  unsigned nbuckets, nnodes;
} lmap_base_t;

typedef struct {
  unsigned bucketidx;
  lmap_node_t *node;
} lmap_iter_t;


#define lmap_t(T)\
  struct { lmap_base_t base; T *ref; T tmp; }


#define lmap_init(m)\
  memset(m, 0, sizeof(*(m)))


#define lmap_deinit(m)\
  lmap_deinit_(&(m)->base)


#define lmap_get(m, key)\
  ( (m)->ref = lmap_get_(&(m)->base, key) )


#define lmap_set(m, key, value)\
  ( (m)->tmp = (value),\
    lmap_set_(&(m)->base, key, &(m)->tmp, sizeof((m)->tmp)) )


#define lmap_remove(m, key)\
  lmap_remove_(&(m)->base, key)


#define lmap_iter(m)\
  lmap_iter_()


#define lmap_next(m, iter)\
  lmap_next_(&(m)->base, iter)


void lmap_deinit_(lmap_base_t *m);
void *lmap_get_(lmap_base_t *m, const char *key);
int lmap_set_(lmap_base_t *m, const char *key, void *value, int vsize);
void lmap_remove_(lmap_base_t *m, const char *key);
lmap_iter_t lmap_iter_(void);
const char *lmap_next_(lmap_base_t *m, lmap_iter_t *iter);


typedef lmap_t(void*) lmap_void_t;
typedef lmap_t(char*) lmap_str_t;
typedef lmap_t(int) lmap_int_t;
typedef lmap_t(char) lmap_char_t;
typedef lmap_t(float) lmap_float_t;
typedef lmap_t(double) lmap_double_t;

#endif
