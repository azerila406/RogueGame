#include "game.h"

void vec_push(Vector *V, int x) {
  if (V == NULL) return;

  int new_sz = V->sz + 1;
  int *new_a = realloc(V->a, new_sz * sizeof(int));
  if (new_a == NULL) return;

  V->a = new_a;
  V->a[V->sz] = x;
  V->sz = new_sz;
}

void vec_pop(Vector *V) {
  if (V == NULL || V->sz == 0) return;

  int new_sz = V->sz - 1;
  int *new_a = realloc(V->a, new_sz * sizeof(int));

  if (new_a != NULL || new_sz == 0) {
    V->a = new_a;
    V->sz = new_sz;
  }
}

void vec_free(Vector *V) {
  if (V == NULL) return;

  free(V->a);
  V->a = NULL;
  V->sz = 0;
}

void vec_init(Vector *V) {
  V->a = NULL;
  V->sz = 0;
}

void vec_test() {
  Vector V;
  vec_init(&V);
  vec_push(&V, 10);
  printf("%d\n", V.sz);
  printf("%d\n", V.a[V.sz - 1]);
}