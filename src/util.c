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

const char PASS_CHAR[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$^&*()-_="
    "+[]{}|;:,.<>?";

char *randomPass(int n) {
  const int SZ = strlen(PASS_CHAR);
  char *pass = (char *)malloc(sizeof(char) * n);
  for (int i = 0; i < n; i++) pass[i] = PASS_CHAR[rand() % SZ];
  return pass;
}

int sgn(int x) {
  if (x < 0) return -1;
  if (x > 0) return 1;
  return 0;
}

int myabs(int x) {
  return max(x, -x);
}