#include "game.h"

void init_dsu(DSU* dsu, int n) {
  for (int i = 0; i < n; i++) dsu->dpr[i] = i;
  dsu->connected_component = n;
}

int gpr(DSU* dsu, int x) {
  return dsu->dpr[x] = (x == dsu->dpr[x] ? x : gpr(dsu, x));
}

void merge(DSU* dsu, int u, int v) {
  dsu->dpr[gpr(dsu, u)] = dsu->dpr[gpr(dsu, v)];
}
