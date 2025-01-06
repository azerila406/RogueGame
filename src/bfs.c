#include "game.h"

int pr[HEIGHT][WIDTH][2], dist[HEIGHT][WIDTH], X[HEIGHT * WIDTH],
    Y[HEIGHT * WIDTH];
const int dx[4] = {0, 0, -1, +1};
const int dy[4] = {1, -1, 0, 0};

/* CHEECCCCCCCCCCCCCCCK */
void BFS(Level *l, int sx, int sy, bool (*check)(Level *, int, int)) {
  memset(pr, -1, sizeof pr);
  memset(dist, 0x3f, sizeof dist);
  int L = 0, R = 0;
  X[R] = sx;
  Y[R] = sy;
  ++R;
  dist[sx][sy] = 0;
  while (L < R) {
    int x = X[L], y = Y[L];
    ++L;
    if (!check(l, x, y) && dist[x][y]) continue;
    int P[4] = {0, 1, 2, 3};
    rand_permute(P, 4);
    for (int i = 0; i < 4; i++) {
      int nx = x + dx[P[i]];
      int ny = y + dy[P[i]];
      if (nx < 0 || ny < 0 || nx >= HEIGHT || ny >= WIDTH ||
          dist[x][y] + 1 >= dist[nx][ny])
        continue;
      X[R] = nx;
      Y[R] = ny;
      ++R;
      pr[nx][ny][0] = x;
      pr[nx][ny][1] = y;
      dist[nx][ny] = dist[x][y] + 1;
    }
  }
}

bool checkDoors(Level *L, int x, int y) {
  int t = L->tile[x][y].type;
  if (t == 1 || t == 3 || t == 6) return 0;
  return 1;
}

bool bfsDoor(Level *l, int sx, int sy, int c, bool replace) {
  BFS(l, sx, sy, checkDoors);
  int x = -1, y = -1;
  for (int i = getX0(&(l->room[c])); i <= getX1(&(l->room[c])); ++i) {
    for (int j = getY0(&(l->room[c])); j <= getY1(&(l->room[c])); ++j)
      if (pr[i][j][0] != -1) {
        int t = l->tile[i][j].type;
        if ((t == 1 || t == 3) && dist[i][j] >= MIN_HALLWAY_LEN) {
          if (x == -1 || rand() % 5) {  // randomness is not uniform
            x = i;
            y = j;
          }
        }
      }
  }
  if (x == -1) return 0;
  if (replace) {
    l->tile[x][y].type = 6;
    while (pr[x][y][0] != -1) {
      int tx = pr[x][y][0];
      int ty = pr[x][y][1];
      x = tx;
      y = ty;
      l->tile[x][y].type = 2;
    }
    l->tile[x][y].type = 6;
    assert(x == sx && y == sy);
  }
  return 1;
}
