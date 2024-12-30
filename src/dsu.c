#include "game.h"

void initDSU(DSU* dsu, int n) {
  for (int i = 0; i < n; i++) dsu->dpr[i] = i;
  dsu->connected_component = n;
}

int gpr(DSU* dsu, int x) {
  assert(0 <= x && x < MAX_ROOMS_PER_LEVEL);
  return x == dsu->dpr[x] ? x : (dsu->dpr[x] = gpr(dsu, dsu->dpr[x]));
}

void merge(DSU* dsu, int u, int v) {
  u = gpr(dsu, u);
  v = gpr(dsu, v);
  if (u == v) return;
  --dsu->connected_component;
  dsu->dpr[u] = v;
}