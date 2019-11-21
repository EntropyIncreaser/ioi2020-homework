#include <cassert>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cctype>

#include <algorithm>
#include <tuple>
#include <random>
#include <bitset>
#include <chrono>
#include <queue>
#include <functional>
#include <set>
#include <map>
#include <vector>
#include <iostream>
#include <limits>
#include <numeric>

#ifdef LBT

#define LOG(FMT...) fprintf(stderr, FMT)

#else

#define LOG(FMT...)

#endif

using namespace std;

typedef long long ll;
typedef unsigned long long ull;

// mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

struct E {
  int v, i;
  E* next;
};

const int M = 200010, N = 400010;

int a, b, m, c;
int u[M], v[M], id[M], ans[M];
int deg[N], stk[N], si[N];
bool curc[N];
tuple<int, int, int> dat[M];
bool vis[N], evs[M], col[M];
E pool[M * 2 + N];
E *g[N], *t[N], *pt;

void adde(E** gg, int u, int v, int i) {
  E* p = pt++;
  p->v = v;
  p->i = i;
  p->next = gg[u];
  gg[u] = p;
}

void dis(int uu) {
  if (vis[uu]) return;
  int s = 0;
  ++s; stk[s] = uu; vis[uu] = true; si[s] = -1;
  while (s) {
    int u = stk[s];
    if (si[s] != -1) {
      adde(t, u, g[u]->v, g[u]->i);
      ++deg[u];
      g[u] = g[u]->next;
    }
    si[s] = 1;
    while (g[u] && evs[g[u]->i]) g[u] = g[u]->next;
    if (!g[u]) {
      --s;
      continue;
    }
    int v = g[u]->v, i = g[u]->i;
    evs[i] = true;
    if (vis[v]) {
      bool f = false;
      while (stk[s] != v) {
        vis[stk[s]] = false;
        col[g[stk[s]]->i] = f;
        g[stk[s]] = g[stk[s]]->next;
        f = !f;
        --s;
      }
      col[g[v]->i] = f;
      g[v] = g[v]->next;
      si[s] = -1;
    } else {
      ++s;
      stk[s] = v;
      si[s] = -1;
      vis[v] = true;
    }
  }
  int ql = 0, qr = 0;
  stk[++qr] = uu;
  while (ql < qr) {
    int u = stk[++ql];
    E* p = t[u];
    int a = deg[u] / 2;
    bool f = curc[u];
    while (a--) {
      col[p->i] = f;
      curc[p->v] = f;
      stk[++qr] = p->v;
      p = p->next;
    }
    f = !f;
    while (p) {
      col[p->i] = f;
      curc[p->v] = f;
      stk[++qr] = p->v;
      p = p->next;
    }
  }
}

void solve(int l, int r) {
  bool f = false;
  pt = pool;
  for (int i = l; i <= r; ++i) {
    ++deg[u[i]]; ++deg[v[i]];
    f |= deg[u[i]] > 1 || deg[v[i]] > 1;
  }
  if (!f) {
    ++c;
    for (int i = l; i <= r; ++i) {
      deg[u[i]] = deg[v[i]] = 0;
      ans[id[i]] = c;
    }
    return;
  }
  for (int i = l; i <= r; ++i) {
    adde(g, u[i], v[i], i);
    adde(g, v[i], u[i], i);
    deg[u[i]] = deg[v[i]] = 0;
  }
  for (int i = l; i <= r; ++i) {
    dis(u[i]);
    dis(v[i]);
  }
  int cnt = count(evs + l, evs + r + 1, true);
  for (int i = l; i <= r; ++i) {
    deg[u[i]] = deg[v[i]] = 0;
    vis[u[i]] = vis[v[i]] = evs[i] = false;
    g[u[i]] = g[v[i]] = t[u[i]] = t[v[i]] = NULL;
  }
  int il = l, ir = r;
  for (int i = l; i <= r; ++i)
    dat[col[i] ? ir-- : il++] = make_tuple(id[i], u[i], v[i]);
  for (int i = l; i <= r; ++i)
    tie(id[i], u[i], v[i]) = dat[i];
  for (int i = 1; i <= il - 1; ++i)
    ans[get<0>(dat[i])] = true;
  for (int i = ir + 1; i <= m; ++i)
    ans[get<0>(dat[i])] = false;
}

int main() {
#ifdef LBT
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif

  pt = pool;
  a = b = 200000;
  scanf("%d", &m);
  for (int i = 1; i <= m; ++i) {
    scanf("%d%d", &u[i], &v[i]);
    v[i] += a;
  }
  iota(id + 1, id + m + 1, 1);
  solve(1, m);
  for (int i = 1; i <= m; ++i)
    putchar(ans[i] ? 'r' : 'b');

#ifdef LBT
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
