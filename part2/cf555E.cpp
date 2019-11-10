#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cctype>

#include <algorithm>
#include <random>
#include <bitset>
#include <queue>
#include <functional>
#include <set>
#include <map>
#include <vector>
#include <chrono>
#include <iostream>
#include <limits>
#include <numeric>

#define LOG(FMT...) fprintf(stderr, FMT)

using namespace std;

typedef long long ll;
typedef unsigned long long ull;

// mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

template <class T>
istream& operator>>(istream& is, vector<T>& v) {
  for (T& x : v)
    is >> x;
  return is;
}

template <class T>
ostream& operator<<(ostream& os, const vector<T>& v) {
  if (!v.empty()) {
    os << v.front();
    for (int i = 1; i < v.size(); ++i)
      os << ' ' << v[i];
  }
  return os;
}

const int N = 200010, L = 18;

int n, m, q;
vector<pair<int, int>> g[N];

int eu[N], ev[N], f[N], prt[N], pid[N], dep[N], st[N][L];
int up[N], dn[N];
bool vis[N], vise[N];

int find(int x) { return x == f[x] ? x : f[x] = find(f[x]); }

void dfs(int u) {
  vis[u] = true;
  st[u][0] = prt[u];
  for (int i = 1; i < L; ++i)
    st[u][i] = st[st[u][i - 1]][i - 1];
  for (auto [v, id] : g[u])
    if (!vise[id]) {
      vise[id] = true;
      if (!vis[v]) {
        prt[v] = u;
        pid[v] = id;
        dep[v] = dep[u] + 1;
        dfs(v);
      } else {
        int x = find(u), y = find(v);
        while (x != y) {
          if (dep[x] < dep[y]) swap(x, y);
          f[x] = prt[x];
          x = find(x);
        }
      }
    }
}

int lca(int u, int v) {
  if (dep[u] < dep[v]) swap(u, v);
  for (int i = 0; i < L; ++i)
    if ((dep[u] - dep[v]) >> i & 1)
      u = st[u][i];
  if (u == v) return u;
  for (int i = L - 1; i >= 0; --i)
    if (st[u][i] != st[v][i]) {
      u = st[u][i];
      v = st[v][i];
    }
  return st[u][0];
}

void dfs2(int u) {
  for (auto [v, id] : g[u])
    if (pid[v] == id) {
      dfs2(v);
      up[u] += up[v];
      dn[u] += dn[v];
    }
  if (f[u] == u) {
    if (up[u] && dn[u]) {
      puts("No");
      exit(0);
    }
  }
}

int main() {
#ifdef LBT
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> m >> q;
  for (int i = 1; i <= m; ++i) {
    int u, v;
    cin >> u >> v;
    eu[i] = u;
    ev[i] = v;
    g[u].emplace_back(v, i);
    g[v].emplace_back(u, i);
  }
  iota(f + 1, f + n + 1, 1);
  for (int i = 1; i <= n; ++i)
    if (!vis[i]) {
      dep[i] = 1;
      dfs(i);
    }
  while (q--) {
    int s, t;
    cin >> s >> t;
    int l = lca(s, t);
    if (l == 0) {
      puts("No");
      return 0;
    }
    ++up[s]; --up[l];
    ++dn[t]; --dn[l];
  }
  for (int i = 1; i <= n; ++i)
    if (dep[i] == 1)
      dfs2(i);
  puts("Yes");

#ifdef LBT
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
