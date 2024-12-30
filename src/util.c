#include "game.h"

char *to_string(char c) {
  char *s = (char *)malloc(sizeof(char));
  s[0] = c;
  return s;
}

int rnd(int l, int r) {
  int x = rand() % (r - l + 1);
  return x + l;
}

int max(int x, int y) { return x < y ? y : x; }

int min(int x, int y) { return x < y ? x : y; }

void swap(int *x, int *y) {
  int t = *x;
  *x = *y;
  *y = t;
}

void rand_permute(int *a, int n) {
  for (int i = 1; i < n; i++) {
    swap(a + i, a + rnd(0, i - 1));
  }
}