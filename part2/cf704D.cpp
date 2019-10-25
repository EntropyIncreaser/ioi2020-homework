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

template <class Z>
struct MaxFlow {
  vector<vector<tuple<int, int, Z>>> g;

  struct Ref {
    const MaxFlow* p;
    int u, id;

    Ref() {}

    Ref(const MaxFlow* p, int u, int id) : p(p), u(u), id(id) {}

    Z get() const { return std::get<2>((p->g)[u][id]); }
  };

  MaxFlow() {}
  MaxFlow(int n) : g(n) {}

  Ref addEdge(int u, int v, const Z& w) {
    if (u == v) return Ref();
    int ru = g[u].size(), rv = g[v].size();
    g[u].emplace_back(v, rv, w);
    g[v].emplace_back(u, ru, 0);
    return Ref(this, v, rv);
  }

  Z dinic(int s, int t) {
    int n = g.size();
    function<vector<int>()> getLevel = [&]() {
      vector<int> level(n, -1);
      queue<int> q;
      level[s] = 0;
      q.push(s);
      while (!q.empty()) {
        int u = q.front(); q.pop();
        for (const auto& pr : g[u])
          if (get<2>(pr) && level[get<0>(pr)] == -1) {
            level[get<0>(pr)] = level[u] + 1;
            q.push(get<0>(pr));
          }
      }
      return level;
    };
    vector<int> r, level;
    function<Z(int, Z)> cap = [&](int u, Z limit) {
      if (u == t) return limit;
      Z ret = 0;
      while (r[u] && limit > ret) {
        int v = get<0>(g[u][r[u] - 1]), rev = get<1>(g[u][r[u] - 1]);
        Z& w = get<2>(g[u][r[u] - 1]);
        if (level[v] == level[u] + 1) {
          Z flow = cap(v, min(limit - ret, w));
          ret += flow;
          w -= flow;
          get<2>(g[v][rev]) += flow;
          if (limit == ret)
            return ret;
          --r[u];
        } else
          --r[u];
      }
      return ret;
    };
    Z ret = 0;
    while (level = getLevel(), level[t] != -1) {
      r = vector<int>(n);
      for (int i = 0; i < n; ++i)
        r[i] = g[i].size();
      ret += cap(s, numeric_limits<Z>::max());
    }
    return ret;
  }
};

const int N = 100010;

int x[N], y[N];
MaxFlow<int>::Ref rf[N];
int cx[N], cy[N];
int dx[N], dy[N];
int lx[N], ly[N];

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, m, r, b;
  cin >> n >> m >> r >> b;
  for (int i = 1; i <= n; ++i)
    cin >> x[i] >> y[i];
  copy(x + 1, x + n + 1, dx + 1);
  sort(dx + 1, dx + n + 1);
  copy(y + 1, y + n + 1, dy + 1);
  sort(dy + 1, dy + n + 1);
  fill(lx + 1, lx + n + 1, n);
  fill(ly + 1, ly + n + 1, n);
  int p = 1;
  for (int i = 1; i <= n; ++i) {
    if (dx[i] != dx[p])
      p = i;
    ++cx[p];
  }
  p = 1;
  for (int i = 1; i <= n; ++i) {
    if (dy[i] != dy[p])
      p = i;
    ++cy[p];
  }
  while (m--) {
    int t, l, d;
    cin >> t >> l >> d;
    if (t == 1) {
      int id = lower_bound(dx + 1, dx + n + 1, l) - dx;
      if (dx[id] == l)
        lx[id] = min(lx[id], d);
    } else {
      int id = lower_bound(dy + 1, dy + n + 1, l) - dy;
      if (dy[id] == l)
        ly[id] = min(ly[id], d);
    }
  }
  MaxFlow<int> bounded(n * 2 + 4);
  int S = n * 2, T = n * 2 + 1, s = n * 2 + 2, t = n * 2 + 3;
  for (int i = 1; i <= n; ++i) {
    int ix = lower_bound(dx + 1, dx + n + 1, x[i]) - dx - 1,
        iy = lower_bound(dy + 1, dy + n + 1, y[i]) - dy - 1;
    rf[i] = bounded.addEdge(ix, iy + n, 1);
  }
  int least = 0;
  for (int i = 1; i <= n; ++i) {
    if (cx[i] == 0) continue;
    int l = 0;
    if (cx[i] - lx[i] >= 0)
      l = (cx[i] - lx[i] + 1) / 2;
    int r = (cx[i] + lx[i]) / 2;
    if (r < l) {
      cout << "-1\n";
      return 0;
    }
    if (l > 0) {
      least += l;
      bounded.addEdge(S, i - 1, l);
      bounded.addEdge(s, T, l);
      r -= l;
    }
    bounded.addEdge(s, i - 1, r);
  }
  for (int i = 1; i <= n; ++i) {
    if (cy[i] == 0) continue;
    int l = 0;
    if (cy[i] - ly[i] >= 0)
      l = (cy[i] - ly[i] + 1) / 2;
    int r = (cy[i] + ly[i]) / 2;
    if (r < l) {
      cout << "-1\n";
      return 0;
    }
    if (l > 0) {
      least += l;
      bounded.addEdge(S, t, l);
      bounded.addEdge(n + i - 1, T, l);
      r -= l;
    }
    bounded.addEdge(n + i - 1, t, r);
  }
  bounded.addEdge(t, s, n);
  if (bounded.dinic(S, T) != least) {
    cout << "-1\n";
    return 0;
  }
  bounded.g[s].pop_back();
  bounded.g[t].pop_back();
  bounded.dinic(s, t);
  int flow = 0;
  for (int i = 1; i <= n; ++i)
    flow += rf[i].get();
  if (r < b) {
    cout << (r * (ll)flow + b * (ll)(n - flow)) << '\n';
    for (int i = 1; i <= n; ++i)
      cout << (rf[i].get() ? 'r' : 'b');
  } else {
    cout << (b * (ll)flow + r * (ll)(n - flow)) << '\n';
    for (int i = 1; i <= n; ++i)
      cout << (rf[i].get() ? 'b' : 'r');
  }

  return 0;
}
