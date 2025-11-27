#ifndef AIV_VECTOR_H
#define AIV_VECTOR_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define guard(index, count)                                                    \
  if (index >= count) {                                                        \
    return NULL;                                                               \
  }

typedef struct aiv_vector {
    void** items;
    size_t count;
    size_t capacity;
} aiv_vector_t;

static aiv_vector_t aiv_vector_new() {
  aiv_vector_t vector;
  vector.count = 0;
  vector.capacity = 10;
  vector.items = malloc(sizeof(void *) * vector.capacity);
  return vector;
}

static void aiv_vector_destroy(aiv_vector_t *vector) {
  if (vector != NULL) {
    free(vector->items);
  }
  vector->items = NULL;
  vector->count = 0;
  vector->capacity = 0;
}

static void __aiv_vector_resize(aiv_vector_t *vector) {
  if (vector->count == vector->capacity) {
    vector->capacity *= 2;
  } else if (vector->count < vector->capacity / 2 && vector->capacity != 10) {
    vector->capacity /= 2;
  } else {
    return;
  }

  vector->items = realloc(vector->items, sizeof(void *) * vector->capacity);
}

static void aiv_vector_add(aiv_vector_t *vector, void *item) {
  __aiv_vector_resize(vector);
  size_t new_index = vector->count;
  vector->count++;
  vector->items[new_index] = item;
}

static void *aiv_vector_at(aiv_vector_t *vector, size_t index) {
  guard(index, vector->count) return vector->items[index];
}


static size_t aiv_vector_size(aiv_vector_t *vector) { return vector->count; }

static bool aiv_vector_is_empty(aiv_vector_t *vector) { return !vector->count; }

static void aiv_vector_insert_at(aiv_vector_t *vector, size_t index, void *item) {
  __aiv_vector_resize(vector);

  memmove(vector->items + index + 1, vector->items + index,
          (vector->count - index) * sizeof(void *));

  vector->count++;
  vector->items[index] = item;
}

static void aiv_vector_remove_at(aiv_vector_t *vector, size_t index) {

  memmove(vector->items + index, vector->items + index + 1,
          (vector->count - index - 1) * sizeof(void *));

  vector->count--;
  __aiv_vector_resize(vector);
}


#endif